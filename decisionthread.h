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

private:
    void checkBlink();
    int units;
    int blinkCounter;
    int lowerBlink;
    int upperBlink;
};
#endif // DECISIONTHREAD_H
