#include "decisionthread.h"

//------ HILO DE PREPARACION DE DATOS ----//
DecisionThread::DecisionThread(QObject *parent) : QThread (parent)
{ 
    blinkCounter = 0;
    units = 4; //Declaracion de tamaño de salto de cursor
    lowerBlink = 10;
    upperBlink = 20;

    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    if (settings.value("Speed").toString().toInt()>0){
        units = settings.value("Speed").toString().toInt();
    }
    if (settings.value("Minblink").toString().toInt()>0){
        lowerBlink = settings.value("Minblink").toString().toInt();
    }
    if (settings.value("Maxblink").toString().toInt()>0){
        upperBlink = settings.value("Maxblink").toString().toInt();
    }

}

void DecisionThread::run(){
    qDebug()<<"Decision Thread running"<<endl;
}

//En funcion de los resultados del tratamiento, se generan unas coordenadas relativas para pasar al cursor
// Estas coordenadas se envian a la interfaz (MainWindow) emitiendo una señal que las contiene.
void DecisionThread::onChannelResults(QStringList results){
    samples.push_back(results);
    if (samples.length()>=30){
        checkBlink();
        QStringList sample = samples[0];
        emit newDecision(sample);
        samples.pop_front();
        int hMov=0;
        int vMov=0;
        //qDebug()<<"Counter: "<<blinkCounter;
        if(sample[1]=="UP"){
            vMov = -units;
        }
        if(sample[1]=="DOWN"){
            vMov = units;
        }
        if(sample[0]=="RIGHT"){
            hMov = units;
        }
        if(sample[0]=="LEFT"){
            hMov = -units;
        }
        QList<int> coord;
        coord << hMov << vMov;
        emit MovementSignal(coord);
        //qDebug()<<"SIGNAL >>> Moviemiento: horizontal "<<coord[0]<<" vertical "<<coord[1]<<endl;

    }
}
void DecisionThread::checkBlink(){
    emit sendSamples(samples);
    for (int i=0;i<samples.length();i++){
        if ((samples[i])[1]=="UP"){
            blinkCounter++;
            emit BlinkSignal(false);
        }
        else {
            if ((blinkCounter > lowerBlink)&&(blinkCounter < upperBlink)&&(i>lowerBlink)&&(i<25)){
                clearBuffer();
                emit BlinkSignal(true);
            }
            else {
                emit BlinkSignal(false);
            }
            blinkCounter = 0;
        }
    }
    blinkCounter = 0;
}

void DecisionThread::clearBuffer(){
    for (int i=0; i<samples.length();i++){
        (samples[i])[1] = "-";
    }
    emit sendSamples(samples);
}
