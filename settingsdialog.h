#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtWidgets>

#include "btsettingsdialog.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

signals:
    void setMusicPath(QString dirPath);

private slots:

    void on_playerSettingsButton_clicked();

    void on_parametersButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::SettingsDialog *ui;
    void getMusicPath();
    BTSettingsDialog *btDialog;
};

#endif // SETTINGSDIALOG_H
