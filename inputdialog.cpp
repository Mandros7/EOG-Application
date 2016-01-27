#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QDesktopServices>
#include <smtp.h>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    mayus = true;
    state = 0;

    setKeyboard();
    setIndex();
}

InputDialog::~InputDialog()
{
    delete ui;
}
void InputDialog::setIndex(){
    listIndex = QStringList() << "-1" << "-1" << "-1" << "-1" << "-1" << "-1" << "-1" << "-1";
}

void InputDialog::setKeyboard(){
    if (mayus){
        listOne = QStringList() << "A" << "B" << "C" << "D";
        listTwo = QStringList() << "I" << "J" << "K" << "L";
        listThree = QStringList() << "P" << "Q" << "R" << "S";
        listFour = QStringList() << "X" << "Y" << "Z" << "[ ]";
        listFive = QStringList() << "E" << "F" << "G" << "H";
        listSix = QStringList() << "M" << "N" << "Ñ" << "O";
        listSeven = QStringList() << "T" << "U" << "V" << "W";
        listEight = QStringList() << "." << "," << "?" << "!" << "@";
    }
    else {
        listOne = QStringList() << "a" << "b" << "c" << "d";
        listTwo = QStringList() << "i" << "j" << "k" << "l";
        listThree = QStringList() << "p" << "q" << "r" << "s";
        listFour = QStringList() << "x" << "y" << "z" << "[ ]";
        listFive = QStringList() << "e" << "f" << "g" << "h";
        listSix = QStringList() << "m" << "n" << "ñ" << "o";
        listSeven = QStringList() << "t" << "u" << "v" << "w";
        listEight = QStringList() << "." << "," << "?" << "!" << "@";
    }
}

void InputDialog::setButtonText(){
    ui->write1Button->setText(listOne.join(' '));
    ui->write2Button->setText(listTwo.join(' '));
    ui->write3Button->setText(listThree.join(' '));
    ui->write4Button->setText(listFour.join(' '));
    ui->write5Button->setText(listFive.join(' '));
    ui->write6Button->setText(listSix.join(' '));
    ui->write7Button->setText(listSeven.join(' '));
    ui->write8Button->setText(listEight.join(' '));
}

void InputDialog::changeIndex(int button){
    int currentIndex = listIndex[button].toInt(0,10);
    if (((currentIndex >= 5)&&(button==7))||((currentIndex >= 4)&&(button!=7))){
        currentIndex = -1;
    }
    setIndex();
    for (int i = 0; i<listIndex.length(); i++){
        if (i == button){
            listIndex[i] = QString::number(currentIndex+1,10);
        }
    }
}

void InputDialog::changeSelection(QStringList &list, int button){
    setKeyboard();
    bool empty = true;
    for (int i = 0; i<list.size();i++){
        if (i == listIndex[button].toInt()){
            ui->selectedLabel->setText("Letra seleccionada: "+list[i]);
            list[i] = "·" + list[i] + "·";
            empty = false;
        }
    }

    if(empty){
        ui->selectedLabel->setText("Letra seleccionada: Ninguna");
    }
}

void InputDialog::on_exitButton_clicked()
{
    switch(state){
        case 0:
            this->~InputDialog();
            break;
        case 1:
            ui->stateLabel->setText("MODO ESCRITURA LIBRE");
            ui->sendButton->setText("Enviar\ne-mail");
            ui->exitButton->setText("Salir");
            break;
        case 2:
            ui->stateLabel->setText("ENVIANDO EMAIL. INTRODUCE ASUNTO");
            break;

    }
    if (state!=0){
        state--;
    }

}

void InputDialog::on_sendButton_clicked()
{
    switch(state){
        case 0:
            body = ui->textField->toPlainText();
            ui->textField->clear();
            ui->sendButton->setText("OK");
            ui->exitButton->setText("Atrás");
            ui->stateLabel->setText("ENVIANDO EMAIL. INTRODUCE ASUNTO");
            break;
        case 1:
            subject = ui->textField->toPlainText();
            ui->textField->clear();
            ui->stateLabel->setText("ENVIANDO EMAIL. INTRODUCE DESTINATARIO");
            break;
        case 2:
            to = ui->textField->toPlainText();
            ui->textField->clear();
            ui->stateLabel->setText("MODO ESCRITURA LIBRE");
            link = QString("https://mail.google.com/mail/u/0/?view=cm&fs=1&to=%1&su=%2&body=%3&tf=1").arg(to).arg(subject).arg(body);
            QDesktopServices::openUrl(QUrl(link));
            //Smtp *newMail  = new Smtp("hectorrogue.9@gmail.com",to,subject,body);
            //delete newMail;
            ui->sendButton->setText("Enviar\ne-mail");
            ui->exitButton->setText("Salir");
            break;
    }
        state++;
        if (state>2){
            state = 0;
        }
}

void InputDialog::on_write1Button_clicked()
{
    changeIndex(0);
    changeSelection(listOne,0);
    setButtonText();
}

void InputDialog::on_write2Button_clicked()
{
    changeIndex(1);
    changeSelection(listTwo,1);
    setButtonText();
}

void InputDialog::on_write3Button_clicked()
{
    changeIndex(2);
    changeSelection(listThree,2);
    setButtonText();
}

void InputDialog::on_write4Button_clicked()
{
    changeIndex(3);
    changeSelection(listFour,3);
    setButtonText();
}

void InputDialog::on_write5Button_clicked()
{
    changeIndex(4);
    changeSelection(listFive,4);
    setButtonText();
}

void InputDialog::on_write6Button_clicked()
{
    changeIndex(5);
    changeSelection(listSix,5);
    setButtonText();
}

void InputDialog::on_write7Button_clicked()
{
    changeIndex(6);
    changeSelection(listSeven,6);
    setButtonText();
}

void InputDialog::on_write8Button_clicked()
{
    changeIndex(7);
    changeSelection(listEight,7);
    setButtonText();
}

void InputDialog::on_confirmButton_clicked()
{
    setKeyboard();
    for (int i = 0; i<listIndex.size();i++){
        if (listIndex[i]!="-1"){
            switch (i) {
            case 0:
                ui->textField->setText(ui->textField->toPlainText()+listOne[listIndex[i].toInt()]);
                break;
            case 1:
                ui->textField->setText(ui->textField->toPlainText()+listTwo[listIndex[i].toInt()]);
                break;
            case 2:
                ui->textField->setText(ui->textField->toPlainText()+listThree[listIndex[i].toInt()]);
                break;
            case 3:
                if (listFour[listIndex[i].toInt()].contains("[ ]")){
                    ui->textField->setText(ui->textField->toPlainText()+" ");
                }
                else{
                    ui->textField->setText(ui->textField->toPlainText()+listFour[listIndex[i].toInt()]);
                }
                break;
            case 4:
                ui->textField->setText(ui->textField->toPlainText()+listFive[listIndex[i].toInt()]);
                break;
            case 5:
                ui->textField->setText(ui->textField->toPlainText()+listSix[listIndex[i].toInt()]);
                break;
            case 6:
                ui->textField->setText(ui->textField->toPlainText()+listSeven[listIndex[i].toInt()]);
                break;
            case 7:
                ui->textField->setText(ui->textField->toPlainText()+listEight[listIndex[i].toInt()]);
                break;
            default:
                break;
            }
        }
    }
}

void InputDialog::on_pushButton_5_clicked()
{

    QString text = ui->textField->toPlainText().remove(ui->textField->toPlainText().length()-1,1);
    ui->textField->setText(text);
}


