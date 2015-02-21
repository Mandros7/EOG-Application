#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    connect(this,SIGNAL(setMusicPath(QString)),this->parent(),SLOT(newMusicPath(QString)));

}

void SettingsDialog::getMusicPath(){
    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Open Directory"),musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first());
    qDebug() << "entry is" << dirPath;
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

void SettingsDialog::on_pushButton_2_clicked()
{
    this->~SettingsDialog();
}
