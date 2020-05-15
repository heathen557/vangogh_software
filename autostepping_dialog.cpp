#include "autostepping_dialog.h"
#include "ui_autostepping_dialog.h"

AutoStepping_Dialog::AutoStepping_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AutoStepping_Dialog)
{
    ui->setupUi(this);
}

AutoStepping_Dialog::~AutoStepping_Dialog()
{
    delete ui;
}

//!
//! \brief AutoStepping_Dialog::on_step_ok_pushButton_clicked
//!滑轨 靠近 或 远离 的槽函数
//!  5A 00 02 00 XX XX XX
void AutoStepping_Dialog::on_step_ok_pushButton_clicked()
{
    int InnerOrouterNum = -1;
    if(ui->near_radioButton->isChecked())
    {
        InnerOrouterNum = 0;
    }
    if(ui->outer_radioButton->isChecked())
    {
        InnerOrouterNum = 1;
    }

    int distance = ui->changeDsitance_lineEdit->text().toInt();
    QString cmdStr = "5A 00 03 00 5B";
    QString tmpStr1 = QString("%1").arg(InnerOrouterNum,2,16,QChar('0'));
    QString tmpStr2 = QString("%1").arg(distance,4,16,QChar('0'));
    tmpStr2 = tmpStr2.mid(2,2) + tmpStr2.mid(0,2);

    cmdStr.append(tmpStr1).append(tmpStr2);

    emit sendSerialSignal(cmdStr);



}


//!
//! \brief AutoStepping_Dialog::on_HistogramGet_pushButton_clicked
//! 采集的槽函数
//!  5A 00 06 00 5A F0/F1
void AutoStepping_Dialog::on_HistogramGet_pushButton_clicked()
{
    int bujin_num = ui->Bujin_lineEdit->text().toInt();
    int histogramNum = ui->HistogramNum_lineEdit->text().toInt();

    QString bujinStr = QString("%1").arg(bujin_num,4,16,QChar('0'));
    bujinStr = bujinStr.mid(2,2) + bujinStr.mid(0,2);

    QString histogramStr = QString("%1").arg(histogramNum,4,16,QChar('0'));
    histogramStr = histogramStr.mid(2,2) + histogramStr.mid(0,2);



    if(ui->HistogramGet_pushButton->text() == QStringLiteral("开始采集"))
    {
        QString cmdStr = "5A 00 06 00 5A F1 ";
        cmdStr.append(bujinStr).append(histogramStr);
        emit sendSerialSignal(cmdStr);

        ui->HistogramGet_pushButton->setText(QStringLiteral("停止"));
    }else
    {
        QString cmdStr = "5A 00 06 00 5A F0 ";
        cmdStr.append(bujinStr).append(histogramStr);
        emit sendSerialSignal(cmdStr);
        ui->HistogramGet_pushButton->setText(QStringLiteral("开始采集"));
    }
}
