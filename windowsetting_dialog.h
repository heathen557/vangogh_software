#ifndef WINDOWSETTING_DIALOG_H
#define WINDOWSETTING_DIALOG_H

#include <QDialog>
#include<qlineedit.h>
#include<QMessageBox>
#include"globaldata.h"

namespace Ui {
class windowSetting_Dialog;
}

class windowSetting_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit windowSetting_Dialog(QWidget *parent = 0);
    ~windowSetting_Dialog();

    QList<QLineEdit *> LineEdit_list;



public slots:
    void on_ok_pushButton_clicked();

    void AckCmd_windowSetting_slot(QString,QString);

signals:
    void sendSerialSignal(QString);             //串口发送信号


private:
    Ui::windowSetting_Dialog *ui;
};

#endif // WINDOWSETTING_DIALOG_H
