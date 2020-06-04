#include "delayline_dialog.h"
#include "ui_delayline_dialog.h"

delayLine_Dialog::delayLine_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delayLine_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);

    tableWidget_list.append(ui->TDC_widget);
    tableWidget_list.append(ui->TDC_widget_2);
    tableWidget_list.append(ui->TDC_widget_3);
    tableWidget_list.append(ui->TDC_widget_4);
    tableWidget_list.append(ui->TDC_widget_5);
    tableWidget_list.append(ui->TDC_widget_6);
    tableWidget_list.append(ui->TDC_widget_7);
    tableWidget_list.append(ui->TDC_widget_8);


    label_list.append(ui->label_TDC0);
    label_list.append(ui->label_TDC0_2);
    label_list.append(ui->label_TDC0_3);
    label_list.append(ui->label_TDC0_4);
    label_list.append(ui->label_TDC0_5);
    label_list.append(ui->label_TDC0_6);
    label_list.append(ui->label_TDC0_7);
    label_list.append(ui->label_TDC0_8);




    for(int i=0;i<8;i++)
    {
        tableWidget_list[i]->xAxis->setRange(0,8);
        tableWidget_list[i]->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
        tableWidget_list[i]->addGraph();
        tableWidget_list[i]->graph(0)->setPen(QPen(Qt::darkGreen));
//        tableWidget_list[i]->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    }


//    label_x.resize(9);
//    for(int i=0;i<9;i++)
//    {
//        label_x[i] = i;
//    }



    //4300
    vi4300_HistorgramLabels.resize(9);
    vi4300_HistorgramTicks.resize(9);
    for(int i=0; i<9; i++)
    {
        vi4300_HistorgramTicks[i] = i;
        vi4300_HistorgramLabels[i] = "TDC" + QString::number(8-i);

    }

//    QVector<double> numData(16);
//    QVector<double> label(16);
//    for(int i=0;i<16;i++)
//    {
//        numData[i] =i;
//        label[i] = i;
//    }
//    ui->TDC_widget->yAxis->setRange(0,16);
//    ui->TDC_widget->graph(0)->setData(label,numData);
//    ui->TDC_widget->replot();

}

delayLine_Dialog::~delayLine_Dialog()
{
    delete ui;
}


//!
//! \brief delayLine_Dialog::on_getPhase_pushButton_clicked
//!  5A 00 01 00 5E
void delayLine_Dialog::on_getPhase_pushButton_clicked()
{
    QString cmdStr = "5A 00 01 00 5E ";
    emit sendSerialSignal(cmdStr);
}

//!
//! \brief delayLine_Dialog::on_delayLine_pushButton_clicked
//! 获取折线图
//! 5A 00 NN 00 5D PP
void delayLine_Dialog::on_delayLine_pushButton_clicked()
{
    QString tmpStr = ui->phase_lineEdit->text();
    QString phase = "00";
    phase.append(tmpStr);
    phase = phase.right(2);

    QString cmdStr = "5A 00 02 00 5D ";
    cmdStr.append(phase);
    emit sendSerialSignal(cmdStr);

}

//!
//! \brief delayLine_Dialog::AckCmd_delayLine_slot
//!   805E :相位  数据区 00：ok 01:error    2字节：相位
void delayLine_Dialog::AckCmd_delayLine_slot(QString returnCmdStr,QString cmdInfo)
{
    if("805E" == returnCmdStr)    //获取相位
    {
        int flag = cmdInfo.mid(0,2).toInt(NULL,16);
        if(0 == flag)
        {
            QString phaseStr = cmdInfo.mid(2,2);

            ui->phase_lineEdit->setText(phaseStr);
        }else
        {
            QMessageBox::warning(NULL,QStringLiteral("提示"),QStringLiteral("获取时钟相位失败！"));
        }

    }else if("805D" == returnCmdStr)
    {

        QString text;

        int len = cmdInfo.length();
        for(int i=0; i<len; i+=4)
        {
            text.append(cmdInfo.mid(i,4)).append("\n");
        }
        QString sfile="tdc.txt";

        QFile file(sfile);
        if(file.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream out(&file);
            out<<text.toLocal8Bit()<<endl;
            file.close();
        }else
        {
            QString strMsg = QStringLiteral("文件保存失败，请检查路径");
            QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
        }



        QString tmpSingleStr;
        int tmpSingleInt;
        QVector<double> numData[10];
        int lastNum = -1;
        bool flag = true;    //是否是单调递增
        int max = -1;


        for(int i=0; i<8; i++)    //Step的序号   8个Step 9个TDC
        {
            lastNum = 10000;
            numData[i+1].resize(9);
            max = -1;
            flag = true;
            for(int j=0;j<36; j+=4)
            {
                tmpSingleStr = cmdInfo.mid(i*36+j,4);
                tmpSingleInt = tmpSingleStr.toInt(NULL,16);
                numData[i+1][8 - j/4] = tmpSingleInt;         //数据TDC0-TDC8  显示要从TDC8 -TDC0


                if(tmpSingleInt>max)
                    max = tmpSingleInt;
                if(tmpSingleInt > lastNum)
                {
                    flag = false;
                }
                lastNum = tmpSingleInt;
            } //1个TDC的数据已经接收完毕


            if(flag)  //单调递增
            {
                 tableWidget_list[i]->graph(0)->setPen(QPen(Qt::darkGreen));
                 label_list[i]->setText("SUCCESS");
                 label_list[i]->setStyleSheet("color:green;");
            }else   //单调递减
            {
                tableWidget_list[i]->graph(0)->setPen(QPen(Qt::red));
                label_list[i]->setText("FAIL");
                label_list[i]->setStyleSheet("color:red;");
            }


            QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
            textTicker->setTicks(vi4300_HistorgramTicks, vi4300_HistorgramLabels);
            tableWidget_list[i]->xAxis->setTicker(textTicker);


            tableWidget_list[i]->yAxis->setRange(0,max);
            tableWidget_list[i]->graph(0)->setData(vi4300_HistorgramTicks,numData[i+1]);
            tableWidget_list[i]->replot();

        }
    }
}

void delayLine_Dialog::on_test_pushButton_clicked()
{

    QString tmpStr = "805D";
    QString cmdStr;
    QString singleStr;
    for(int i=0; i<8; i++)
    {
        for(int j=9; j>0; j--)
        {

            if(i==0 && j==5)
            {
                singleStr = QString("%1").arg(21,4,16,QChar('0'));
            }else
            {
                singleStr = QString("%1").arg(j*10,4,16,QChar('0'));
            }
            cmdStr.append(singleStr);
        }
    }

    AckCmd_delayLine_slot(tmpStr,cmdStr);

}

void delayLine_Dialog::on_test_pushButton_2_clicked()
{
    QString tmpStr = "805D";
    QString cmdStr;
    QString singleStr;
    for(int i=0; i<8; i++)
    {
        for(int j=9; j>0; j--)
        {

            if(i==7&& j==6)
            {
                singleStr = QString("%1").arg(21,4,16,QChar('0'));
            }else
            {
                singleStr = QString("%1").arg(j*10,4,16,QChar('0'));
            }
            cmdStr.append(singleStr);
        }
    }

    AckCmd_delayLine_slot(tmpStr,cmdStr);
}
