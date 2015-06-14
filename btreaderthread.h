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
    void handleError(QSerialPort::SerialPortError error);
    void openSerialPort();
    void closeSerialPort();

signals:
    void ClosedSignal();
    void OpenedSignal();
    void DataBytesSignal(QByteArray);
    void ShowErrorSignal(QString);

    void SampleReadSignal();
    void FinishSignal();

private:
    QSerialPort *serial;
    int sleeptime;
    QStringList list;
};

#endif // BTREADERTHREAD_H
