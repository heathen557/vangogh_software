#include "channekparameter_dialog.h"
#include "ui_channekparameter_dialog.h"

channekParameter_Dialog::channekParameter_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::channekParameter_Dialog)
{
    ui->setupUi(this);
}

channekParameter_Dialog::~channekParameter_Dialog()
{
    delete ui;
}


//!
//! \brief channekParameter_Dialog::on_read_pushButton_clicked
//! 读取通道校准系数的槽函数 5A 01 02 00 41 00
void channekParameter_Dialog::on_read_pushButton_clicked()
{
    QString cmdStr = "5A 01 02 00 41 00";
    emit sendSerialSignal(cmdStr);
}

//!
//! \brief channekParameter_Dialog::on_write_pushButton_clicked
//! 设置通道校准系数的槽函数  5A 01 NN NN 41 00 DD DD
void channekParameter_Dialog::on_write_pushButton_clicked()
{
    //从控件上获取校准系数
    int para_1 = ui->lineEdit->text().toFloat();
    int para_2 = ui->lineEdit_2->text().toFloat();
    int para_3 = ui->lineEdit_3->text().toFloat();
    int para_4 = ui->lineEdit_4->text().toFloat();
    int para_5 = ui->lineEdit_5->text().toFloat();
    int para_6 = ui->lineEdit_6->text().toFloat();
    int para_7 = ui->lineEdit_7->text().toFloat();
    int para_8 = ui->lineEdit_8->text().toFloat();
    int para_9 = ui->lineEdit_9->text().toFloat();
    int para_10 = ui->lineEdit_10->text().toFloat();


    QString tmpStr;
    QString paraStr[10];
    tmpStr = QString("%1").arg(para_1,4,16,QChar('0'));
    paraStr[0] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_2,4,16,QChar('0'));
    paraStr[1] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_3,4,16,QChar('0'));
    paraStr[2] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_4,4,16,QChar('0'));
    paraStr[3] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_5,4,16,QChar('0'));
    paraStr[4] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_6,4,16,QChar('0'));
    paraStr[5] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_7,4,16,QChar('0'));
    paraStr[6] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_8,4,16,QChar('0'));
    paraStr[7] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_9,4,16,QChar('0'));
    paraStr[8] = tmpStr.mid(2,2)+tmpStr.mid(0,2);

    tmpStr = QString("%1").arg(para_10,4,16,QChar('0'));
    paraStr[9] = tmpStr.mid(2,2)+tmpStr.mid(0,2);


    QString cmdStr="5A 01 00 00 41";
    QString numStr;
    for(int i=0; i<10; i++)
    {
        numStr = QString("%1").arg(i,2,16,QChar('0'));
        cmdStr.append(numStr).append(paraStr[i]);
    }


    emit sendSerialSignal(cmdStr);
}


void channekParameter_Dialog::AckCmd_channelParameter_slot(QString returnCmdStr,QString returnInfo)
{
    if("8141"==returnCmdStr)
    {
        QMessageBox::warning(NULL,QStringLiteral("提示"),QStringLiteral("通道校准系数写入成功！"));
    }else if("8041" == returnCmdStr)
    {

        int res[10];
        QString tmpStr;
        QString resStr;
        for(int i=0; i<10; i++)
        {
            tmpStr = returnInfo.mid(i*6+2,4);
            resStr = tmpStr.mid(2,2) +tmpStr.mid(0,2);
            res[i] = resStr.toInt(NULL,16);
        }

        ui->lineEdit->setText(QString::number(res[0]));
        ui->lineEdit_2->setText(QString::number(res[1]));
        ui->lineEdit_3->setText(QString::number(res[2]));
        ui->lineEdit_4->setText(QString::number(res[3]));
        ui->lineEdit_5->setText(QString::number(res[4]));
        ui->lineEdit_6->setText(QString::number(res[5]));
        ui->lineEdit_7->setText(QString::number(res[6]));
        ui->lineEdit_8->setText(QString::number(res[7]));
        ui->lineEdit_9->setText(QString::number(res[8]));
        ui->lineEdit_10->setText(QString::number(res[9]));
    }
}
