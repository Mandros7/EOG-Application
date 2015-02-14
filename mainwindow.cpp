#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"

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

void MainWindow::on_musicButton_clicked()
{
    if (musicPlayerRunning){
        mDialog->show();
    }
    else{
        mDialog = new MusicDialog(this);
        musicPlayerRunning = true;
        connect(mDialog,SIGNAL(dialogClosed()),this,SLOT(playerClosed()));
        mDialog->show();
    }
}

void MainWindow::playerClosed(){
    musicPlayerRunning = false;
}

