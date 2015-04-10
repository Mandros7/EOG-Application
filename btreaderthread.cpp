#include "btreaderthread.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>

BTReaderThread::BTReaderThread(QObject *parent) : QThread (parent)
{    
    serial = new QSerialPort(this);
    sleeptime = 33;
}

void BTReaderThread::run()
{
    qDebug()<<"Reader Thread running"<<endl;
    if (serial!=NULL){
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this->parent(),SIGNAL(openSignal(QString)),this,SLOT(openSerialPort(QString)));
    connect(this->parent(),SIGNAL(closeSignal()),this,SLOT(closeSerialPort()));
    qDebug()<<"Reader Thread ready"<<endl;
    }
    else {
        emit ShowErrorSignal("Error al iniciar el puerto serie");
    }

    /*Debugging de los signals y slots
    emit DataBytesSignal(QByteArray::QByteArray("1023 1029\n\r"));
    emit DataBytesSignal(QByteArray::QByteArray("0 1029\n\r"));
    emit DataBytesSignal(QByteArray::QByteArray("1023 0\n\r"));
    emit DataBytesSignal(QByteArray::QByteArray("5 23\n\r"));
*/
    QFile file("DataSample_2_T100.txt");
    if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
    {
        QTextStream stream(&file);
        QString line = stream.readLine();
        while (line.length()>0){
              line.append("\n\r");
              QByteArray data;
              data.append(line);
              emit DataBytesSignal(data);
              line = stream.readLine();
              this->msleep(sleeptime);
        }
    }
    file.close();
}

void BTReaderThread::openSerialPort(QString portName)
{
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadOnly)) {
        emit OpenedSignal();
    } else {
        emit ShowErrorSignal(serial->errorString());
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
    emit DataBytesSignal(data);
}

void BTReaderThread::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort();
        emit ShowErrorSignal(serial->errorString());
    }
}
