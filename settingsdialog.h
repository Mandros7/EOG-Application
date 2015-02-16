#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QtWidgets>

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

    void on_pushButton_2_clicked();

private:
    Ui::SettingsDialog *ui;
    void getMusicPath();
};

#endif // SETTINGSDIALOG_H
