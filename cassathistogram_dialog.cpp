#include "cassathistogram_dialog.h"
#include "ui_cassathistogram_dialog.h"

cassatHistogram_Dialog::cassatHistogram_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cassatHistogram_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);
    binSize = 512;
    init_histogram();
    lasthistogramIndex= 0;
    isSaveFileFlag = false;

    ui->pushButton->setVisible(false);
    ui->saveVangogh_histogram_pushButton->setVisible(false);

    connect(&vangogh_read_timer,SIGNAL(timeout()),this,SLOT(vangogh_read_timer_slot()));
}

void cassatHistogram_Dialog::init_histogram()      //初始化直方图数据
{
//    //cassatt
    vangogh_HistorgramLabels.resize(binSize);
    vangogh_HistorgramTicks.resize(binSize);
    for(int i=0; i<binSize; i++)
    {
        vangogh_HistorgramTicks[i] = i;
        vangogh_HistorgramLabels[i] = "";
        if(0 == i%10)
        {
            vangogh_HistorgramLabels[i] = QString::number(i);
        }
    }
//    regen_vangogh[0] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[0]->setAntialiased(false);
//    regen_vangogh[0]->setStackingGap(2);
//    regen_vangogh[0]->setName("Histogram_0");
//    regen_vangogh[0]->setPen(QPen(QColor(200, 0, 0).lighter(130)));
//    regen_vangogh[0]->setBrush(QColor(200, 0, 0));

//    regen_vangogh[1] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[1]->setAntialiased(false);
//    regen_vangogh[1]->setStackingGap(2);
//    regen_vangogh[1]->setName("Histogram_1");
//    regen_vangogh[1]->setPen(QPen(QColor(200, 100, 0).lighter(130)));
//    regen_vangogh[1]->setBrush(QColor(200, 100, 0));


//    regen_vangogh[2] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[2]->setAntialiased(false);
//    regen_vangogh[2]->setStackingGap(2);
//    regen_vangogh[2]->setName("Histogram_2");
//    regen_vangogh[2]->setPen(QPen(QColor(200, 0, 100).lighter(130)));
//    regen_vangogh[2]->setBrush(QColor(200, 0, 100));


//    regen_vangogh[3] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[3]->setAntialiased(false);
//    regen_vangogh[3]->setStackingGap(2);
//    regen_vangogh[3]->setName("Histogram_3");
//    regen_vangogh[3]->setPen(QPen(QColor(200, 100, 100).lighter(130)));
//    regen_vangogh[3]->setBrush(QColor(200, 100, 100));

//    regen_vangogh[4] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[4]->setAntialiased(false);
//    regen_vangogh[4]->setStackingGap(2);
//    regen_vangogh[4]->setName("Histogram_4");
//    regen_vangogh[4]->setPen(QPen(QColor(250, 100, 200).lighter(130)));
//    regen_vangogh[4]->setBrush(QColor(250, 100, 200));

//    regen_vangogh[5] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[5]->setAntialiased(false);
//    regen_vangogh[5]->setStackingGap(2);
//    regen_vangogh[5]->setName("Histogram_5");
//    regen_vangogh[5]->setPen(QPen(QColor(100, 200, 250).lighter(130)));
//    regen_vangogh[5]->setBrush(QColor(100, 200, 250));

//    regen_vangogh[6] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[6]->setAntialiased(false);
//    regen_vangogh[6]->setStackingGap(2);
//    regen_vangogh[6]->setName("Histogram_6");
//    regen_vangogh[6]->setPen(QPen(QColor(200, 250, 100).lighter(130)));
//    regen_vangogh[6]->setBrush(QColor(200, 250, 100));

//    regen_vangogh[7] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[7]->setAntialiased(false);
//    regen_vangogh[7]->setStackingGap(2);
//    regen_vangogh[7]->setName("Histogram_7");
//    regen_vangogh[7]->setPen(QPen(QColor(0, 0, 250).lighter(130)));
//    regen_vangogh[7]->setBrush(QColor(0, 0, 250));

//    regen_vangogh[8] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[8]->setAntialiased(false);
//    regen_vangogh[8]->setStackingGap(2);
//    regen_vangogh[8]->setName("Histogram_8");
//    regen_vangogh[8]->setPen(QPen(QColor(150, 150, 0).lighter(130)));
//    regen_vangogh[8]->setBrush(QColor(150, 150, 0));

//    regen_vangogh[9] = new QCPBars(ui->vangoghHistogram_widget->xAxis,ui->vangoghHistogram_widget->yAxis);
//    regen_vangogh[9]->setAntialiased(false);
//    regen_vangogh[9]->setStackingGap(2);
//    regen_vangogh[9]->setName("Histogram_9");
//    regen_vangogh[9]->setPen(QPen(QColor(0, 150, 150).lighter(130)));
//    regen_vangogh[9]->setBrush(QColor(0, 150, 150));





//    ui->vangoghHistogram_widget->xAxis->setTickLabelRotation(60);//设置标签角度旋转
//    ui->vangoghHistogram_widget->xAxis->setSubTicks(false);//设置是否显示子标签
//    ui->vangoghHistogram_widget->xAxis->setTickLength(0, 4);
//    ui->vangoghHistogram_widget->xAxis->setRange(0, 512);     //设置x轴区间
//    ui->vangoghHistogram_widget->xAxis->setBasePen(QPen(Qt::black));
//    ui->vangoghHistogram_widget->xAxis->setTickPen(QPen(Qt::black));
//    ui->vangoghHistogram_widget->xAxis->grid()->setVisible(true);//设置网格是否显示
//    ui->vangoghHistogram_widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    ui->vangoghHistogram_widget->xAxis->setTickLabelColor(Qt::black);//设置标记标签颜色
//    ui->vangoghHistogram_widget->xAxis->setLabelColor(Qt::black);
//    // prepare y axis: //设置y轴
//    ui->vangoghHistogram_widget->yAxis->setRange(0, 100);
//    ui->vangoghHistogram_widget->yAxis->setPadding(5); // a bit more space to the left border 设置左边留空间
//    //    ui->Histogram_widget->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
//    ui->vangoghHistogram_widget->yAxis->setBasePen(QPen(Qt::black));
//    ui->vangoghHistogram_widget->yAxis->setTickPen(QPen(Qt::black));
//    ui->vangoghHistogram_widget->yAxis->setSubTickPen(QPen(Qt::black));//设置SubTick颜色，SubTick指的是轴上的
//    //刻度线
//    ui->vangoghHistogram_widget->yAxis->grid()->setSubGridVisible(true);
//    ui->vangoghHistogram_widget->yAxis->setTickLabelColor(Qt::black);//设置标记标签颜色（y轴标记标签）
//    ui->vangoghHistogram_widget->yAxis->setLabelColor(Qt::black);//设置标签颜色（y轴右边标签）
//    ui->vangoghHistogram_widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
//    ui->vangoghHistogram_widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    // setup legend: 设置标签
//    //    ui->Histogram_widget->legend->setVisible(true);
//    ui->vangoghHistogram_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
//    ui->vangoghHistogram_widget->legend->setBrush(QColor(255, 255, 255, 100));
//    ui->vangoghHistogram_widget->legend->setBorderPen(Qt::NoPen);
//    QFont legendFont = font();
//    legendFont.setPointSize(7);
//    ui->vangoghHistogram_widget->legend->setFont(legendFont);
//    ui->vangoghHistogram_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//设置 可拖动，可放大缩
//    //初始化MA曲线显示相关
//    ui->vangoghHistogram_widget->legend->setVisible(true);




//    //cassatt
//    cassatt_HistorgramLabels.resize(binSize);
//    cassatt_HistorgramTicks.resize(binSize);
//    for(int i=0; i<binSize; i++)
//    {
//        cassatt_HistorgramTicks[i] = i;
//        cassatt_HistorgramLabels[i] = "";
//        if(0 == i%10)
//        {
//            cassatt_HistorgramLabels[i] = QString::number(i);
//        }
//    }
//    regen_cassatt = new QCPBars(ui->cassatHistogram_widget->xAxis,ui->cassatHistogram_widget->yAxis);
//    regen_cassatt->setAntialiased(false);
//    regen_cassatt->setStackingGap(2);
//    regen_cassatt->setName("Histogram");
//    regen_cassatt->setPen(QPen(QColor(0, 168, 140).lighter(130)));
//    regen_cassatt->setBrush(QColor(0, 168, 140));
//    ui->cassatHistogram_widget->xAxis->setTickLabelRotation(60);//设置标签角度旋转
//    ui->cassatHistogram_widget->xAxis->setSubTicks(false);//设置是否显示子标签
//    ui->cassatHistogram_widget->xAxis->setTickLength(0, 4);
//    ui->cassatHistogram_widget->xAxis->setRange(0, 512);     //设置x轴区间
//    ui->cassatHistogram_widget->xAxis->setBasePen(QPen(Qt::black));
//    ui->cassatHistogram_widget->xAxis->setTickPen(QPen(Qt::black));
//    ui->cassatHistogram_widget->xAxis->grid()->setVisible(true);//设置网格是否显示
//    ui->cassatHistogram_widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    ui->cassatHistogram_widget->xAxis->setTickLabelColor(Qt::black);//设置标记标签颜色
//    ui->cassatHistogram_widget->xAxis->setLabelColor(Qt::black);
//    // prepare y axis: //设置y轴
//    ui->cassatHistogram_widget->yAxis->setRange(0, 100);
//    ui->cassatHistogram_widget->yAxis->setPadding(5); // a bit more space to the left border 设置左边留空间
//    //    ui->Histogram_widget->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
//    ui->cassatHistogram_widget->yAxis->setBasePen(QPen(Qt::black));
//    ui->cassatHistogram_widget->yAxis->setTickPen(QPen(Qt::black));
//    ui->cassatHistogram_widget->yAxis->setSubTickPen(QPen(Qt::black));//设置SubTick颜色，SubTick指的是轴上的
//    //刻度线
//    ui->cassatHistogram_widget->yAxis->grid()->setSubGridVisible(true);
//    ui->cassatHistogram_widget->yAxis->setTickLabelColor(Qt::black);//设置标记标签颜色（y轴标记标签）
//    ui->cassatHistogram_widget->yAxis->setLabelColor(Qt::black);//设置标签颜色（y轴右边标签）
//    ui->cassatHistogram_widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
//    ui->cassatHistogram_widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    // setup legend: 设置标签
//    ui->Histogram_widget->legend->setVisible(true);
//    ui->cassatHistogram_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
//    ui->cassatHistogram_widget->legend->setBrush(QColor(255, 255, 255, 100));
//    ui->cassatHistogram_widget->legend->setBorderPen(Qt::NoPen);
//    legendFont = font();
//    legendFont.setPointSize(7);
//    ui->cassatHistogram_widget->legend->setFont(legendFont);
//    ui->cassatHistogram_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//设置 可拖动，可放大缩
//    //初始化MA曲线显示相关
//    ui->cassatHistogram_widget->legend->setVisible(true);
//    ui->cassatHistogram_widget->addGraph();
//    ui->cassatHistogram_widget->graph(0)->setName(QStringLiteral("MA"));
//    ui->cassatHistogram_widget->graph(0)->setPen(QPen(QColor(255, 0, 0).lighter(130)));




//    //4300
//    vi4300_HistorgramLabels.resize(binSize);
//    vi4300_HistorgramTicks.resize(binSize);
//    for(int i=0; i<binSize; i++)
//    {
//        vi4300_HistorgramTicks[i] = i;
//        vi4300_HistorgramLabels[i] = "";
//        if(0 == i%10)
//        {
//            vi4300_HistorgramLabels[i] = QString::number(i);
//        }
//    }
//    regen_4300 = new QCPBars(ui->VI4300Histogram_widget->xAxis,ui->VI4300Histogram_widget->yAxis);
//    regen_4300->setAntialiased(false);
//    regen_4300->setStackingGap(2);
//    regen_4300->setName("Histogram");
//    regen_4300->setPen(QPen(QColor(0, 168, 140).lighter(130)));
//    regen_4300->setBrush(QColor(0, 168, 140));
//    ui->VI4300Histogram_widget->xAxis->setTickLabelRotation(60);//设置标签角度旋转
//    ui->VI4300Histogram_widget->xAxis->setSubTicks(false);//设置是否显示子标签
//    ui->VI4300Histogram_widget->xAxis->setTickLength(0, 4);
//    ui->VI4300Histogram_widget->xAxis->setRange(0, 512);     //设置x轴区间
//    ui->VI4300Histogram_widget->xAxis->setBasePen(QPen(Qt::black));
//    ui->VI4300Histogram_widget->xAxis->setTickPen(QPen(Qt::black));
//    ui->VI4300Histogram_widget->xAxis->grid()->setVisible(true);//设置网格是否显示
//    ui->VI4300Histogram_widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    ui->VI4300Histogram_widget->xAxis->setTickLabelColor(Qt::black);//设置标记标签颜色
//    ui->VI4300Histogram_widget->xAxis->setLabelColor(Qt::black);
//    // prepare y axis: //设置y轴
//    ui->VI4300Histogram_widget->yAxis->setRange(0, 100);
//    ui->VI4300Histogram_widget->yAxis->setPadding(5); // a bit more space to the left border 设置左边留空间
//    //    ui->Histogram_widget->yAxis->setLabel("Power Consumption in\nKilowatts per Capita (2007)");
//    ui->VI4300Histogram_widget->yAxis->setBasePen(QPen(Qt::black));
//    ui->VI4300Histogram_widget->yAxis->setTickPen(QPen(Qt::black));
//    ui->VI4300Histogram_widget->yAxis->setSubTickPen(QPen(Qt::black));//设置SubTick颜色，SubTick指的是轴上的
//    //刻度线
//    ui->VI4300Histogram_widget->yAxis->grid()->setSubGridVisible(true);
//    ui->VI4300Histogram_widget->yAxis->setTickLabelColor(Qt::black);//设置标记标签颜色（y轴标记标签）
//    ui->VI4300Histogram_widget->yAxis->setLabelColor(Qt::black);//设置标签颜色（y轴右边标签）
//    ui->VI4300Histogram_widget->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
//    ui->VI4300Histogram_widget->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
//    // setup legend: 设置标签
//    ui->Histogram_widget->legend->setVisible(true);
//    ui->VI4300Histogram_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignLeft);
//    ui->VI4300Histogram_widget->legend->setBrush(QColor(255, 255, 255, 100));
//    ui->VI4300Histogram_widget->legend->setBorderPen(Qt::NoPen);
//    legendFont = font();
//    legendFont.setPointSize(7);
//    ui->VI4300Histogram_widget->legend->setFont(legendFont);
//    ui->VI4300Histogram_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//设置 可拖动，可放大缩
//    //初始化MA曲线显示相关
//    ui->VI4300Histogram_widget->legend->setVisible(true);
//    ui->VI4300Histogram_widget->addGraph();
//    ui->VI4300Histogram_widget->graph(0)->setName(QStringLiteral("MA"));
//    ui->VI4300Histogram_widget->graph(0)->setPen(QPen(QColor(255, 0, 0).lighter(130)));



    ui->vangoghHistogram_widget->legend->setVisible(true);
    ui->vangoghHistogram_widget->xAxis->setRange(0,512);
    ui->vangoghHistogram_widget->xAxis->setLabel("time");
    ui->vangoghHistogram_widget->yAxis->setLabel("number");

    ui->vangoghHistogram_widget->xAxis->grid()->setVisible(true);//设置网格是否显示
    ui->vangoghHistogram_widget->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
    ui->vangoghHistogram_widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);//设置 可拖动，可放大缩
    ui->vangoghHistogram_widget->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);

    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();
    ui->vangoghHistogram_widget->addGraph();

    ui->vangoghHistogram_widget->graph(0)->setName("histogram0");

    ui->vangoghHistogram_widget->graph(0)->setPen(QPen(Qt::red));
    ui->vangoghHistogram_widget->graph(1)->setPen(QPen(QColor(80, 100, 80)));
    ui->vangoghHistogram_widget->graph(2)->setPen(QPen(QColor(200, 200, 200)));
    ui->vangoghHistogram_widget->graph(3)->setPen(QPen(QColor(120, 20, 150)));
    ui->vangoghHistogram_widget->graph(4)->setPen(QPen(QColor(250, 100, 200)));
    ui->vangoghHistogram_widget->graph(5)->setPen(QPen(QColor(100, 200, 250)));
    ui->vangoghHistogram_widget->graph(6)->setPen(QPen(QColor(200, 250, 100)));
    ui->vangoghHistogram_widget->graph(7)->setPen(QPen(QColor(0, 0, 250)));
    ui->vangoghHistogram_widget->graph(8)->setPen(QPen(QColor(150, 150, 0)));
    ui->vangoghHistogram_widget->graph(9)->setPen(QPen(QColor(0, 150, 150)));

    QString strName;
    for(int i=0; i<10; i++)
    {
        strName = "Histogram" + QString::number(i);
        ui->vangoghHistogram_widget->graph(i)->setName(strName);
    }




}


