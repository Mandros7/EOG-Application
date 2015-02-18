#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "musicdialog.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    musicPlayerRunning = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*-------------------------------------- SLOTS --------------------------------------------//
 *
 *       La ventana de menú principal recibe la siguiente informacion:
 *           - Reproductor cerrado. Es necesario registrar esta señal para
 *           saber cuando crear un nuevo dialogo y asi evitar crear dos dialogos concurrentes
 *           - Nuevo directorio de música. Cuando la ventana de configuración
 *           envíe una señal de cambio de directorio, la ventana principal (menú) actuará
 *           como intermediario, emitiendo una señal que cierra el reproductor.
 *           La proxima vez que se abra utilizará el nuevo directorio (musicPath).
 */
void MainWindow::playerClosed(){
    qDebug() << "closed";
    musicPlayerRunning = false;
    disconnect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
}

void MainWindow::newMusicPath(QString newPath){
    //qDebug() << Q_FUNC_INFO;
    qDebug() << "recieved" << newPath;
    musicPath = newPath;
    emit musicSettingsChanged();
}

/* ------------------------ REGISTRO DE ACCIONES (sobre Qbuttons) ----------------------------
 *              Se lanzan los dialogos del reproductor de musica y de configuración
 *              Se conecta la señal de cambio de configuración a la función de cierre
 *                  del reproductor.
 */
void MainWindow::on_musicButton_clicked()
{
    if (musicPlayerRunning){
        mDialog->show();
    }
    else{
        mDialog = new MusicDialog(musicPath,this);
        musicPlayerRunning = true;
        mDialog->show();
        connect(this,SIGNAL(musicSettingsChanged()),mDialog,SLOT(on_quitButton_clicked()));
    }
}

void MainWindow::on_settingsButton_clicked()
{
        sDialog = new SettingsDialog(this);
        //settingsRunning = true;
        sDialog->show();
}

