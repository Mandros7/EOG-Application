#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "musicdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_musicButton_clicked();
    void playerClosed();

private:
    Ui::MainWindow *ui;
    MusicDialog *mDialog;
    bool musicPlayerRunning;
};

#endif // MAINWINDOW_H
