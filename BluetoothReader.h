#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

namespace Ui {
class BluetoothReader;
}

class BluetoothReader : public QMainWindow
{
    Q_OBJECT

public:
    explicit BluetoothReader(QWidget *parent = 0);
    ~BluetoothReader();

private slots:
    void openSerialPort();
    void closeSerialPort();
    void readData();
    void update();

    void handleError(QSerialPort::SerialPortError error);

private:
    Ui::BluetoothReader *ui;
    QSerialPort *serial;
    QTimer *timer;
    qint32 counter;
    QString dataString;
    QStringList finalDataList;

};

#endif // MAINWINDOW_H