cassatHistogram_Dialog::~cassatHistogram_Dialog()
{
    delete ui;
}

/**
*@date 2020-04-30
*@brief: vangogh 直方图
*/
void cassatHistogram_Dialog::toShowVangogh_histogramSlot(int histogramIndex,QVector<double> numData,int yMax)   //显示梵高的直方图
{


    if(histogramIndex<0 || histogramIndex>9)
    {
        qDebug()<<"histogramIndex error ";
        return;
    }

    QVector<double> zero_vec(512);
    ui->vangoghHistogram_widget->xAxis->setRange(0,binSize);
    ui->vangoghHistogram_widget->yAxis->setRange(0,yMax);
    ui->vangoghHistogram_widget->graph(histogramIndex)->setData(vangogh_HistorgramTicks,numData);

//    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
//    textTicker->setTicks(vangogh_HistorgramTicks, vangogh_HistorgramLabels);
//    ui->vangoghHistogram_widget->xAxis->setTicker(textTicker);
//    ui->vangoghHistogram_widget->xAxis->setRange(0,binSize);
//    ui->vangoghHistogram_widget->yAxis->setRange(0,yMax);
//    regen_vangogh[histogramIndex]->setData(vangogh_HistorgramTicks, numData);        //只不过第一个向量xTicks的每个元素表示“第几个柱子”，然后后面对应的values表示对应“柱子的值”

    //文件保存相关
    if(isSaveFileFlag)
    {
        for(int i=0; i<numData.size(); i++)
        {
            saveText[histogramIndex].append(QString::number(numData[i])).append(" ");
        }
        saveText[histogramIndex].append("\n");
    }






    if(readAll_flag)   //读全部
    {
        if(histogramIndex<lasthistogramIndex)    //一帧已经接收完毕
        {
            ui->vangoghHistogram_widget->replot();   //全部接收完毕以后 刷新直方图


            if(isSaveFileFlag)
            {
                for(int i=0;i<10;i++)
                {
                    saveFileText.append(saveText[i]);
                    saveText[i].clear();

                }
                QString fileName = ui->filePath_lineEdit->text() + "all"+QString::number(saveFile_index)+".txt";
                writeTxt_slot(fileName,saveFileText);
                saveFileText.clear();
                saveFile_index++;

            }



        sendCmd_slot();
        }


    }else   //读单个
    {
        for(int i=0; i<10; i++)     //清空别的控件的数据
        {
            if(i!=histogramIndex)
            {
//               regen_vangogh[i]->setData(vangogh_HistorgramTicks, zero_vec);
                ui->vangoghHistogram_widget->graph(i)->setData(vangogh_HistorgramTicks,zero_vec);
            }
        }
        ui->vangoghHistogram_widget->replot();   //刷新
        if(isSaveFileFlag)
        {
            for(int i=0;i<10;i++)
            {
                saveFileText.append(saveText[i]);
                saveText[i].clear();

            }
            QString fileName = ui->filePath_lineEdit->text() + "TDC"+ QString::number(histogramIndex) +"_" + QString::number(saveFile_index)+".txt";
            writeTxt_slot(fileName,saveFileText);
            saveFileText.clear();
            saveFile_index++;
        }

        sendCmd_slot();
    }
    lasthistogramIndex = histogramIndex;
}


