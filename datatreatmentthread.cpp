#include "datatreatmentthread.h"

DataTreatmentThread::DataTreatmentThread(QObject *parent) : QThread (parent)
{
    upperThreshold = 800;
    lowerThreshold = 300;
}

void DataTreatmentThread::run(){
    qDebug()<<"Treatment Thread running"<<endl;
}

void DataTreatmentThread::onChannelsData(QStringList channels){
    int vertical = channels[1].toInt();
    int horizontal = channels[0].toInt();
    QString VResult="-";
    QString HResult="-";
    if (vertical>upperThreshold) {
        VResult = "DOWN";
    }
    if (vertical<lowerThreshold) {
        VResult = "UP";
    }
    if (horizontal>upperThreshold){
        HResult = "LEFT";
    }
    if (horizontal<lowerThreshold){
        HResult = "RIGHT";
    }
    QStringList results;
    results << HResult << VResult;
    emit ShowResultsSignal(results);
}

