#include "BluetoothReader.h"
#include "ui_BluetoothReader.h"

#include <QtSerialPort/QSerialPort>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

BluetoothReader::BluetoothReader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BluetoothReader)
{
    ui->setupUi(this);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    counter = 0;

    thread = new BTReaderThread(this);
    thread->start();
    connect(thread, SIGNAL(DataSignal(QByteArray)), this, SLOT(newData(QByteArray)));
    connect(thread, SIGNAL(ErrorSignal(QString)),this,SLOT(newError(QString)));

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(openPort()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(closePort()));

}

void BluetoothReader::openPort(){
    emit openSignal("COM15");
}

void BluetoothReader::closePort(){
    emit closeSignal();
}

BluetoothReader::~BluetoothReader()
{
    delete ui;
    delete thread;
}

void BluetoothReader::openedSerialPort()
{
    ui->startButton->setEnabled(false);
}

void BluetoothReader::closedSerialPort()
{
    ui->statusBar->showMessage(tr("Disconnected"));
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

void BluetoothReader::newData(QByteArray data)
{
    ui->textEditData->append(data);
    ui->textEditData->ensureCursorVisible();
    dataString = dataString + QString::QString(data);
    QList<QString> dataList = dataString.split("\n\r");
    dataString = dataList.last();
    dataList.removeLast();
    finalDataList.append(dataList);
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

void BluetoothReader::newError(QString info)
{
        QMessageBox::critical(this, tr("Error"), info);
}

void BluetoothReader::update(){
    counter++;
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

