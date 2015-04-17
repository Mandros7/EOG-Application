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
    void openSerialPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void ClosedSignal();
    void OpenedSignal();
    void DataBytesSignal(QByteArray);
    void ShowErrorSignal(QString);

private:
    QSerialPort *serial;
    int sleeptime;
};

#endif // BTREADERTHREAD_H
