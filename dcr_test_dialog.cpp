#include "dcr_test_dialog.h"
#include "ui_dcr_test_dialog.h"

DCR_test_Dialog::DCR_test_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DCR_test_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_2->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_3->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_4->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_5->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_5->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_6->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_7->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_7->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_8->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_8->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_9->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_9->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_10->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_10->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_11->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_11->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_12->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_12->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_13->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_13->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_14->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_14->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_15->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_15->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_16->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_16->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_17->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_17->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int index = 0;
    for(int row=0; row<3; row++)
    {
        for(int col = 0; col<3; col++)
        {

            ui->tableWidget->setItem(row,col,&DCR_Item[0][index]);
            DCR_Item[0][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[0][index].setText(QString::number(1));

            ui->tableWidget_2->setItem(row,col,&DCR_Item[1][index]);
            DCR_Item[1][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[1][index].setText(QString::number(2*1000));

            ui->tableWidget_3->setItem(row,col,&DCR_Item[2][index]);
            DCR_Item[2][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[2][index].setText(QString::number(3*1000));

            ui->tableWidget_4->setItem(row,col,&DCR_Item[3][index]);
            DCR_Item[3][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[3][index].setText(QString::number(4*1000));

            ui->tableWidget_5->setItem(row,col,&DCR_Item[4][index]);
            DCR_Item[4][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[4][index].setText(QString::number(5*1000));

            ui->tableWidget_6->setItem(row,col,&DCR_Item[5][index]);
            DCR_Item[5][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[5][index].setText(QString::number(6*1000));

            ui->tableWidget_7->setItem(row,col,&DCR_Item[6][index]);
            DCR_Item[6][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[6][index].setText(QString::number(7*1000));

            ui->tableWidget_8->setItem(row,col,&DCR_Item[7][index]);
            DCR_Item[7][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[7][index].setText(QString::number(8*1000));

            ui->tableWidget_9->setItem(row,col,&DCR_Item[8][index]);
            DCR_Item[8][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[8][index].setText(QString::number(9*1000));

            ui->tableWidget_10->setItem(row,col,&DCR_Item[9][index]);
            DCR_Item[9][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[9][index].setText(QString::number(10*1000));

            ui->tableWidget_11->setItem(row,col,&DCR_Item[10][index]);
            DCR_Item[10][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[10][index].setText(QString::number(11*1000));

            ui->tableWidget_12->setItem(row,col,&DCR_Item[11][index]);
            DCR_Item[11][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[11][index].setText(QString::number(12*1000));

            ui->tableWidget_13->setItem(row,col,&DCR_Item[12][index]);
            DCR_Item[12][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[12][index].setText(QString::number(13*1000));

            ui->tableWidget_14->setItem(row,col,&DCR_Item[13][index]);
            DCR_Item[13][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[13][index].setText(QString::number(14*1000));

            ui->tableWidget_15->setItem(row,col,&DCR_Item[14][index]);
            DCR_Item[14][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[14][index].setText(QString::number(15*1000));

            ui->tableWidget_16->setItem(row,col,&DCR_Item[15][index]);
            DCR_Item[15][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[15][index].setText(QString::number(16*1000));

            ui->tableWidget_17->setItem(row,col,&DCR_Item[16][index]);
            DCR_Item[16][index].setTextAlignment(Qt::AlignCenter);
//            DCR_Item[16][index].setText(QString::number(17*1000));

            index++;
        }
    }
}

DCR_test_Dialog::~DCR_test_Dialog()
{
    delete ui;
}


//!
//! \brief DCR_test_Dialog::on_start_pushButton_clicked
//!  发送采集命令
//!  5A 00 02 00 5C
void DCR_test_Dialog::on_start_pushButton_clicked()
{
    QString tmpStr = ui->votage_lineEdit->text();
    QString valtageStr = "00" + tmpStr;
    valtageStr = valtageStr.right(2);
    QString DCR_offset_str = ui->timeOffset_comboBox->currentText();


    QString cmdStr = "5A 00 02 00 5C";
    cmdStr.append(valtageStr).append(DCR_offset_str);
    emit sendSerialSignal(cmdStr);

}

//返回命令
void DCR_test_Dialog::AckCmd_DCRTest_slot(QString returnCmdStr,QString cmdInfo)
{
    int index = 0;
    QString tmpStr;
    if("805C" == returnCmdStr)
    {
        for(int i=0; i<17; i++)
        {
            for(int j=0; j<9;j++)
            {
                tmpStr = cmdInfo.mid(index*4,4);
                int DCR_tmp_int = tmpStr.toInt(NULL,16);
                DCR_Item[i][j].setText(QString::number(DCR_tmp_int));

                index++;
            }
        }
    }
}
