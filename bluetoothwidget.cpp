#include "bluetoothwidget.h"
#include "ui_bluetoothwidget.h"

#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

BluetoothWidget::BluetoothWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BluetoothWidget)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    counter = 0;

    readerThread = new BTReaderThread(this);
    parserThread = new DataParserThread(this);
    treatmentThread = new DataTreatmentThread(this);
    decisionThread = new DecisionThread(this);


    connect(readerThread,SIGNAL(DataBytesSignal(QByteArray)),parserThread,SLOT(onDataBytes(QByteArray)));
    connect(parserThread,SIGNAL(ChannelsDataSignal(QStringList)),treatmentThread,SLOT(onChannelsData(QStringList)));
    connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),decisionThread,SLOT(onChannelResults(QStringList)));

    connect(parserThread, SIGNAL(ShowDataSignal(QString)), this, SLOT(newData(QString)));
    connect(readerThread, SIGNAL(ShowErrorSignal(QString)),this,SLOT(newError(QString)));
    connect(readerThread, SIGNAL(OpenedSignal()),this,SLOT(openedSerialPort()));
    connect(readerThread, SIGNAL(ClosedSignal()),this,SLOT(closedSerialPort()));
    connect(treatmentThread,SIGNAL(ShowResultsSignal(QStringList)),this,SLOT(newResults(QStringList)));
    connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));

    parserThread->start();
    treatmentThread->start();
    decisionThread->start();
    readerThread->start();

    //connect(ui->startButton, SIGNAL(clicked()), this, SLOT(openPort()));
    //connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(closePort()));

    moveMouse = new QShortcut(Qt::Key_Space, this);
    mouseState = 1;
    connect(moveMouse, SIGNAL(activated()), this, SLOT(mouseMovementControl()));
}


void BluetoothWidget::openPort(){
    ui->startButton->setEnabled(false);
    timer->start(1000);
    emit openSignal("COM15");
}

void BluetoothWidget::closePort(){
    emit closeSignal();
    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);
}

BluetoothWidget::~BluetoothWidget()
{
    emit widgetClosedSignal();
    delete moveMouse;
    delete readerThread;
    delete parserThread;
    delete treatmentThread;
    delete decisionThread;
    delete ui;
}

void BluetoothWidget::openedSerialPort()
{
    ui->stopButton->setEnabled(true);
}

void BluetoothWidget::closedSerialPort()
{
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
              stream << QString::QString(finalDataList.at(i)) << endl;
              //stream << "\n\r";
        }
    }
    ui->textEdit->insertPlainText("\nMuestras guardadas en "+filename);
    counter = 0;
}

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

void BluetoothWidget::newMovement(QList<int> coord){
    //qDebug()<<"Moviemiento: horizontal "<<coord[0]<<" vertical "<<coord[1]<<endl;
    QPoint p = cur->pos();
    p.setY(p.y()+
           coord[1]);
    p.setX(p.x()+coord[0]);
    cur->setPos(p);
}

void BluetoothWidget::mouseMovementControl(){
    if(mouseState){
        disconnect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
        ui->stateLabel->setText("Estado actual: Stopped");
    }
    else{
        connect(decisionThread,SIGNAL(MovementSignal(QList<int>)),this,SLOT(newMovement(QList<int>)));
        ui->stateLabel->setText("Estado actual: Running");
    }
    mouseState = 1-mouseState;
}


