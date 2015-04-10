#include "decisionthread.h"

DecisionThread::DecisionThread(QObject *parent) : QThread (parent)
{
    units = 4;
}

void DecisionThread::run(){
    qDebug()<<"Decision Thread running"<<endl;
}

void DecisionThread::onChannelResults(QStringList results){
    int hMov=0;
    int vMov=0;

    if(results[1]=="UP"){
        vMov = -units;
    }
    if(results[1]=="DOWN"){
        vMov = units;
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

