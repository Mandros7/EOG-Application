#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qRegisterMetaType<QList<int> >("QList<int>");

    ui->setupUi(this);
    //Variables de estado de las ventanas de música y recepción (configuración)
    musicPlayerRunning = false;
    bluetoothTestRunning = false;

    //Carga del directorio de música desde archivo de configuración
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    QString mPath = settings.value("MusicPath").toString();
    newMusicPath(mPath);

    //Se definen los hilos de lectura, parseo, tratamiento y decision
    readerThread = new BTReaderThread(this);
    parserThread = new DataParserThread(this);
    treatmentThread = new DataTreatmentThread(this);
    decisionThread = new DecisionThread(this);

    //Se realizan las conexiones entre los hilos
    connect(readerThread,SIGNAL(DataBytesSignal(QByteArray)),parserThread,SLOT(onDataBytes(QByteArray)));
    connect(parserThread,SIGNAL(ChannelsDataSignal(QStringList)),treatmentThread,SLOT(onChannelsData(QStringList)));
    connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),decisionThread,SLOT(onChannelResults(QStringList)));

    /*Conexion final en la que la interfaz recibe las instrucciones de movimiento
    por parte del núcleo funcional del programa */
    connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
    connect(decisionThread,SIGNAL(BlinkSignal(bool)),this,SLOT(newBlink(bool)));
    //Se inician los hilos
    parserThread->start();
    treatmentThread->start();
    decisionThread->start();
    readerThread->start();

    //Atajo para activar y desactivar las instrucciones de movimiento de cursor
    moveMouse = new QShortcut(Qt::Key_Escape, this);
    mouseState = 1;
    connect(moveMouse, SIGNAL(activated()), this, SLOT(mouseMovementControl()));
}

MainWindow::~MainWindow()
{
    //Liberacion de memoria
    delete moveMouse;
    delete readerThread;
    delete parserThread;
    delete treatmentThread;
    delete decisionThread;
    delete ui;
}

/*-------------------------------------- SLOTS --------------------------------------------//
 *
 *       La ventana de menú principal recibe la siguiente informacion:
 *           - Reproductor cerrado. Es necesario registrar esta señal para
 *           saber cuando crear un nuevo dialogo y asi evitar crear dos dialogos concurrentes
 *           - Nuevo directorio de música. Cuando la ventana de configuración
 *           envíe una señal de cambio de directorio, la ventana principal (menú) actuará
 *           como intermediario, emitiendo una señal que cierra el reproductor.
 *           La proxima vez que se abra utilizará el nuevo directorio (musicPath). Este path del
 *           directorio también se recibe a través del archivo de configuración config.ini
 *           - Apertura y cierre de Bluetooth.
 *
 */
void MainWindow::playerClosed(){
    qDebug() << "closed";
    musicPlayerRunning = false;
    disconnect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
}

void MainWindow::newMusicPath(QString newPath){
    qDebug() << "recieved" << newPath;
    musicPath = newPath;
    emit musicSettingsChanged(); //Comunica al reproductor que el directorio ha cambiado.
}

void MainWindow::openBluetooth(){
    /*Estructura de comprobación, que evita generar más de una instancia de
     * la ventana de control de Bluetooth. Para ello comprueba la variable de estado
     * "BluetoothTestRunning"
     */
    if (bluetoothTestRunning){
        bWidget->show();
    }
    else{
        bWidget = new BluetoothWidget();
        bWidget->setAttribute( Qt::WA_DeleteOnClose );
        bWidget->show();
        bluetoothTestRunning = true;
        connect(bWidget,SIGNAL(widgetClosedSignal()),this,SLOT(onClosedBTest()));

        //Señales que envían los datos en estados intermedios para mostrarlos en el widget de bluetooth
        connect(parserThread, SIGNAL(ShowDataSignal(QString)), bWidget, SLOT(newData(QString)));
        connect(readerThread, SIGNAL(ShowErrorSignal(QString)),bWidget,SLOT(newError(QString)));
        connect(readerThread, SIGNAL(OpenedSignal()),bWidget,SLOT(openedSerialPort()));
        connect(readerThread, SIGNAL(ClosedSignal()),bWidget,SLOT(closedSerialPort()));
        connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),bWidget,SLOT(newResults(QStringList)));
        connect(decisionThread,SIGNAL(BlinkSignal(bool)),bWidget,SLOT(newBlink(bool)));
        connect(bWidget, SIGNAL(shortcutChange()), this, SLOT(mouseMovementControl()));
        //connect(bWidget, SIGNAL(clickedStart()), this, SLOT(openPort()));
        //connect(bWidget, SIGNAL(clickedStop()), this, SLOT(closePort()));
    }
}

