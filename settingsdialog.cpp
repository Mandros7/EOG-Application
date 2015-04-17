#include "settingsdialog.h"
#include "ui_settingsdialog.h"

/*Esta ventana no está destinada a ser manejada mediante señales EOG, a pesar de su simplicidad.
 * El uso de archivos de configuración, pretende eliminar la necesidad de configurar la aplicacion
 * cada vez que se vaya a utilizar.
  */
SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    //Se conectan las señales de cambio de directorio de musica y de apertura de widget bluetooth.
    connect(this,SIGNAL(setMusicPath(QString)),this->parent(),SLOT(newMusicPath(QString)));
    connect(ui->bluetoothButton,SIGNAL(clicked()),this->parent(),SLOT(openBluetooth()));
}

void SettingsDialog::getMusicPath(){
    //Se obtiene el directorio mediante un dialogo de sistema que permite seleccionar directorio
    const QStringList musicPaths =
            QStandardPaths::standardLocations(QStandardPaths::MusicLocation); //Directorios comunes
    QString dirPath = QFileDialog::getExistingDirectory(this,tr("Open Directory"),musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first());

    //Guardar nuevo directorio en configuración
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
    //Lanza dialogo de configuración Bluetooth.
    btDialog = new BTSettingsDialog();
    btDialog->setModal(true);
    btDialog->show();
}


