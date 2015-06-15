#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtTest/qtest.h>
#include "musicdialog.h"
#include "settingsdialog.h"
#include "inputdialog.h"
#include "bluetoothwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void musicSettingsChanged();
    void openSignal();
    void closeSignal();

private slots:
    void on_musicButton_clicked();
    void playerClosed();
    void on_settingsButton_clicked();
    void newMusicPath(QString newPath);
    void on_inputButton_clicked();
    void openBluetooth();
    void onClosedBTest();

    void newMovement(QList<int> coord);
    void newBlink(bool);
    void mouseMovementControl();

    void dataSentTimeStamp();
    void saveData();

    void openPort();
    void closePort();

private:
    Ui::MainWindow *ui;
    MusicDialog *mDialog;
    SettingsDialog *sDialog;
    InputDialog *iDialog;
    bool musicPlayerRunning;
    bool bluetoothTestRunning;
    QString musicPath;
    BluetoothWidget *bWidget;

    QList<int> processedTimeStamps;
    QList<int> receivedTimeStamps;
    QElapsedTimer timer;

    QCursor *cur;
    BTReaderThread *readerThread;
    DataParserThread *parserThread;
    DataTreatmentThread *treatmentThread;
    DecisionThread *decisionThread;
    QShortcut *moveMouse;
    int mouseState;
};

#endif // MAINWINDOW_H