void cassatHistogram_Dialog::sendCmd_slot()
{
    int tdcNumber = ui->TDC_number_comboBox->currentIndex();
    if(11 == tdcNumber)
    {
        readAll_flag = true;
        QString cmdStr = "5A 00 02 00 57 F2";
        emit sendSerialSignal(cmdStr);
    }
    if(10 == tdcNumber)  //读取全部
    {
        readAll_flag = true;
        QString cmdStr = "5A 00 02 00 57 F1";
        emit sendSerialSignal(cmdStr);

    }else
    {
        readAll_flag = false;
        QString cmdStr = "5A 00 02 00 57 ";
        QString tdcNumStr = QString("%1").arg(tdcNumber,2,16,QChar('0'));
        cmdStr.append(tdcNumStr);
        emit sendSerialSignal(cmdStr);
    }
    ui->read_vangogh_Histogram_pushButton->setText(QStringLiteral("暂停"));
}







void cassatHistogram_Dialog::on_pushButton_clicked()
{
    QVector<double> vec[10];

    for(int k=0; k<10; k++)
    {
        vec[k].resize(binSize);
    }



    for(int i=0;i<binSize;i++)
    {
        vec[0][i] = i;
        vec[1][i] = i + i%2;
        vec[2][i] = i - i%2;
        vec[3][i] = i +1;
        vec[4][i] = i-1;
        vec[5][i] = i+2;

    }



    int yMax = binSize;

    toShowVangogh_histogramSlot(0,vec[0],yMax);
    toShowVangogh_histogramSlot(1,vec[1],yMax);
    toShowVangogh_histogramSlot(2,vec[2],yMax);
    toShowVangogh_histogramSlot(3,vec[3],yMax);
    toShowVangogh_histogramSlot(4,vec[4],yMax);
    toShowVangogh_histogramSlot(5,vec[5],yMax);


}


