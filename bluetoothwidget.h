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
    void newDecision(QStringList results);
    void printSamples(QList<QStringList> samples);
    void newError(QString info);
    void newBlink(bool performed);
    void update();

    void on_shortcut();

    void on_updatePushButton_clicked();

signals:
    void widgetClosedSignal();
    void clickedStart();
    void clickedStop();
    void shortcutChange();

private:
    Ui::BluetoothWidget *ui;
    QTimer *timer;
    qint32 counter;
    QString dataString;
    QStringList finalDataList;
    QShortcut *moveMouse;
};

#endif // BLUETOOTHWIDGET_H
