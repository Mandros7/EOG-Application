#ifndef MUSICDIALOG_H
#define MUSICDIALOG_H

#include <QDialog>
#include <QtMultimedia>
#include <QtWidgets>

namespace Ui {
class MusicDialog;
}

class MusicDialog : public QDialog
{
    Q_OBJECT


public:
    explicit MusicDialog(QWidget *parent = 0);
    void openDir();
    void setFile(const QString& filePath);
    ~MusicDialog();

private slots:
    void on_moreVolumeButton_pressed();

    void on_lessVolumeButton_pressed();

    void on_playPauseButton_clicked();

    void on_goForwardButton_clicked();

    void on_goBackButton_clicked();

    void updateSongInfo();

    void updateTimeStamp(qint64 position);

    void updateProgressBar(qint64 position);

    void updatePlayPauseButton(QMediaPlayer::State state);

private:
    void createShortcuts();
    void setIcons();
    Ui::MusicDialog *ui;
    QMediaPlayer mediaPlayer;
    QStringList filesPath;
    int currentFile;
};

#endif // MUSICDIALOG_H
