#ifndef BTSETTINGSDIALOG_H
#define BTSETTINGSDIALOG_H

#include <QDialog>

#include <QSerialPortInfo>

namespace Ui {
class BTSettingsDialog;
}

class BTSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BTSettingsDialog(QWidget *parent = 0);
    ~BTSettingsDialog();

private slots:
    void on_nameComboBox_currentIndexChanged(int index);

    void on_buttonBox_accepted();

private:
    Ui::BTSettingsDialog *ui;
    QList<QSerialPortInfo> infoPorts;
};

#endif // BTSETTINGSDIALOG_H
