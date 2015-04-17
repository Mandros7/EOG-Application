#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    musicPlayerRunning = false;
    bluetoothTestRunning = false;
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    QString mPath = settings.value("MusicPath").toString();
    newMusicPath(mPath);

    readerThread = new BTReaderThread(this);
    parserThread = new DataParserThread(this);
    treatmentThread = new DataTreatmentThread(this);
    decisionThread = new DecisionThread(this);


    connect(readerThread,SIGNAL(DataBytesSignal(QByteArray)),parserThread,SLOT(onDataBytes(QByteArray)));
    connect(parserThread,SIGNAL(ChannelsDataSignal(QStringList)),treatmentThread,SLOT(onChannelsData(QStringList)));
    connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),decisionThread,SLOT(onChannelResults(QStringList)));


    connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));

    parserThread->start();
    treatmentThread->start();
    decisionThread->start();
    readerThread->start();

    moveMouse = new QShortcut(Qt::Key_Escape, this);
    mouseState = 1;
    connect(moveMouse, SIGNAL(activated()), this, SLOT(mouseMovementControl()));
}

MainWindow::~MainWindow()
{
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
 *           La proxima vez que se abra utilizará el nuevo directorio (musicPath).
 */
void MainWindow::playerClosed(){
    qDebug() << "closed";
    musicPlayerRunning = false;
    disconnect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
}

void MainWindow::newMusicPath(QString newPath){
    //qDebug() << Q_FUNC_INFO;
    qDebug() << "recieved" << newPath;
    musicPath = newPath;
    emit musicSettingsChanged();
}

void MainWindow::openBluetooth(){

    if (bluetoothTestRunning){
        bWidget->show();
    }
    else{
        bWidget = new BluetoothWidget();
        bWidget->setAttribute( Qt::WA_DeleteOnClose );
        bWidget->show();
        bluetoothTestRunning = true;
        connect(bWidget,SIGNAL(widgetClosedSignal()),this,SLOT(onClosedBTest()));

        connect(parserThread, SIGNAL(ShowDataSignal(QString)), bWidget, SLOT(newData(QString)));
        connect(readerThread, SIGNAL(ShowErrorSignal(QString)),bWidget,SLOT(newError(QString)));
        connect(readerThread, SIGNAL(OpenedSignal()),bWidget,SLOT(openedSerialPort()));
        connect(readerThread, SIGNAL(ClosedSignal()),bWidget,SLOT(closedSerialPort()));
        connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),bWidget,SLOT(newResults(QStringList)));
        connect(bWidget, SIGNAL(shortcutChange()), this, SLOT(mouseMovementControl()));
        connect(bWidget, SIGNAL(clickedStart()), this, SLOT(openPort()));
        //connect(bWidget, SIGNAL(clickedStop()), this, SLOT(closePort()));
    }
}

void MainWindow::onClosedBTest(){
    bluetoothTestRunning = false;
    disconnect(bWidget,SIGNAL(widgetClosedSignal()),this,SLOT(onClosedBTest()));

    disconnect(parserThread, SIGNAL(ShowDataSignal(QString)), bWidget, SLOT(newData(QString)));
    disconnect(readerThread, SIGNAL(ShowErrorSignal(QString)),bWidget,SLOT(newError(QString)));
    disconnect(readerThread, SIGNAL(OpenedSignal()),bWidget,SLOT(openedSerialPort()));
    disconnect(readerThread, SIGNAL(ClosedSignal()),bWidget,SLOT(closedSerialPort()));
    disconnect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),bWidget,SLOT(newResults(QStringList)));
}

void MainWindow::openPort(){
    emit openSignal();
}

void MainWindow::closePort(){
    emit closeSignal();
}

/* ------------------------ REGISTRO DE ACCIONES (sobre Qbuttons) ----------------------------
 *              Se lanzan los dialogos del reproductor de musica y de configuración
 *              Se conecta la señal de cambio de configuración a la función de cierre
 *                  del reproductor.
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
    if(mouseState){
        qDebug()<<"Test1"<<endl;
        disconnect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
    }
    else{
        qDebug()<<"Test2"<<endl;
        connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
    }
    mouseState = 1-mouseState;
    qDebug()<<"Test"<<endl;
}

void MainWindow::newMovement(QList<int> coord){
    //qDebug()<<"Moviemiento: horizontal "<<coord[0]<<" vertical "<<coord[1]<<endl;
    QPoint p = cur->pos();
    p.setY(p.y()+
           coord[1]);
    p.setX(p.x()+coord[0]);
    cur->setPos(p);
}
