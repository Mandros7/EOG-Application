#include "btreaderthread.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>

BTReaderThread::BTReaderThread(QObject *parent) : QThread (parent)
{    
    serial = new QSerialPort(this);
}

void BTReaderThread::run()
{
    if (serial!=NULL){
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this->parent(),SIGNAL(openSignal(QString)),this,SLOT(openSerialPort(QString)));
    connect(this->parent(),SIGNAL(closeSignal()),this,SLOT(closeSerialPort()));
    }
    else {
        emit ErrorSignal("Error al iniciar el puerto serie");
    }
}

void BTReaderThread::openSerialPort(QString portName)
{
    qDebug() << "Llega" << endl;
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadOnly)) {
        emit OpenedSignal();
    } else {
        emit ErrorSignal(serial->errorString());
    }
}

void BTReaderThread::closeSerialPort()
{
    serial->close();
    emit ClosedSignal();
}

void BTReaderThread::readData()
{
    QByteArray data = serial->readLine();
    emit DataSignal(data);
    //Send to buffer.
}

void BTReaderThread::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort();
        emit ErrorSignal(serial->errorString());
    }
}
