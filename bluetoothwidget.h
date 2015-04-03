#ifndef BLUETOOTHWIDGET_H
#define BLUETOOTHWIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QTimer>
#include <QShortcut>
#include "btreaderthread.h"
#include "dataparserthread.h"
#include "datatreatmentthread.h"
#include "decisionthread.h"

namespace Ui {
class BluetoothWidget;
}

class BluetoothWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BluetoothWidget(QWidget *parent = 0);
    ~BluetoothWidget();

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
    void mouseMovementControl();

signals:
    void openSignal(QString portName);
    void closeSignal();

private:
    Ui::BluetoothWidget *ui;
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
    QShortcut *moveMouse;
    int mouseState;
};

#endif // BLUETOOTHWIDGET_H