//!
//! \brief cassatHistogram_Dialog::on_read_vangogh_Histogram_pushButton_clicked
//!读取vangogh直方图
void cassatHistogram_Dialog::on_read_vangogh_Histogram_pushButton_clicked()
{

    if(ui->read_vangogh_Histogram_pushButton->text() == QStringLiteral("读取直方图"))
    {
        int tdcNumber = ui->TDC_number_comboBox->currentIndex();
        if(11 == tdcNumber)   //读取0-8
        {
            readAll_flag = true;    // 读取0-8 页可以这么读取
            QString cmdStr = "5A 00 02 00 57 F2";
            emit sendSerialSignal(cmdStr);
        }
        if(10 == tdcNumber)  //读取全部
        {
            readAll_flag = true;
            QString cmdStr = "5A 00 02 00 57 F1";
            emit sendSerialSignal(cmdStr);

        }else
        {
            readAll_flag = false;
            QString cmdStr = "5A 00 02 00 57 ";
            QString tdcNumStr = QString("%1").arg(tdcNumber,2,16,QChar('0'));
            cmdStr.append(tdcNumStr);
            emit sendSerialSignal(cmdStr);
        }
        ui->read_vangogh_Histogram_pushButton->setText(QStringLiteral("暂停"));
    }else
    {
        QString cmdStr = "5A 00 02 00 57 F0";
        emit sendSerialSignal(cmdStr);
        ui->read_vangogh_Histogram_pushButton->setText(QStringLiteral("读取直方图"));
    }


}

