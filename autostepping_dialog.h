#ifndef AUTOSTEPPING_DIALOG_H
#define AUTOSTEPPING_DIALOG_H

#include <QDialog>

namespace Ui {
class AutoStepping_Dialog;
}

class AutoStepping_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoStepping_Dialog(QWidget *parent = 0);
    ~AutoStepping_Dialog();

private slots:
    void on_step_ok_pushButton_clicked();

    void on_HistogramGet_pushButton_clicked();

signals:
    void sendSerialSignal(QString);             //串口发送信号

private:
    Ui::AutoStepping_Dialog *ui;
};

#endif // AUTOSTEPPING_DIALOG_H
