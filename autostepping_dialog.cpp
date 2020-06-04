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
    QString sFilePath = ui->filePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        return;
    }
    //设置文件的保存路径
    if(sFilePath.right(1) != "\\")
    {
        sFilePath.append("\\");
    }
    ui->filePath_lineEdit->setText(sFilePath);




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


//!
//! \brief AutoStepping_Dialog::AckCmd_autoStepping_slot
//! \param returnCmdStr
//! \param returnInfo
//!返回命令
void AutoStepping_Dialog::AckCmd_autoStepping_slot(QString returnCmdStr,QString returnInfo)
{
    int len = returnInfo.size();
    if(len<2)
        return;

    if("805A" == returnCmdStr)
    {
        QString positionTmp = returnInfo.mid(2,2) + returnInfo.mid(0,2);
        QString positionStr = QString::number(positionTmp.toInt(NULL,16));

        QString TDC_str = QString::number(returnInfo.mid(4,2).toInt(NULL,16));

        QString indexTmp = returnInfo.mid(8,2) +returnInfo.mid(6,2) ;
        QString indexStr = QString::number(indexTmp.toInt(NULL,16));

//        QString text = returnInfo.right(1024*2);
        QString text = returnInfo.mid(10,2048);

        qDebug()<<"text = "<<text;

        QString filePath = ui->filePath_lineEdit->text();
        QString sFilePath =filePath+ positionStr+"_"+TDC_str+"_"+indexStr+".txt";    //文件名

        qDebug()<<" sfilePath = "<<sFilePath;
        QFile file(sFilePath);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<text.toLocal8Bit()<<endl;
            file.close();
//            ui->filePath_lineEdit->setText(sFilePath);
//            QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("文件已保存！"));
        }else
        {
            QString strMsg = QStringLiteral("文件保存失败，请检查路径");
            QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
        }

    }
}

void AutoStepping_Dialog::on_test_pushButton_clicked()
{
    QString  str1 = "805A";
    QString str2  = "01000902";
    for(int i=0;i<1024;i++)
    {
        str2.append(QString("%1").arg(i%100,2,16,QChar('0')).toUpper());
    }
    AckCmd_autoStepping_slot(str1,str2);
    qDebug()<<" str2 = "<<str2;


}

void AutoStepping_Dialog::on_AutoTofTest_pushButton_clicked()
{
    QString sFilePath = ui->filePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        return;
    }
    //设置文件的保存路径
    if(sFilePath.right(1) != "\\")
    {
        sFilePath.append("\\");
    }
    ui->filePath_lineEdit->setText(sFilePath);

    int bujin_num = ui->Bujin_lineEdit->text().toInt();
    int histogramNum = ui->HistogramNum_lineEdit->text().toInt();

    QString bujinStr = QString("%1").arg(bujin_num,4,16,QChar('0'));
    bujinStr = bujinStr.mid(2,2) + bujinStr.mid(0,2);

    QString histogramStr = QString("%1").arg(histogramNum,4,16,QChar('0'));
    histogramStr = histogramStr.mid(2,2) + histogramStr.mid(0,2);

    if(ui->AutoTofTest_pushButton->text() == QStringLiteral("自动TOF采集"))
    {
        QString cmdStr = "5A 00 06 00 5A F2 ";
        cmdStr.append(bujinStr).append(histogramStr);
        emit sendSerialSignal(cmdStr);

        ui->AutoTofTest_pushButton->setText(QStringLiteral("停止"));
    }else
    {
        QString cmdStr = "5A 00 06 00 5A F0 ";
        cmdStr.append(bujinStr).append(histogramStr);
        emit sendSerialSignal(cmdStr);
        ui->AutoTofTest_pushButton->setText(QStringLiteral("自动TOF采集"));
    }



}
