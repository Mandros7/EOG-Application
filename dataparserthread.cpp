#include "dataparserthread.h"

//------ HILO DE PREPARACION DE DATOS ----//
DataParserThread::DataParserThread(QObject *parent) : QThread (parent)
{
}

void DataParserThread::run(){
    qDebug()<<"Parser Thread running"<<endl;
    //connect(this,SIGNAL(SampleReadSignal()),this->parent(),SLOT(dataSentTimeStamp()));
}

//Los bytes se almacenan en un buffer, en el que se espera un salto de linea, se extraen los datos
// y se envía la nueva línea a la ventana de etado, y ambos canales a la ventana de tratamiento.
void DataParserThread::onDataBytes(QByteArray data){
    queue+=data;
    int index = queue.indexOf('\n\r', 0);
    if (index >= 0){
        QString line = QString(queue.mid(0, index)); //Quitar \n\r
        queue.remove(0, index + 1);
        emit ShowDataSignal(line);
        //emit SampleReadSignal();
        QStringList channels = line.split(" ");
        emit ChannelsDataSignal(channels);
    }

}


