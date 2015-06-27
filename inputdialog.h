#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>

namespace Ui {
class InputDialog;
}

class InputDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = 0);
    ~InputDialog();

private slots:
    void on_exitButton_clicked();

    void on_sendButton_clicked();

    void on_write1Button_clicked();

    void on_write2Button_clicked();

    void on_write3Button_clicked();

    void on_write4Button_clicked();

    void on_confirmButton_clicked();

    void on_write5Button_clicked();

    void on_write6Button_clicked();

    void on_write7Button_clicked();

    void on_mayusButton_clicked();

    void on_pushButton_5_clicked();

private:
    void setKeyboard();
    void setIndex();
    void setButtonText();
    void changeIndex(int button);
    void changeSelection(QStringList &list, int button);
    Ui::InputDialog *ui;
    QStringList listOne;
    QStringList listTwo;
    QStringList listThree;
    QStringList listFour;
    QStringList listFive;
    QStringList listSix;
    QStringList listSeven;
    QStringList listIndex;

    bool mayus;
    QString body;
    QString su;
    QString to;
};

#endif // INPUTDIALOG_H
