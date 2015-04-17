#include "btsettingsdialog.h"
#include "ui_btsettingsdialog.h"

#include <QSettings>

/* Mediante el uso de menus desplegables, es posible modificar los parámetros
 * de la conexión Bluetooth. Estos parametros vienen vacios por defecto, o con
 * los valores que se lean en el archivo de configuración.
 *
 * Una vez modificados, al aceptar los cambios, el archivo de configuración se
 * actualiza y, tras reiniciarse la aplicación, el hilo de lectura de datos
 * utilizará los nuevos parametros para conectarse al dispositivo.
 * */
BTSettingsDialog::BTSettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BTSettingsDialog)
{
    ui->setupUi(this);
    //Enumeración de puertos disponibles
    infoPorts = QSerialPortInfo::availablePorts();
    for (int i = 0; i<infoPorts.size(); i++){
            ui->nameComboBox->addItem(infoPorts.at(i).portName());
    }

    /* -------------------- Configuración de menús desplegables ----------------------
     *
     * Los menús se llenan con los valores disponibles (salvo el que indica cantidad de
     * baudios, que se actualiza al modificar el nombre del puerto). A continuación,
     * se selecciona el elemento que actualmente se encuentra configurado. Si el
     * resultado de esta operación es que el parámetro no está configurado, el menú
     * desplegable quedará sin ningún parámetro seleccionado.
     *
     * ------------------------------------------------------------------------------ */
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);

    QStringList dataBits;
    dataBits << "5" << "6" << "7" << "8" << "Desconocido";
    ui->dataBitsComboBox->addItems(dataBits);
    ui->dataBitsComboBox->setCurrentIndex(
                ui->dataBitsComboBox->findText(settings.value("DataBits").toString()));

    QStringList flowControl;
    flowControl << "No" << "Hardware" << "Software" << "Desconocido";
    ui->flowComboBox->addItems(flowControl);
    ui->flowComboBox->setCurrentIndex(
                ui->flowComboBox->findText(settings.value("FlowControl").toString()));

    QStringList parity;
    parity << "No" << "Even Parity" << "Odd Parity" << "Space Parity" << "Mark Parity" << "Desconocido";
    ui->parityComboBox->addItems(parity);
    ui->parityComboBox->setCurrentIndex(
                ui->parityComboBox->findText(settings.value("Parity").toString()));

    QStringList stopBits;
    stopBits << "1" << "2" << "1,5" << "Desconocido";
    ui->stopBitsComboBox->addItems(stopBits);
    ui->stopBitsComboBox->setCurrentIndex(
                ui->stopBitsComboBox->findText(settings.value("StopBits").toString()));


}

BTSettingsDialog::~BTSettingsDialog()
{
    delete ui;
}

void BTSettingsDialog::on_nameComboBox_currentIndexChanged(int index)
{
    //Al cambiar el nombre de puerto, se generan las velocidades disponibles en baudios.
    QList<qint32> baudRates = infoPorts.at(index).standardBaudRates();
    for (int i = 0; i<baudRates.size(); i++){
            ui->baudComboBox->addItem(QString::number(baudRates.at(i)));
    }
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);

    int indexBR = ui->baudComboBox->findText(settings.value("BaudRate").toString());
    ui->baudComboBox->setCurrentIndex(indexBR);
}

void BTSettingsDialog::on_buttonBox_accepted()
{
    //Una vez se acepta, los parámetros se guardan en el archivo de configuración
    QSettings settings(QString("configs/config.ini"), QSettings::IniFormat);

    settings.setValue("PortName",ui->nameComboBox->currentText());

    settings.setValue("BaudRate",ui->baudComboBox->currentText());

    settings.setValue("DataBits",ui->dataBitsComboBox->currentText());

    settings.setValue("FlowControl",ui->flowComboBox->currentText());

    settings.setValue("Parity",ui->parityComboBox->currentText());

    settings.setValue("StopBits",ui->stopBitsComboBox->currentText());

}
