#include "windowsetting_dialog.h"
#include "ui_windowsetting_dialog.h"

windowSetting_Dialog::windowSetting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::windowSetting_Dialog)
{
    ui->setupUi(this);

     LineEdit_list.append(ui->lineEdit);
     LineEdit_list.append(ui->lineEdit_2);
     LineEdit_list.append(ui->lineEdit_3);
     LineEdit_list.append(ui->lineEdit_4);
     LineEdit_list.append(ui->lineEdit_5);
     LineEdit_list.append(ui->lineEdit_6);
     LineEdit_list.append(ui->lineEdit_7);
     LineEdit_list.append(ui->lineEdit_8);
     LineEdit_list.append(ui->lineEdit_9);
     LineEdit_list.append(ui->lineEdit_10);
     LineEdit_list.append(ui->lineEdit_11);
     LineEdit_list.append(ui->lineEdit_12);
     LineEdit_list.append(ui->lineEdit_13);
     LineEdit_list.append(ui->lineEdit_14);
     LineEdit_list.append(ui->lineEdit_15);
     LineEdit_list.append(ui->lineEdit_16);


     QSettings paraSetting("parameters.ini", QSettings::IniFormat);
     for(int i=0;i<16; i++)
     {
          QString keyStr = "window_para/A";
          keyStr.append(QString::number(i));
          QString valueStr = paraSetting.value(keyStr).toString();
          LineEdit_list[i]->setText(valueStr);
     }

}

windowSetting_Dialog::~windowSetting_Dialog()
{
    delete ui;
}

void windowSetting_Dialog::AckCmd_windowSetting_slot(QString returnCmdStr,QString returnInfo)
{
    if("815F" == returnCmdStr)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("窗口设置成功！"));
    }
}



//!设置
void windowSetting_Dialog::on_ok_pushButton_clicked()
{
    int len = LineEdit_list.size();
    QString strCmd = "5A 01 02 00 5F";
    QString singleStr ;
    int tmpNum;
    for(int i=0;i<len; i++)
    {
        tmpNum = LineEdit_list[i]->text().toInt();
        if(tmpNum>15)
        {
            QMessageBox::warning(NULL,QStringLiteral("提示"),QStringLiteral("输入有误，不能输入大于15的数"));
        }
        singleStr = QString("%1").arg(tmpNum,2,16,QChar('0'));
        strCmd.append(singleStr);
    }

    emit sendSerialSignal(strCmd);


    QSettings paraSetting("parameters.ini", QSettings::IniFormat);


    for(int i=0;i<16; i++)
    {
         QString keyStr = "window_para/A";
         keyStr.append(QString::number(i));
         paraSetting.setValue(keyStr,LineEdit_list[i]->text());
    }





}
