#include "pixelsetting_dialog.h"
#include "ui_pixelsetting_dialog.h"

pixelSetting_Dialog::pixelSetting_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pixelSetting_Dialog)
{
    ui->setupUi(this);

    QTableWidget_list.append(ui->tableWidget);
    QTableWidget_list.append(ui->tableWidget_2);
    QTableWidget_list.append(ui->tableWidget_3);
    QTableWidget_list.append(ui->tableWidget_4);
    QTableWidget_list.append(ui->tableWidget_5);
    QTableWidget_list.append(ui->tableWidget_6);
    QTableWidget_list.append(ui->tableWidget_7);
    QTableWidget_list.append(ui->tableWidget_8);
    QTableWidget_list.append(ui->tableWidget_9);
    QTableWidget_list.append(ui->tableWidget_10);
    QTableWidget_list.append(ui->tableWidget_11);
    QTableWidget_list.append(ui->tableWidget_12);
    QTableWidget_list.append(ui->tableWidget_13);
    QTableWidget_list.append(ui->tableWidget_14);
    QTableWidget_list.append(ui->tableWidget_15);
    QTableWidget_list.append(ui->tableWidget_16);
    QTableWidget_list.append(ui->tableWidget_17);

    for(int i=0; i<17; i++)
    {
        QTableWidget_list[i]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        QTableWidget_list[i]->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    }



    int byteIndex = 0;   //第几个字节
    for(int row=0; row<3; row++)
    {
        for(int col=0; col<3; col++)
        {

            for(int tableIndex=0; tableIndex<8; tableIndex++)   // tableIndex : 第几个tableWidget
            {

                QTableWidget_list[tableIndex]->setItem(row,col,&checkBox_byte[byteIndex][tableIndex]);
                checkBox_byte[byteIndex][tableIndex].setCheckState(Qt::Checked);
                QString str = "[" + QString::number(byteIndex)+"]:["+QString::number(tableIndex) +"]";
                checkBox_byte[byteIndex][tableIndex].setText(str);

            }
            byteIndex++;
            for(int tableIndex=8; tableIndex<16; tableIndex++)
            {
                QTableWidget_list[tableIndex]->setItem(row,col,&checkBox_byte[byteIndex][tableIndex-8]);
                checkBox_byte[byteIndex][tableIndex-8].setCheckState(Qt::Checked);
                QString str = "[" + QString::number(byteIndex)+"]:["+QString::number(tableIndex-8) +"]";
                checkBox_byte[byteIndex][tableIndex-8].setText(str);
            }
            byteIndex++;
        }
    }


}

pixelSetting_Dialog::~pixelSetting_Dialog()
{
    delete ui;
}

//!
//! \brief pixelSetting_Dialog::on_setting_pushButton_clicked
//!总共18个字节
void pixelSetting_Dialog::on_setting_pushButton_clicked()
{
    QString dataStr;

    for(int byteIndex=0; byteIndex<18; byteIndex++)   //18个字节
    {
        int singleByte = 0;
        QString tmpStr;
        for(int bit=0; bit<8; bit++)
        {
            if(checkBox_byte[byteIndex][bit].checkState() == Qt::Checked)
            {


                if(0 ==bit)
                {
                    singleByte = singleByte | 0x01;

                }else if(1 ==bit)
                {
                    singleByte = singleByte | 0x02;

                }else if(2 ==bit)
                {
                    singleByte = singleByte | 0x04;

                }else if(3 == bit)
                {
                    singleByte = singleByte | 0x08;

                }else if(4 == bit)
                {
                    singleByte = singleByte | 0x10;

                }else if(5 == bit)
                {
                    singleByte = singleByte | 0x20;

                }else if(6 == bit)
                {
                    singleByte = singleByte | 0x40;

                }else if(7 == bit)
                {
                    singleByte = singleByte | 0x80;

                }
            }
        }
        tmpStr = QString("%1").arg(singleByte,2,16,QChar('0')).toUpper();
        dataStr.append(tmpStr).append(" ");
    }


    ui->result_lineEdit->setText(dataStr);


    QString cmdStr = "5A 00 01 00 62";
    cmdStr.append(dataStr);
    emit sendSerialSignal(cmdStr);

}

//!
//! \brief pixelSetting_Dialog::AckCmd_PixelSetting_slot
//! 单pixel的返回信息
void pixelSetting_Dialog::AckCmd_PixelSetting_slot(QString returnCmdStr,QString cmdInfo)
{
    if("8062" == returnCmdStr)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("pixel使能设置成功！"));
    }
}
