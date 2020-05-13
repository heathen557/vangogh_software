#ifndef CHANNEKPARAMETER_DIALOG_H
#define CHANNEKPARAMETER_DIALOG_H

#include <QDialog>
#include<QMessageBox>

namespace Ui {
class channekParameter_Dialog;
}

class channekParameter_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit channekParameter_Dialog(QWidget *parent = 0);
    ~channekParameter_Dialog();

signals:
    void sendSerialSignal(QString);             //串口发送信号

public slots:

    void on_read_pushButton_clicked();

    void on_write_pushButton_clicked();

    void AckCmd_channelParameter_slot(QString,QString);

private:
    Ui::channekParameter_Dialog *ui;
};

#endif // CHANNEKPARAMETER_DIALOG_H
