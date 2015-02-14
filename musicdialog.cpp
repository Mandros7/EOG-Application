#include "musicdialog.h"
#include "ui_musicdialog.h"

MusicDialog::MusicDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicDialog)
{
    createShortcuts();
    ui->setupUi(this);
    connect(ui->volumeBar, SIGNAL(valueChanged(int)), &mediaPlayer, SLOT(setVolume(int)));
    ui->volumeBar->setValue(20);
    connect(&mediaPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateSongInfo()));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateTimeStamp(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(updateProgressBar(qint64)));
    ui->timeBar->setValue(0);
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(updatePlayPauseButton(QMediaPlayer::State)));
    connect(ui->quitButton, SIGNAL(clicked()),parent,SLOT(playerClosed()));
    setIcons();
    openDir();
}

MusicDialog::~MusicDialog()
{
    delete ui;
}
void MusicDialog::setIcons(){
     ui->goBackButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
     ui->goForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
     ui->playPauseButton->setText("Reproducir");
     ui->playPauseButton->setIcon(style()->standardIcon((QStyle::SP_MediaPlay)));
     ui->moreVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
     ui->lessVolumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
}

void MusicDialog::openDir()
{
    const QStringList musicPaths = QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

    const QString dirPath =
                QFileDialog::getExistingDirectory(this,
                    tr("Open Directory"),musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first());
        QStringList nameFilter("*.mp3");
        QDir directory(dirPath);
        QStringList mp3Files = directory.entryList(nameFilter);
        for (int i = 0; i<mp3Files.size(); i++){
            filesPath.append(directory.filePath(mp3Files.at(i)));
        }
        if (!mp3Files.empty()){
            currentFile = 0;
        }
}

void MusicDialog::setFile(const QString &filePath)
{
    //playButton->setEnabled(true);
    mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
}

void MusicDialog::createShortcuts()
{

    QShortcut *toggleShortcut = new QShortcut(Qt::Key_Space, this);
    connect(toggleShortcut, SIGNAL(activated()), this, SLOT(on_playPauseButton_clicked()));

    QShortcut *nextSongShortcut = new QShortcut(Qt::Key_Right, this);
    connect(nextSongShortcut, SIGNAL(activated()), this, SLOT(on_goForwardButton_clicked()));

    QShortcut *previousSongShortcut = new QShortcut(Qt::Key_Left, this);
    connect(previousSongShortcut, SIGNAL(activated()), this, SLOT(on_goBackButton_clicked()));

    QShortcut *increaseShortcut = new QShortcut(Qt::Key_Up, this);
    connect(increaseShortcut, SIGNAL(activated()), this, SLOT(on_moreVolumeButton_pressed()));

    QShortcut *decreaseShortcut = new QShortcut(Qt::Key_Down, this);
    connect(decreaseShortcut, SIGNAL(activated()), this, SLOT(on_lessVolumeButton_pressed()));
}


void MusicDialog::updatePlayPauseButton(QMediaPlayer::State state){
   if (state == QMediaPlayer::PlayingState) {
            ui->playPauseButton->setText("Pausar");
            ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        } else {
            ui->playPauseButton->setText("Reproducir");
            ui->playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        }

}

void MusicDialog::updateTimeStamp(qint64 position)
{
    ui->timeBar->setValue(position);

    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    ui->timeLabel->setText(duration.toString(tr("mm:ss")));
}

void MusicDialog::updateProgressBar(qint64 duration)
{
    ui->timeBar->setRange(0, duration);
    ui->timeBar->setEnabled(duration > 0);
    QTime totalDuration(0,mediaPlayer.duration()/60000, qRound((mediaPlayer.duration()%60000)/1000.0),0);
    ui->totalTimeLabel->setText(totalDuration.toString(tr("/ mm:ss")));
}

void MusicDialog::updateSongInfo()
{
    QStringList info;
    QString author = mediaPlayer.metaData("Author").toString();
    if (!author.isEmpty())
        info += author;
    QString title = mediaPlayer.metaData("Title").toString();
    if (!title.isEmpty())
        info += title;
    if (!info.isEmpty())
        ui->songLabel->setText(info.join(tr(" - ")));
}

void MusicDialog::on_playPauseButton_clicked()
{
    if (mediaPlayer.mediaStatus() == QMediaPlayer::NoMedia){
        if (!filesPath.empty()){
        setFile(filesPath.at(currentFile));
        }
         mediaPlayer.play();
       }
    else if (mediaPlayer.state() == QMediaPlayer::PlayingState)
        mediaPlayer.pause();
    else
        mediaPlayer.play();
}

void MusicDialog::on_goForwardButton_clicked()
{
     if (!filesPath.empty()){
         currentFile++;
         if (currentFile>=filesPath.size()){
             currentFile = 0;
         }
         setFile(filesPath.at(currentFile));
         on_playPauseButton_clicked();
     }
}

void MusicDialog::on_goBackButton_clicked()
{
    if (!filesPath.empty()){

        currentFile--;
        if (currentFile<0){
            currentFile = filesPath.size()-1;
        }
        setFile(filesPath.at(currentFile));
        on_playPauseButton_clicked();
    }
}
void MusicDialog::on_moreVolumeButton_pressed()
{
    ui->volumeBar->setValue(ui->volumeBar->value()+5);
}

void MusicDialog::on_lessVolumeButton_pressed()
{
    ui->volumeBar->setValue(ui->volumeBar->value()-5);
}

void MusicDialog::on_quitButton_clicked()
{
    emit dialogClosed();
    this->~MusicDialog();
}
