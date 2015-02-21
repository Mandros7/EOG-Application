#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QDesktopServices>

InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    setKeyboard();
    listOne = {"a","b","c","d","e","f","g"};
    listTwo = {"h","i","j","k","l","m","n"};
    listThree = {"ñ","o","p","q","r","s","t"};
    listFour = {"u","v","w","x","y","z"};

}

InputDialog::~InputDialog()
{
    delete ui;
}

void InputDialog::setKeyboard(){
    listOne = {"a","b","c","d","e","f","g"};
    listTwo = {"h","i","j","k","l","m","n"};
    listThree = {"ñ","o","p","q","r","s","t"};
    listFour = {"u","v","w","x","y","z"};
    listIndex = {"-1","-1","-1","-1"};
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
