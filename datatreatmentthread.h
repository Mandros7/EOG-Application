#ifndef DATATREATMENTTHREAD_H
#define DATATREATMENTTHREAD_H
#include <QtCore>

class DataTreatmentThread : public QThread
{
    Q_OBJECT
public:
    explicit DataTreatmentThread(QObject *parent = 0);
    void run();

private slots:
    void onChannelsData(QStringList channels);

signals:
    void ShowResultsSignal(QStringList);

private:
    int lowerThreshold;
    int upperThreshold;
};

#endif // DATATREATMENTTHREAD_H
