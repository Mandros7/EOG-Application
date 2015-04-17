#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(setMusicPath(QString)),this->parent(),SLOT(newMusicPath(QString)));
    connect(ui->bluetoothButton,SIGNAL(clicked()),this->parent(),SLOT(openBluetooth()));
}

void SettingsDialog::getMusicPath(){
    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Open Directory"),musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first());
    qDebug() << "entry is" << dirPath;
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);
    settings.setValue("MusicPath",dirPath);
    emit setMusicPath(dirPath);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_playerSettingsButton_clicked()
{
    getMusicPath();
}

void SettingsDialog::on_closeButton_clicked()
{
     this->~SettingsDialog();
}

void SettingsDialog::on_parametersButton_clicked()
{
    btDialog = new BTSettingsDialog();
    btDialog->setModal(true);
    btDialog->show();
}


