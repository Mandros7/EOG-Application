#include "musicdialog.h"
#include "ui_musicdialog.h"

MusicDialog::MusicDialog(QString newPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MusicDialog)
{
    createShortcuts(); //Atajos, para una posible implementación de un clasificador de movimientos
    ui->setupUi(this);

    connect(ui->volumeBar, SIGNAL(valueChanged(int)), &mediaPlayer, SLOT(setVolume(int)));
    ui->volumeBar->setValue(20); //Volumen inicial al 20%

    // Conexión de señales y slots para la información del reproductor
    connect(&mediaPlayer, SIGNAL(metaDataAvailableChanged(bool)), this, SLOT(updateSongInfo()));
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(updateTimeStamp(qint64)));
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(updateProgressBar(qint64)));
    ui->timeBar->setValue(0);
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),this, SLOT(updatePlayPauseButton(QMediaPlayer::State)));

    // Envio de señal de dialogo cerrado (no en segundo plano)
    connect(this,SIGNAL(dialogClosed()),this->parent(),SLOT(playerClosed()));

    setIcons();
    pathToMusic = newPath;
    openDir();
}

MusicDialog::~MusicDialog()
{
    delete ui;
}

/* ------------------------------ MÉTODOS PROPIOS ---------------------------------------
 * ----------------------------------------------------------------------------------------*/

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
        qDebug() << "value is" << pathToMusic;
        QStringList nameFilter("*.mp3");
        QDir directory(pathToMusic);
        QStringList mp3Files = directory.entryList(nameFilter);
        QStringList modelList;
        for (int i = 0; i<mp3Files.size(); i++){
            filesPath.append(directory.filePath(mp3Files.at(i)));
            modelList << mp3Files.at(i).split(".").at(0);
        }
        if (!mp3Files.empty()){
            currentFile = 0;
        }
        else {
            modelList << "NO se detectaron archivos mp3";
            disableButtons();
            //modelList << "Configure el directorio de musica";
        }
        model = new QStringListModel();

        model->setStringList(modelList);
        ui->songList->setModel(model);
        QModelIndex index = model->index(0,currentFile);
        ui->songList->setCurrentIndex(index);
}

void MusicDialog::setFile(const QString &filePath)
{
    mediaPlayer.setMedia(QUrl::fromLocalFile(filePath));
}

void MusicDialog::createShortcuts()
{
    //Los atajos son punteros cuya memoria asignada se libera automaticamente al liberar la memoria de
    //la ventana del reproductor (variable UI).
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

void MusicDialog::disableButtons(){
    ui->playPauseButton->setEnabled(false);
    ui->goBackButton->setEnabled(false);
    ui->goForwardButton->setEnabled(false);
}

/* ------------------------------ SLOTS --------------------------------------------------
 * ----------------------------------------------------------------------------------------*/

void MusicDialog::updatePlayPauseButton(QMediaPlayer::State state){
    //En funcion del estado (reproduciendo o no) se muestra un boton u otro.
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
    //Se reciben los minutos y segundos de canción que ya se han reproducido, se muestran en pantalla.
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
    //Se accede a los metadatos de los archivos mp3 y se muestra información
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

/* ------------------------------ REGISTRO DE ACCIONES (QButton) ---------------------------
 * Los botones de pausa/play afectan al estado del reproductor
 * El resto de botones modifican el indice seleccionado dentro de la lista de canciones
 * Los botones de volumen modifican la barra de volumen, que provoca la ejecucion del slot correspondiente
 * ----------------------------------------------------------------------------------------*/

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
     QModelIndex index = model->index(currentFile,0);
     ui->songList->setCurrentIndex(index);
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
    QModelIndex index = model->index(currentFile,0);
    ui->songList->setCurrentIndex(index);
}

void MusicDialog::on_moreVolumeButton_pressed()
{
    ui->volumeBar->setValue(ui->volumeBar->value()+5);
}

void MusicDialog::on_lessVolumeButton_pressed()
{
    ui->volumeBar->setValue(ui->volumeBar->value()-5);
}


//Botones que permiten cerrar o poner en segundo plano el reproductor.
void MusicDialog::on_quitButton_clicked()
{
    emit dialogClosed();
    this->~MusicDialog();
}

void MusicDialog::on_hideButton_clicked()
{
    this->hide();
}
