#include "datatreatmentthread.h"

DataTreatmentThread::DataTreatmentThread(QObject *parent) : QThread (parent)
{
}

void DataTreatmentThread::run(){
    qDebug()<<"Treatment Thread running"<<endl;
}

void DataTreatmentThread::onChannelsData(QStringList channels){
    int vertical = channels[1].toInt();
    int horizontal = channels[0].toInt();
    QString VResult="-";
    QString HResult="-";
    if (vertical>900) {
        VResult = "DOWN";
    }
    if (vertical<200) {
        VResult = "UP";
    }
    if (horizontal>900){
        HResult = "LEFT";
    }
    if (horizontal<200){
        HResult = "RIGHT";
    }
    QStringList results;
    results << HResult << VResult;
    emit ShowResultsSignal(results);
}