//!定时读取直方图
//! 5A 00 02 00 57 (00-09)
void cassatHistogram_Dialog::vangogh_read_timer_slot()
{
    int tdcNumber = ui->TDC_number_comboBox->currentIndex();
    QString cmdStr = "5A 00 02 00 57 ";
    QString tdcNumStr = QString("%1").arg(tdcNumber,2,16,QChar('0'));
    cmdStr.append(tdcNumStr);
    emit sendSerialSignal(cmdStr);
}


//!
//! \brief cassatHistogram_Dialog::on_saveData_pushButton_clicked
//!保存数据
void cassatHistogram_Dialog::on_saveData_pushButton_clicked()
{

    QString sFilePath = ui->filePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        return;
    }
    //设置文件的保存路径
    if(sFilePath.right(3)=="txt")
    {
        sFilePath = sFilePath.left(sFilePath.length() - 4);
    }
    if(sFilePath.right(1) != "\\")
    {
        sFilePath.append("\\");
    }
    ui->filePath_lineEdit->setText(sFilePath);

    if(ui->saveData_pushButton->text() == QStringLiteral("保存数据"))
    {

        isSaveFileFlag = true;
        saveFile_index = 1;
        ui->saveData_pushButton->setText(QStringLiteral("取消"));
    }else
    {
        isSaveFileFlag= false;
        saveFile_index= 1;
        ui->saveData_pushButton->setText(QStringLiteral("保存数据"));
    }
}

//!
//! \brief cassatHistogram_Dialog::writeTxt_slot
//! \param fileName
//! \param Text
//!保存文件的槽函数
void cassatHistogram_Dialog::writeTxt_slot(QString fileName,QString text)
{


    QFile file(fileName);
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
}
