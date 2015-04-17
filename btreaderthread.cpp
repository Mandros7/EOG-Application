#include "btreaderthread.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>

BTReaderThread::BTReaderThread(QObject *parent) : QThread (parent)
{    
    serial = new QSerialPort(this);
    sleeptime = 33;
    openSerialPort();
}

void BTReaderThread::run()
{
    qDebug()<<"Reader Thread running"<<endl;
    if (serial!=NULL){
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(this->parent(),SIGNAL(openSignal()),this,SLOT(openSerialPort()));
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

void BTReaderThread::openSerialPort()
{
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    QSettings map(QString("configs/mapper.ini"), QSettings::IniFormat);

    serial->setPortName(settings.value("PortName").toString());
    qDebug()<<"Port Name: "<<settings.value("PortName").toString()<<endl;

    serial->setBaudRate(settings.value("BaudRate").toInt());
    qDebug()<<"Baud Rate: "<<settings.value("BaudRate").toString()<<endl;

    serial->setDataBits(QSerialPort::DataBits(
                            map.value(settings.value("DataBits").toString()).toInt()));
    qDebug()<<"Data Bits: "<<settings.value("DataBits").toString()<<endl;

    serial->setStopBits(QSerialPort::StopBits(
                            map.value(settings.value("StopBits").toString()).toInt()));
    qDebug()<<"Stop Bits : "<<settings.value("StopBits").toString()<<endl;

    serial->setParity(QSerialPort::Parity(
                          map.value(settings.value("Parity").toString()).toInt()));
    qDebug()<<"Parity : "<<settings.value("Parity").toString()<<endl;

    serial->setFlowControl(QSerialPort::FlowControl(
                               map.value(settings.value("FlowControl").toString()).toInt()));
    qDebug()<<"Flow Control : "<<settings.value("FlowControl").toString()<<endl;

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
