#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include "btreaderthread.h"
#include "dataparserthread.h"
#include "datatreatmentthread.h"
#include "decisionthread.h"

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
    void openedSerialPort();
    void closedSerialPort();
    void openPort();
    void closePort();
    void newData(QString data);
    void newResults(QStringList results);
    void newMovement(QList<int> coord);
    void newError(QString info);
    void update();

signals:
    void openSignal(QString portName);
    void closeSignal();

private:
    Ui::BluetoothReader *ui;
    QSerialPort *serial;
    QTimer *timer;
    qint32 counter;
    QString dataString;
    QStringList finalDataList;
    QCursor *cur;
    BTReaderThread *readerThread;
    DataParserThread *parserThread;
    DataTreatmentThread *treatmentThread;
    DecisionThread *decisionThread;
};

#endif // MAINWINDOW_H
