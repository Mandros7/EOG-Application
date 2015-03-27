#include "dataparserthread.h"

DataParserThread::DataParserThread(QObject *parent) : QThread (parent)
{
}

void DataParserThread::run(){
    qDebug()<<"Parser Thread running"<<endl;
}

void DataParserThread::onDataBytes(QByteArray data){
    queue+=data;
    int index = queue.indexOf('\n\r', 0);
    if (index >= 0){
        QString line = QString(queue.mid(0, index)); //Quitar \n\r
        queue.remove(0, index + 1);
        emit DataSignal(line);
        QStringList channels = line.split(" ");
    }

}


