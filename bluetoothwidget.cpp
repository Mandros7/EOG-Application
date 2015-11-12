#include "bluetoothwidget.h"
#include "ui_bluetoothwidget.h"

#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

//--------------- WIDGET BLUETOOTH --------------//
//Muestra el estado de la conexión en tiempo real//
BluetoothWidget::BluetoothWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothWidget)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(false);

    QSettings settings(QString("./configs/config.ini"), QSettings::IniFormat);
    ui->uThresholdlineEdit->setText(settings.value("Upthreshold").toString());
    ui->dThresholdlineEdit->setText(settings.value("Downthreshold").toString());
    ui->speedlineEdit->setText(settings.value("Speed").toString());
    ui->minBlinklineEdit->setText(settings.value("Minblink").toString());
    ui->maxBlinklineEdit->setText(settings.value("Maxblink").toString());


    //Timer que cuenta los segundos de ejecucion de la ventana
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    counter = 0;

    //Atajo de desactivación de movimiento
    moveMouse = new QShortcut(Qt::Key_Escape, this);
    connect(moveMouse, SIGNAL(activated()), this, SLOT(on_shortcut()));
    //connect(ui->startButton, SIGNAL(clicked()), this, SLOT(openPort()));
    //connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(closePort()));
}


void BluetoothWidget::openPort(){
    ui->startButton->setEnabled(false);
    timer->start(1000);
    emit clickedStart();
}

void BluetoothWidget::closePort(){
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    emit clickedStop();
}

BluetoothWidget::~BluetoothWidget()
{
    emit widgetClosedSignal();
    delete ui;
}

void BluetoothWidget::openedSerialPort()
{
    ui->stopButton->setEnabled(true);
}

void BluetoothWidget::closedSerialPort()
{
    //Al cerrarse el puerto serie, se genera una archivo con todos los datos recibidos y
    // se indica el numero total de muestras
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
    timer->stop();
    ui->textEdit->clear();
   for (int i = 0; i<finalDataList.size();i++){
          ui->textEdit->append(finalDataList.at(i));
          ui->textEdit->ensureCursorVisible();
    }
    ui->textEdit->insertPlainText("\nNumero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
    QString filename="DataSample_Diagonal.txt";
    QFile file( filename );
    if ( file.open(QIODevice::WriteOnly|QIODevice::Text) )
    {
        QTextStream stream(&file);
        for (int i = 0; i<finalDataList.size();i++){
              stream << QString(finalDataList.at(i)) << endl;
              //stream << "\n\r";
        }
    }
    ui->textEdit->insertPlainText("\nMuestras guardadas en "+filename);

    counter = 0;
}

/* FUNCIONES DE RECEPCION DE DATOS DEL ESTADO DE LA CONEXION
 * Datos, resultados, mensajes de informacion o de actualizacion del timer.
 * */
void BluetoothWidget::newData(QString data)
{
    ui->textEditData->append(data);
    ui->textEditData->ensureCursorVisible();
    finalDataList.append(data);
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

void BluetoothWidget::newError(QString info)
{
        QMessageBox::critical(this, tr("Error"), info);
}

void BluetoothWidget::update(){
    counter++;
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

void BluetoothWidget::newResults(QStringList results){
    ui->horizontalLineEdit->setText(results[0]);
    ui->verticalLineEdit->setText(results[1]);
}

void BluetoothWidget::newDecision(QStringList results){
    ui->hDecisionLineEdit->setText(results[0]);
    ui->vDecisionLineEdit->setText(results[1]);
}

void BluetoothWidget::newBlink(bool performed){
    if(performed){
        ui->blinkLineEdit->setText("SI");
    }
    else{
         ui->blinkLineEdit->setText("NO");
    }
}

void BluetoothWidget::on_shortcut(){
    emit shortcutChange();
}

void BluetoothWidget::printSamples(QList<QStringList> samples){

    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));

    for (int i = 0;i<samples.length();i++){
        ui->textEdit->insertPlainText("\n"+(samples[i])[0] + " " +(samples[i])[1]);
    }
}



void BluetoothWidget::on_updatePushButton_clicked()
{
    if ((ui->dThresholdlineEdit->text().toInt()>0)&&
        (ui->uThresholdlineEdit->text().toInt()>ui->dThresholdlineEdit->text().toInt())&&
            (ui->minBlinklineEdit->text().toInt()>0)&&
            (ui->maxBlinklineEdit->text().toInt()>ui->minBlinklineEdit->text().toInt())&&
                (ui->speedlineEdit->text().toInt()>0)){

        QSettings settings(QString("./configs/config.ini"), QSettings::IniFormat);
        settings.setValue("Upthreshold",ui->uThresholdlineEdit->text().toInt());
        settings.setValue("Downthreshold",ui->dThresholdlineEdit->text().toInt());
        settings.setValue("Maxblink",ui->maxBlinklineEdit->text().toInt());
        settings.setValue("MinBlink",ui->minBlinklineEdit->text().toInt());
        settings.setValue("Speed",ui->speedlineEdit->text().toInt());

        //ui->textEdit->clear();
        //ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
        //ui->textEdit->insertPlainText("\n Configuracion de la aplicacion actualizada!\n");
    ui->stateLabel->setText("Estado actual: Configuración actualizada (reiniciar)");
    }
}
