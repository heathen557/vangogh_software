#ifndef DELAYLINE_DIALOG_H
#define DELAYLINE_DIALOG_H

#include <QDialog>
#include"globaldata.h"
#include<QLabel>

namespace Ui {
class delayLine_Dialog;
}

class delayLine_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit delayLine_Dialog(QWidget *parent = 0);
    ~delayLine_Dialog();

    QList<QCustomPlot *> tableWidget_list;

    QList<QLabel *> label_list;

    QVector<double> label_x;



    QVector<double> vi4300_HistorgramTicks;      //标签
    QVector<QString> vi4300_HistorgramLabels;    //标识
signals:
    void sendSerialSignal(QString );             //串口发送信号

public slots:
    void on_getPhase_pushButton_clicked();

    void on_delayLine_pushButton_clicked();

    void AckCmd_delayLine_slot(QString,QString);

private slots:
    void on_test_pushButton_clicked();

    void on_test_pushButton_2_clicked();

private:
    Ui::delayLine_Dialog *ui;
};

#endif // DELAYLINE_DIALOG_H
