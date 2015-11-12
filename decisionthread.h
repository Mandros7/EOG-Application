#ifndef DECISIONTHREAD_H
#define DECISIONTHREAD_H


#include <QtCore>

class DecisionThread : public QThread
{
    Q_OBJECT
public:
    explicit DecisionThread(QObject *parent = 0);
    void run();

private slots:
    void onChannelResults(QStringList results);

signals:
    void MovementSignal(QList<int>);
    void BlinkSignal(bool);
    void sendSamples(QList<QStringList> samples);
    void newDecision(QStringList sample);

private:
    void checkBlink();
    void clearBuffer();
    int units;
    int blinkCounter;
    int lowerBlink;
    int upperBlink;
    QList<QStringList> samples;
};
#endif // DECISIONTHREAD_H
