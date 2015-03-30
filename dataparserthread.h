#ifndef DATAPARSERTHREAD_H
#define DATAPARSERTHREAD_H

#include <QtCore>

class DataParserThread : public QThread
{
    Q_OBJECT
public:
    explicit DataParserThread(QObject *parent = 0);
    void run();

signals:
    void ShowDataSignal(QString);
    void ChannelsDataSignal(QStringList);

private slots:
    void onDataBytes(QByteArray data);

private:
    QByteArray queue;
};

#endif // DATAPARSERTHREAD_H
