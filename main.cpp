//#include "mainwindow.h"
#include "BluetoothReader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    BluetoothReader w;
    w.show();

    return a.exec();
}
