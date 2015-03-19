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
    serial = new QSerialPort(this);

    ui->stopButton->setEnabled(false);
    ui->startButton->setEnabled(true);

    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(openSerialPort()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(closeSerialPort()));

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

    timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    counter = 0;

}

BluetoothReader::~BluetoothReader()
{
    delete ui;
}

void BluetoothReader::openSerialPort()
{
    ui->startButton->setEnabled(false);
    serial->setPortName("COM15");
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadOnly)) {
            ui->statusBar->showMessage(tr("Connected"));
            ui->stopButton->setEnabled(true);
            ui->startButton->setEnabled(false);
            timer->start(1000);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
        ui->startButton->setEnabled(true);
        ui->statusBar->showMessage(tr("Open error"));
    }
}

void BluetoothReader::closeSerialPort()
{
    serial->close();
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
    QString filename="DataSample.txt";
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

}

void BluetoothReader::readData()
{
    QByteArray data = serial->readLine();
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

void BluetoothReader::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void BluetoothReader::update(){
    counter++;
    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Numero de muestras: "+QString::number(finalDataList.size())+". Segundos en funcionamiento: "+QString::number(counter));
}

