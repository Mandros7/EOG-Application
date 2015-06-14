#include "decisionthread.h"

//------ HILO DE PREPARACION DE DATOS ----//
DecisionThread::DecisionThread(QObject *parent) : QThread (parent)
{
    units = 4; //Declaracion de tamaño de salto de cursor
    lowerBlink = 10;
    upperBlink = 35;
}

void DecisionThread::run(){
    qDebug()<<"Decision Thread running"<<endl;
}

//En funcion de los resultados del tratamiento, se generan unas coordenadas relativas para pasar al cursor
// Estas coordenadas se envian a la interfaz (MainWindow) emitiendo una señal que las contiene.
void DecisionThread::onChannelResults(QStringList results){
    int hMov=0;
    int vMov=0;
    //qDebug()<<"Counter: "<<blinkCounter;
    if(results[1]=="UP"){
        blinkCounter++;
        vMov = -units;
    }
    else if(results[1]=="DOWN"){
        vMov = units;
        checkBlink();
    }
    else{
        checkBlink();
    }
    if(results[0]=="RIGHT"){
        hMov = units;
    }
    if(results[0]=="LEFT"){
        hMov = -units;
    }
    QList<int> coord;
    coord << hMov << vMov;
    emit MovementSignal(coord);
    //qDebug()<<"SIGNAL >>> Moviemiento: horizontal "<<coord[0]<<" vertical "<<coord[1]<<endl;
}

void DecisionThread::checkBlink(){
    if ((blinkCounter > lowerBlink)&&(blinkCounter < upperBlink)){
        emit BlinkSignal(true);
    }
    else {
        emit BlinkSignal(false);
    }
    blinkCounter = 0;
}

