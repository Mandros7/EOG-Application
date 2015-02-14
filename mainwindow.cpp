#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_musicButton_clicked()
{
    MusicDialog mDialog;
    mDialog.setModal(true);
    mDialog.exec();
}

