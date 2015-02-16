#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    musicPlayerRunning = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newMusicPath(QString newPath){
    //qDebug() << Q_FUNC_INFO;
    qDebug() << "recieved" << newPath;
    musicPath = newPath;
    emit musicSettingsChanged();
}

void MainWindow::on_musicButton_clicked()
{
    if (musicPlayerRunning){
        mDialog->show();
    }
    else{
        mDialog = new MusicDialog(musicPath,this);
        musicPlayerRunning = true;
        mDialog->show();
        connect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
    }
}

void MainWindow::playerClosed(){
    qDebug() << "closed";
    musicPlayerRunning = false;
    disconnect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
}


void MainWindow::on_settingsButton_clicked()
{
        sDialog = new SettingsDialog(this);
        settingsRunning = true;
        sDialog->show();
}

