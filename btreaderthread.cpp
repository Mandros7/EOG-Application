#include "btreaderthread.h"
#include <QtSerialPort/QSerialPort>
#include <QDebug>

//------------------HILO DE LECTURA POR BLUETOOTH----------------------//
BTReaderThread::BTReaderThread(QObject *parent) : QThread (parent)
{    
    serial = new QSerialPort(this);
    sleeptime = 33; //Tiempo de simulación entre muestras
}

void BTReaderThread::run()
{
    qDebug()<<"Reader Thread running"<<endl;
    if (serial!=NULL){
        //Conexión a las notificaciones del objeto QSerialPort
        connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
                SLOT(handleError(QSerialPort::SerialPortError)));
        connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));

        //Recepcion de señal de apertura de puerto. Conexión sin uso necesario.
        connect(this->parent(),SIGNAL(openSignal()),this,SLOT(openSerialPort()));
        connect(this->parent(),SIGNAL(closeSignal()),this,SLOT(closeSerialPort()));
        //Señal de recepcion de muestra, para estudio de retardo
        //connect(this,SIGNAL(SampleReadSignal()),this->parent(),SLOT(dataSentTimeStamp()));
        //connect(this,SIGNAL(FinishSignal()),this->parent(),SLOT(saveData()));
    }
    else {
        emit ShowErrorSignal("Error al iniciar el puerto serie");
    }

    /*Sistema de simulación de recepción de datos.
     * Los datos se leen de un fichero como si se recibiesen y
     * se introduce un retardo artificial que simula el que
     * existe entre muestras.
    */
    QFile file("DataSample_2_T100.txt");
    if ( file.open(QIODevice::ReadOnly|QIODevice::Text) )
    {
        QTextStream stream(&file);
        QString line = stream.readLine();
        while (line.length()>0){
        //for (int i=0; i<100 ; i++){
              line.append("\n\r");
              QByteArray data;
              data.append(line);
              emit DataBytesSignal(data);
              line = stream.readLine();
              //emit SampleReadSignal();
              this->msleep(sleeptime);
        }
    }
    file.close();
}

void BTReaderThread::openSerialPort()
{
    //Lectura de configuración y traducción de los parámetros para poder pasarlos
    //al objeto QSerialPort.
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
    emit DataBytesSignal(data); //Señal que envia los datos en bytes
}

void BTReaderThread::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        closeSerialPort();
        emit ShowErrorSignal(serial->errorString());
    }
}
