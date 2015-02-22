#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QDesktopServices>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    setKeyboard();
    setIndex();

}

InputDialog::~InputDialog()
{
    delete ui;
}
void InputDialog::setIndex(){
    listIndex = {"-1","-1","-1","-1"};
}

void InputDialog::setKeyboard(){
    listOne = {"A","B","C","D","E","F","G"};
    listTwo = {"H","I","J","K","L","M","N"};
    listThree = {"Ñ","O","P","Q","R","S","T"};
    listFour = {"U","V","W","X","Y","Z"};
}

void InputDialog::setButtonText(){
    ui->write1Button->setText(listOne.join(' '));
    ui->write2Button->setText(listTwo.join(' '));
    ui->write3Button->setText(listThree.join(' '));
    ui->write4Button->setText(listFour.join(' '));
}

void InputDialog::changeIndex(int button){
    int currentIndex = listIndex[button].toInt(0,10);
    if (((currentIndex >= 6)&&(button<=2))||((currentIndex >= 5)&&(button==3))){
        currentIndex = -1;
    }
    setIndex();
    for (int i = 0; i<4; i++){
        if (i == button){
            listIndex[i] = QString::number(currentIndex+1,10);
        }
    }
}

void InputDialog::changeSelection(QStringList &list, int button){
    setKeyboard();
    for (int i = 0; i<list.size();i++){
        if (i == listIndex[button].toInt()){
            list[i] = "-" + list[i] + "-";
            //list[i]
        }
    }
}

void InputDialog::on_exitButton_clicked()
{
    this->~InputDialog();
}

void InputDialog::on_sendButton_clicked()
{
    QString body = ui->textField->toPlainText();
    QString to = "alguien@example.com";
    QString su = "Asunto";
    QString link = QString("https://mail.google.com/mail/u/0/?view=cm&fs=1&to=%1&su=%2&body=%3&tf=1").arg(to).arg(su).arg(body);
    QDesktopServices::openUrl(QUrl(link));
    //ui->webView->load(QUrl("http://www.google.es"));
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
                ui->textField->setText(ui->textField->toPlainText()+listFour[listIndex[i].toInt()]);
                break;
            default:
                break;
            }
        }
    }
}
