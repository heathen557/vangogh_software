#ifndef AUTOSTEPPING_DIALOG_H
#define AUTOSTEPPING_DIALOG_H

#include <QDialog>
#include<QMessageBox>
#include<QFile>
#include<QTextStream>
#include<qdebug.h>

namespace Ui {
class AutoStepping_Dialog;
}

class AutoStepping_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit AutoStepping_Dialog(QWidget *parent = 0);
    ~AutoStepping_Dialog();

public slots:
    void on_step_ok_pushButton_clicked();

    void on_HistogramGet_pushButton_clicked();

    void AckCmd_autoStepping_slot(QString,QString);

signals:
    void sendSerialSignal(QString);             //串口发送信号

private slots:
    void on_test_pushButton_clicked();

    void on_AutoTofTest_pushButton_clicked();

private:
    Ui::AutoStepping_Dialog *ui;
};

#endif // AUTOSTEPPING_DIALOG_H
