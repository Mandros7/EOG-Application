#include "datatreatmentthread.h"

//------ HILO DE TRATAMIENTO DE DATOS ----//
DataTreatmentThread::DataTreatmentThread(QObject *parent) : QThread (parent)
{
    upperThreshold = 1000; //Declaracion de umbrales
    lowerThreshold = 100;
}

void DataTreatmentThread::run(){
    qDebug()<<"Treatment Thread running"<<endl;
}


//Tratamiento simple mediante dos umbrales por canal. El resultado se envía al hilo de decisión
void DataTreatmentThread::onChannelsData(QStringList channels){
    /*CODIGO DE SEPARACION DE CANALES EN DOS ARCHIVOS
    QFile file_H("DataSample_2_horizontal.txt");
    if ( file_H.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append ) )
    {
        QTextStream stream_H(&file_H);
        stream_H<<channels[0]<<"\t";
    }
    QFile file_V("DataSample_2_vertical.txt");
    if ( file_V.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append ) )
    {
        QTextStream stream_V(&file_V);
        stream_V<<channels[1]<<"\t";
    }
    file_H.close();
    file_V.close();
    */
    int vertical = channels[1].toDouble();
    int horizontal = channels[0].toDouble();
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

