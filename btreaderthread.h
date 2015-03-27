#ifndef BTREADERTHREAD_H
#define BTREADERTHREAD_H

#include <QtCore>
#include <QtSerialPort/QSerialPort>

class BTReaderThread : public QThread
{
    Q_OBJECT
public:
    explicit BTReaderThread(QObject *parent = 0); //Add Buffer to constructor
    void run();

private slots:
    void readData();
    void openSerialPort(QString portName);
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void ClosedSignal();
    void OpenedSignal();
    void ErrorSignal(QString);

    void DataSignal(QByteArray);

private:
    QSerialPort *serial;
    //Add Buffer
};

#endif // BTREADERTHREAD_H