void MainWindow::onClosedBTest(){
    //Se cambia la variable de estado y se elimina la subscripción del widget a señales de los hilos
    bluetoothTestRunning = false;
    disconnect(bWidget,SIGNAL(widgetClosedSignal()),this,SLOT(onClosedBTest()));

    disconnect(parserThread, SIGNAL(ShowDataSignal(QString)), bWidget, SLOT(newData(QString)));
    disconnect(readerThread, SIGNAL(ShowErrorSignal(QString)),bWidget,SLOT(newError(QString)));
    disconnect(readerThread, SIGNAL(OpenedSignal()),bWidget,SLOT(openedSerialPort()));
    disconnect(readerThread, SIGNAL(ClosedSignal()),bWidget,SLOT(closedSerialPort()));
    disconnect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),bWidget,SLOT(newResults(QStringList)));
    disconnect(decisionThread,SIGNAL(BlinkSignal(bool)),bWidget,SLOT(newBlink(bool)));
}
//Estas funciones son "deprecated". Su uso no es recomendable, o es innecesario en la mayoria de casos
void MainWindow::openPort(){
    emit openSignal();
}

void MainWindow::closePort(){
    emit closeSignal();
}

/* ------------------------ REGISTRO DE ACCIONES (sobre Qbuttons) ----------------------------
 *              Se lanzan los dialogos del reproductor de musica, configuración y teclado
 *              Se conecta la señal de cambio de configuración a la función de cierre
 *                  del reproductor.
 *              Aquí se encuentra la acción automatizada de movimiento de cursor, asi como
 *              la activación y desactivación de ésta
 */
void MainWindow::on_musicButton_clicked()
{
    if (musicPlayerRunning){
        mDialog->show();
    }
    else{
        mDialog = new MusicDialog(musicPath,this);
        musicPlayerRunning = true;
        mDialog->show();
        connect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
    }
}

void MainWindow::on_settingsButton_clicked()
{
        sDialog = new SettingsDialog(this);
        //settingsRunning = true;      
        sDialog->show();
}


void MainWindow::on_inputButton_clicked()
{
    iDialog = new InputDialog(this);
    iDialog->show();
}

void MainWindow::mouseMovementControl(){

    //Logica de estados simple que permite activar y desactivar el movimiento de cursor
    if(mouseState){
        qDebug()<<"Test1"<<endl;
        disconnect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
        disconnect(decisionThread,SIGNAL(BlinkSignal(bool)),this,SLOT(newBlink(bool)));
    }
    else{
        qDebug()<<"Test2"<<endl;
        connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
        connect(decisionThread,SIGNAL(BlinkSignal(bool)),this,SLOT(newBlink(bool)));
    }
    mouseState = 1-mouseState;
    qDebug()<<"Test"<<endl;
}

void MainWindow::newMovement(QList<int> coord){
    //Cuando llegan nuevas coordenadas de cursor, se actualiza la posicion (set) en ambos ejes
    QPoint p = cur->pos();
    p.setY(p.y()+
           coord[1]);
    p.setX(p.x()+coord[0]);
    cur->setPos(p);
    if (timer.isValid()){
         processedTimeStamps << timer.nsecsElapsed();
    }
    else{
        processedTimeStamps << -1;
        timer.start();
    }
}

void MainWindow::newBlink(bool performed){
    if (performed){
        QWidget *p = qApp->widgetAt(cur->pos());
        if (p){
            QMetaObject::invokeMethod(p, "clicked");
        }
    }
}

void MainWindow::dataSentTimeStamp(){
    if (timer.isValid()){
         receivedTimeStamps << timer.nsecsElapsed();
    }
    else{
        receivedTimeStamps << -1;
        timer.start();
    }
}
void MainWindow::saveData(){
    QFile file("delay_1ms.txt");
    if ( file.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append ) )
    {
        QTextStream stream(&file);
        for (int i = 0; i<processedTimeStamps.size();i++){
            stream <<  receivedTimeStamps.at(i) << "\t" << processedTimeStamps.at(i) << "\t"
                    << processedTimeStamps.at(i)- receivedTimeStamps.at(i) << "\t" <<
                       processedTimeStamps.at(i+1) - processedTimeStamps.at(i) << "\t" <<
                       receivedTimeStamps.at(i+1) - receivedTimeStamps.at(i) << endl;
        }
    }
    file.close();
    qDebug()<<"FINISHED";
}

