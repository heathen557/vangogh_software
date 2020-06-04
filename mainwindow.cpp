#include "mainwindow.h"
#include "ui_mainwindow.h"


extern Settings currentSettings;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qRegisterMetaType<vector<float>>("vector<float>");   //注册函数
    qRegisterMetaType<vector<double>>("vector<double>");   //注册函数
    qRegisterMetaType<vector<int>>("vector<int>");       //注册函数
    qRegisterMetaType<QVector<double>>("QVector<double>");   //注册函数
    qRegisterMetaType<QVector<QString>>("QVector<QString>");   //注册函数

    receSerial_Obj = new receSerial_msg;;
    receSerialThread = new QThread;
    receSerial_Obj->moveToThread(receSerialThread);
    receSerialThread->start();

    initSerial();  //串口的初始化
    initConnect();

}

MainWindow::~MainWindow()
{
    delete ui;
}


//!\
//! 信号与槽的连接
void MainWindow::initConnect()
{
    //接收数据线程 接收并处理数据后，将处理结果发送给主线程的信号与槽
    connect(this,&MainWindow::openOrCloseSerial_signal,receSerial_Obj,&receSerial_msg::openOrCloseSerial_slot);
    connect(receSerial_Obj,SIGNAL(returnLinkInfo_signal(QString, bool)),this,SLOT(returnLinkInfo_slot(QString, bool)));
    connect(this,&MainWindow::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_MainWindow_signal,this,&MainWindow::AckCmd_MainWindow_slot);

    //寄存器读写的 信号与槽的连接
    connect(&register_dia,&RegisiterSetDialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmdRegister_signal,&register_dia,&RegisiterSetDialog::AckCmdRegister_slot);
    connect(&register_dia,&RegisiterSetDialog::Display_log_signal,this,&MainWindow::Display_log_slot);

    //测距界面 信号与槽的连接
    connect(&distanceTest_dia,&distanceTest_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::toShow_vangogh_ResultMsg_signal,&distanceTest_dia,&distanceTest_Dialog::toShow_vangogh_ResultMsg_slot);
    connect(&distanceTest_dia,&distanceTest_Dialog::Display_log_signal,this,&MainWindow::Display_log_slot);
    connect(receSerial_Obj,&receSerial_msg::toSendStatistic_signal,&distanceTest_dia,&distanceTest_Dialog::toSendStatistic_slot);

    //RowData相关
    connect(&RawData_dia,&RowData_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_rawDataTest_signal,&RawData_dia,&RowData_Dialog::AckCmd_rawDataTest_slot);
    connect(&RawData_dia,&RowData_Dialog::Display_log_signal,this,&MainWindow::Display_log_slot);

    //histogram相关
    connect(&cassatHistogram_dia,&cassatHistogram_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::toShowVangogh_histogramSignal,&cassatHistogram_dia,&cassatHistogram_Dialog::toShowVangogh_histogramSlot);
//    connect(receSerial_Obj,&receSerial_msg::toShowCassatt_histogramSignal,&cassatHistogram_dia,&cassatHistogram_Dialog::toShowCassatt_histogramSlot);
//    connect(receSerial_Obj,&receSerial_msg::toShow4300_histogramSignal,&cassatHistogram_dia,&cassatHistogram_Dialog::toShow4300_histogramSlot);


    //通道校准系数的设置通道
    connect(&channelPara_dia,&channekParameter_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_channelParameter_signal,&channelPara_dia,&channekParameter_Dialog::AckCmd_channelParameter_slot);

    //自动步进界面的 信号与槽的连接
    connect(&autoStep_dia,&AutoStepping_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_autoStepping_signal,&autoStep_dia,&AutoStepping_Dialog::AckCmd_autoStepping_slot);

    //DCR测试的界面 信号与槽的连接
    connect(&DCR_test_dia,&DCR_test_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_DCRTest_signal,&DCR_test_dia,&DCR_test_Dialog::AckCmd_DCRTest_slot);

    //delayLine 测试的槽函数
    connect(&delayLine_dia,&delayLine_Dialog::sendSerialSignal,receSerial_Obj,&receSerial_msg::sendSerialSlot);
    connect(receSerial_Obj,&receSerial_msg::AckCmd_delayLine_signal,&delayLine_dia,&delayLine_Dialog::AckCmd_delayLine_slot);




}

//!
//! \brief MainWindow::SerialSetting_Enable_true
//! 使能串口的标签
void MainWindow::SerialSetting_Enable_true()
{
    ui->serialPortInfoListBox->setEnabled(true);
    ui->baudRateBox->setEnabled(true);
    ui->dataBitsBox->setEnabled(true);
    ui->parityBox->setEnabled(true);
    ui->stopBitsBox->setEnabled(true);
}

//!
//! \brief MainWindow::SerialSetting_Enable_false
//!
void MainWindow::SerialSetting_Enable_false()
{
    ui->serialPortInfoListBox->setEnabled(false);
    ui->baudRateBox->setEnabled(false);
    ui->dataBitsBox->setEnabled(false);
    ui->parityBox->setEnabled(false);
    ui->stopBitsBox->setEnabled(false);
}



//!
//! \brief MainWindow::initSerial
//!串口的初始化
void  MainWindow::initSerial()
{
    QFile file("setting.ini");
    QByteArray temp("\r\n");
    QString line[20];

    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        int i = 0;
        while (!in.atEnd())
        {
            line[i] = in.readLine();
            i++;
        }
        file.close();
    }
    int numSeri_ = line[0].toInt();       //串口号
    int baudRateBox_ = line[1].toInt();   //波特率

    int num = 0;
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        num++;
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    ui->serialPortInfoListBox->clear();
    ui->serialPortInfoListBox->addItems(m_serialPortName);
    if(numSeri_>num)
        ui->serialPortInfoListBox->setCurrentIndex(0);
    else
        ui->serialPortInfoListBox->setCurrentIndex(numSeri_);



    ui->baudRateBox->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateBox->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateBox->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateBox->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);
    ui->baudRateBox->addItem(QStringLiteral("230400"), QSerialPort::Baud230400);
    ui->baudRateBox->addItem(QStringLiteral("256000"), QSerialPort::Baud256000);
    ui->baudRateBox->addItem(QStringLiteral("460800"), QSerialPort::Baud460800);

    ui->baudRateBox->addItem(tr("Custom"));
    ui->baudRateBox->setCurrentIndex(baudRateBox_);

    ui->dataBitsBox->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsBox->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsBox->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsBox->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsBox->setCurrentIndex(3);

    ui->parityBox->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityBox->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityBox->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityBox->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityBox->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsBox->addItem(QStringLiteral("1"), QSerialPort::OneStop);
#ifdef Q_OS_WIN
    ui->stopBitsBox->addItem(tr("1.5"), QSerialPort::OneAndHalfStop);
#endif
    ui->stopBitsBox->addItem(QStringLiteral("2"), QSerialPort::TwoStop);
}

void MainWindow::on_portScan_pushButton_clicked()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    ui->serialPortInfoListBox->clear();
    ui->serialPortInfoListBox->addItems(m_serialPortName);
    QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("可用端口检测完毕！"));
}


//!
//! \brief MainWindow::on_openPort_pushButton_clicked
//! 打开串口
void MainWindow::on_openPort_pushButton_clicked()
{
    if(ui->openPort_pushButton->text() == "Open")
    {
        currentSettings.name = ui->serialPortInfoListBox->currentText();

        if (ui->baudRateBox->currentIndex() == 4) {
            currentSettings.baudRate = ui->baudRateBox->currentText().toInt();
        } else {
            currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                        ui->baudRateBox->itemData(ui->baudRateBox->currentIndex()).toInt());
        }


        qDebug()<<" currentIndex ="<<ui->baudRateBox->currentIndex() <<" currentSettings.baudRate = "<<currentSettings.baudRate;
        currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

        currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                    ui->dataBitsBox->itemData(ui->dataBitsBox->currentIndex()).toInt());
        currentSettings.stringDataBits = ui->dataBitsBox->currentText();

        currentSettings.parity = static_cast<QSerialPort::Parity>(
                    ui->parityBox->itemData(ui->parityBox->currentIndex()).toInt());
        currentSettings.stringParity = ui->parityBox->currentText();

        currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                    ui->stopBitsBox->itemData(ui->stopBitsBox->currentIndex()).toInt());
        currentSettings.stringStopBits = ui->stopBitsBox->currentText();

//         qDebug()<<"name="<<currentSettings.name<<" baudRate ="<<currentSettings.baudRate<<" dataBits="<<currentSettings.dataBits<<" parity="<<currentSettings.parity<<" stopBits="<<currentSettings.stopBits<<" flowCon"<<currentSettings.flowControl;


        emit openOrCloseSerial_signal(true);

        qDebug()<<"name="<<currentSettings.name<<" baudRate ="<<currentSettings.baudRate<<" dataBits="<<currentSettings.dataBits<<" parity="<<currentSettings.parity<<" stopBits="<<currentSettings.stopBits<<" flowCon"<<currentSettings.flowControl;


        QFile file("setting.ini");
        QByteArray temp("\r\n");
        QString line[20];

        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            int i = 0;
            while (!in.atEnd())
            {
                line[i] = in.readLine();
                i++;
            }
            file.close();
        }

        int seriNum = ui->serialPortInfoListBox->currentIndex();
        int baudBox = ui->baudRateBox->currentIndex();


        if(file.open(QIODevice::WriteOnly))
        {
            QByteArray writeData;
            writeData = QString::number(seriNum).toLatin1()+ temp + QString::number(baudBox).toLatin1()+temp+\
                    line[2].toLatin1()+ temp +line[3].toLatin1()+ temp+ line[4].toLatin1()+ temp +line[5].toLatin1();
            if (-1 == file.write(writeData))
            {
                qDebug()<<"ERROR";
            }
            file.close();
        }

    }
    else
    {
        emit openOrCloseSerial_signal(false);

    }
}



//!
//! \brief MainWindow::returnLinkInfo_slot
//! \param str
//! \param flag
//! 串口连接的返回信息
void MainWindow::returnLinkInfo_slot(QString str, bool flag)
{
    if("open" == str)
    {
        if(true == flag)
        {
             isLinked = true;
             ui->openPort_pushButton->setText("Close");
             SerialSetting_Enable_false();

             QString logStr = "[Open SerialPort Suceess!]";
             Display_log_slot(logStr);
        }else
        {
            QString logStr = "[Open SerialPort Failed!]";
            Display_log_slot(logStr);
            QMessageBox::critical(this, QStringLiteral("告警"), QStringLiteral("打开串口失败！"));
        }
    }else
    {
        if(true == flag)
        {
            isLinked = false;
            ui->openPort_pushButton->setText("Open");
            SerialSetting_Enable_true();
            QString logStr = "[Close SerialPort Suceess!]";
            Display_log_slot(logStr);

        }else
        {
            QString logStr = "[Close SerialPort Failed!]";
            Display_log_slot(logStr);
            QMessageBox::critical(this, QStringLiteral("告警"), QStringLiteral("关闭串口失败！"));
        }
    }
}


/******************以上为串口相关***********************************/
/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief:  显示日志信息的槽函数
*/
void MainWindow::Display_log_slot(QString str)  //打印日志信息到控制信息输出窗口的槽函数
{
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy-MM-dd");
    QString current_time = current_date_time.toString("hh:mm:ss.zzz ");
    str = current_time + str;

    ui->control_log_textEdit->appendPlainText(str);
}


//!
//! \brief MainWindow::on_Register_action_triggered
//!寄存器配置界面
void MainWindow::on_Register_action_triggered()
{

//    register_dia.setModal(true);
    register_dia.showMaximized();
}


//!
//! \brief MainWindow::on_CH_IA_import_pushButton_clicked
//!电源通道 导入的槽函数
void MainWindow::on_CH_IA_import_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_CH_IA_export_pushButton_clicked
//!电源通道 导出的 槽函数
void MainWindow::on_CH_IA_export_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_electricityTest_pushButton_clicked
//!电学测试 槽函数
void MainWindow::on_electricityTest_pushButton_clicked()
{

}



//!
//! \brief MainWindow::on_vango_timeCheck_pushButton_clicked
//!
void MainWindow::on_vango_timeCheck_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_vango_vspadCheck_pushButton_clicked
//!
void MainWindow::on_vango_vspadCheck_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_vango_MCU_test_pushButton_clicked
//!
void MainWindow::on_vango_MCU_test_pushButton_clicked()
{

}

/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief:  测距的界面
*/
void MainWindow::on_distance_test_pushButton_clicked()
{
//    distanceTest_dia.setModal(true);
    distanceTest_dia.show();
}



//!
//! \brief MainWindow::on_inteface_import_pushButton_clicked
//!
void MainWindow::on_inteface_import_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_inteface_export_pushButton_clicked
//!
void MainWindow::on_inteface_export_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_register_import_pushButton_clicked
//!
void MainWindow::on_register_import_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_register_export_pushButton_clicked
//!
void MainWindow::on_register_export_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_register_I2C_write_pushButton_clicked
//!
void MainWindow::on_register_I2C_write_pushButton_clicked()
{

}
//!
//! \brief MainWindow::on_register_SPI_write_pushButton_clicked
//!
void MainWindow::on_register_SPI_write_pushButton_clicked()
{

}

/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief:
* 1、读取本地文件
*/
void MainWindow::on_vangoFW_select_pushButton_clicked()
{
    QString fileName=QFileDialog::getOpenFileName();
    qDebug() << "filename = "<<fileName;

    if(fileName.isEmpty())
    {
        QMessageBox::warning(NULL,QStringLiteral("提示"),QStringLiteral("路径不能为空！"));
        return;
    }
    ui->FW_filePath_lineEdit->setText(fileName);


}
/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief:
* 1、读取本地配置文件
* 2、写入到MCU文件
* 3、包号：00-F1：  每次最多1024个字节
* 5A 01 NN NN 56 dd DD DD ... DD DD
*/
void MainWindow::on_vangoFW_write_pushButton_clicked()
{
    fwBin_stringList.clear();

    QString fwFileName = ui->FW_filePath_lineEdit->text();
    if(fwFileName.isEmpty())
    {
        qDebug()<<QStringLiteral("加载的文件路径不能为空");
        return;
    }
    QFile file(fwFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(NULL,QStringLiteral("提示"),QStringLiteral("打开文件失败！"));
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_7);
    int index =1;
    QString TextStr;
    while(!in.atEnd())
    {
        quint8 dataByte;
        in>>dataByte;
        TextStr.append(QString("%1").arg(dataByte,2,16,QChar('0')));
        index++;
    }

//   1024个字节 对应 2048个QString, 分组

   // 1 去掉空格
   TextStr = TextStr.replace(" ","");

   // 2 获取长度
   int allLen = TextStr.length();
   qDebug()<<"allLen = "<<allLen;

   // 3 分包的个数
   int allPackageNum = 0;
   if(0 == allLen%2048)
   {
       allPackageNum = allLen/2048;
   }else
   {
       allPackageNum = allLen/2048 +1;
   }

   // 4 按照协议分包发送
   // 5A 01 NN NN 56 dd DD DD ... DD DD
   QString cmdStr="5A 01 00 00 56 ";
   QString packIndexStr;
   QString dataStr;
   for(int i=0; i<allPackageNum; i++)
   {
       packIndexStr = QString("%1").arg(i,2,16,QChar('0'));
       qDebug()<<"packIndexStr = "<<packIndexStr;

       if(i == allPackageNum-1)  //最后一包数据
       {
           dataStr =TextStr.right(allLen - i*2048);
       }else
       {
           dataStr = TextStr.mid(i*2048,2048);
       }

       cmdStr.append(packIndexStr).append(dataStr);
       fwBin_stringList.append(cmdStr);      //把命令都封装到fwBin_stringList当中
       cmdStr.clear();
       cmdStr="5A 01 00 00 56 ";

   }



   QString toSendCmd = fwBin_stringList[0];
   fwBin_stringList.removeAt(0);
   emit sendSerialSignal(toSendCmd);


}

/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief: rowData窗口的界面
*/
void MainWindow::on_RawData_dia_pushButton_clicked()
{
//    RawData_dia.setModal(true);
    RawData_dia.show();
}


void MainWindow::on_vango_Histogram_pushButton_clicked()
{
//    cassatHistogram_dia.setModal(true);
    cassatHistogram_dia.show();
}


//!
//! \brief MainWindow::on_channelParametre_pushButton_clicked
//!测试通道校准系数
void MainWindow::on_channelParametre_pushButton_clicked()
{
//    channelPara_dia.setModal(true);
    channelPara_dia.show();
}

//!
//! \brief MainWindow::on_chipZhongduanShineng_checkBox_clicked
//!芯片中断使能槽函数
//! 使能：5A 01 02 00 44 00
//! 失能：5A 01 02 00 44 01
void MainWindow::on_chipZhongduanShineng_checkBox_clicked()
{
    if(ui->chipZhongduanShineng_checkBox->isChecked())
    {
        QString cmdStr = "5A 01 02 00 44 00";
        emit sendSerialSignal(cmdStr);
    }else
    {
        QString cmdStr = "5A 01 02 00 44 01";
        emit sendSerialSignal(cmdStr);
    }
}
//!
//! \brief MainWindow::on_ChipShineng_checkBox_clicked
//!芯片使能的槽函数
//! 使能：5A 01 02 00 46 00
//! 失能：5A 01 02 00 46 01
void MainWindow::on_ChipShineng_checkBox_clicked()
{
    if(ui->ChipShineng_checkBox->isChecked())
    {
        QString cmdStr = "5A 01 02 00 46 00";
        emit sendSerialSignal(cmdStr);
    }else
    {
        QString cmdStr = "5A 01 02 00 46 01";
        emit sendSerialSignal(cmdStr);
    }
}

//!
//! \brief MainWindow::on_ChipFuwei_pushButton_clicked
//!芯片复位的槽函数
//!  5A 01 01 00 45
void MainWindow::on_ChipFuwei_pushButton_clicked()
{
    QString cmdStr = "5A 01 01 00 45";
    emit sendSerialSignal(cmdStr);
}

/*****************************写继电器的相关的槽函数****************************************/
//!
//! \brief MainWindow::on_zongdianyuan_in_radioButton_clicked
//!总电源关 5A 01 02 00 40 00 00
void MainWindow::on_zongdianyuan_close_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 00 00";
    emit sendSerialSignal(cmdStr);
}
//!
//! \brief MainWindow::on_zongdianyuan_open_radioButton_clicked
//!总电源开 5A 01 02 00 40 00 01
void MainWindow::on_zongdianyuan_open_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 00 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH1_in_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 01 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH1_out_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 01 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH2_in_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 02 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH2_out_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 02 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH3_in_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 03 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH3_out_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 03 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH4_in_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 04 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH4_out_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 04 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH5_in_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 05 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_CH5_out_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 05 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_I2C_MCU_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 06 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_I2C_outer_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 06 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_SPI_master_MCU_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 07 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_SPI_master_outer_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 07 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_SPI_slave_MCU_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 08 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_SPI_slave_outer_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 08 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_interuptReser_MCU_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 09 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_interuptReser_outer_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 09 01";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_time_banzai_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 0A 00";
    emit sendSerialSignal(cmdStr);
}

void MainWindow::on_time_outer_radioButton_clicked()
{
    QString cmdStr = "5A 01 02 00 40 0A 01";
    emit sendSerialSignal(cmdStr);
}


//!
//! \brief MainWindow::AckCmd_MainWindow_slot
//! \param returnCmd
//! \param AckInfo
//! 返回信息的槽函数
//!
//!  芯片中断使能设置返回：8144   none
//! 芯片复位完成返回：     8145    none
//! 芯片使能完成返回       8146    none
//! MCU 主动上传状态       8047    具体的状态信息
//! 下载固件               8156    F0：传输完成  F1：传输中止  其他正在传输
void MainWindow::AckCmd_MainWindow_slot(QString returnCmd,QString AckInfo)
{
    if("8144" == returnCmd)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("芯片中断使能设置成功!"));
    }else if("8145" == returnCmd)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("芯片复位成功!"));
    }else if("8146" == returnCmd)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("芯片使能设置成功!"));
    }else if("8047" == returnCmd)
    {
        int len = AckInfo.length();

        int chipShiNeng = AckInfo.mid(0,2).toInt(NULL,16);
        int chipZhongduanShiNeng = AckInfo.mid(2,2).toInt(NULL,16);
        int jidianqi_status[11];
        float voltage[11];
        int IA[11];

        QString tmpStr;
        QString jidianqiStatusStr;
        QString voltageStr;
        QString voltageResStr;
        QString IA_str;
        QString IA_res_str;

        for(int i=0; i<11; i++)
        {
            tmpStr = AckInfo.mid(4+i*2,2);
            jidianqi_status[i] = tmpStr.toInt(NULL,16);
        }

        for(int i=0; i<5; i++)
        {
            voltageStr = AckInfo.mid(26+i*8,8);
            voltageResStr = voltageStr.mid(6,2)+voltageStr.mid(4,2)+voltageStr.mid(2,2) + voltageStr.mid(0,2);
            voltage[i] = voltageResStr.toInt(NULL,16)/1000.0;
        }

        for(int i=0; i<5;i++)
        {
            IA_str = AckInfo.mid(66+i*8,8);
            IA_res_str = IA_str.mid(6,2)+IA_str.mid(4,2)+IA_str.mid(2,2)+IA_str.mid(0,2);
            IA[i] = IA_res_str.toInt(NULL,16)/1000.0;

        }


        //1 芯片使能
        if(0 == chipShiNeng)
        {
            ui->ChipShineng_checkBox->setChecked(true);
        }else if(1 == chipShiNeng)  //芯片失能
        {
            ui->ChipShineng_checkBox->setChecked(false);
        }

        // 2 芯片中断使能
        if(0 == chipZhongduanShiNeng)
        {
            ui->chipZhongduanShineng_checkBox->setChecked(true);
        }else if(1 == chipZhongduanShiNeng)
        {
            ui->chipZhongduanShineng_checkBox->setChecked(false);
        }

        //3 总电源 继电器状态 与电压
        if(0 == jidianqi_status[0] )
        {
            ui->zongdianyuan_close_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[0])
        {
            ui->zongdianyuan_open_radioButton->setChecked(true);
        }
//        ui->zongDianyuan_voltage_lineEdit->setText(QString::number(voltage[0]));
//        ui->zongDianyuan_IA_lineEdit->setText(QString::number(IA[0]));


        //4、 芯片电源1 的状态 电压、电流
        if(0 ==  jidianqi_status[1])
        {
            ui->CH1_out_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[1])
        {
            ui->CH1_in_radioButton->setChecked(true);
        }
        ui->CH1_voltage_lineEdit->setText(QString::number(voltage[0]));
        ui->CH1_IA_lineEdit->setText(QString::number(IA[0]));

        //5 、芯片2
        if(0 ==  jidianqi_status[2])
        {
            ui->CH2_out_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[2])
        {
            ui->CH2_in_radioButton->setChecked(true);
        }
        ui->CH2_voltage_lineEdit->setText(QString::number(voltage[1]));
        ui->CH2_IA_lineEdit->setText(QString::number(IA[1]));

        //6 芯片3
        if(0 ==  jidianqi_status[3])
        {
            ui->CH3_out_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[3])
        {
            ui->CH3_in_radioButton->setChecked(true);
        }
        ui->CH3_voltage_lineEdit->setText(QString::number(voltage[2]));
        ui->CH3_IA_lineEdit->setText(QString::number(IA[2]));

        //7 芯片4
        if(0 ==  jidianqi_status[4])
        {
            ui->CH4_out_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[4])
        {
            ui->CH4_in_radioButton->setChecked(true);
        }
        ui->CH4_voltage_lineEdit->setText(QString::number(voltage[3]));
        ui->CH4_IA_lineEdit->setText(QString::number(IA[3]));

        //8 芯片5
        if(0 ==  jidianqi_status[5])
        {
            ui->CH5_out_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[5])
        {
            ui->CH5_in_radioButton->setChecked(true);
        }
        ui->CH5_voltage_lineEdit->setText(QString::number(voltage[4]));
        ui->CH5_IA_lineEdit->setText(QString::number(IA[4]));

        //9 i2c
        if(0 ==  jidianqi_status[6])
        {
            ui->I2C_MCU_radioButton->setChecked(true);
        }else if(1 == jidianqi_status[6])
        {
            ui->I2C_outer_radioButton->setChecked(true);
        }


        //10 spi master
        if(0 == jidianqi_status[7])
        {
            ui->SPI_master_MCU_radioButton->setChecked(true);
        }else
        {
            ui->SPI_master_outer_radioButton->setChecked(true);
        }

        //11 spi slave
        if(0 == jidianqi_status[8])
        {
            ui->SPI_slave_MCU_radioButton->setChecked(true);
        }else
        {
            ui->SPI_slave_outer_radioButton->setChecked(true);
        }

        //12 中断
        if(0 == jidianqi_status[9])
        {
           ui->interuptReser_MCU_radioButton->setChecked(true);
        }else
        {
           ui->interuptReser_outer_radioButton->setChecked(true);
        }

        // 13时钟输入
        if(0 == jidianqi_status[10])
        {
            ui->time_banzai_radioButton->setChecked(true);
        }else
        {
            ui->time_outer_radioButton->setChecked(true);
        }
    }else if("8156" == returnCmd)  //下载固件的返回命令
    {
        if("F0" == AckInfo)
        {
            QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("固件已经下载完成！"));
            Display_log_slot(QStringLiteral("固件已经下载完成！"));
        }else if("F1" == AckInfo)
        {
            QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("固件下载中止!"));
            Display_log_slot(QStringLiteral("固件下载中止!"));
        }else   //传输过程中的
        {
            // 1 判断是否已经发送完毕,完毕则发送 5A 00 00 00 56 FO
            if(0 == fwBin_stringList.length())
            {
                QString cmdStr = "5A 01 00 00 56 F0";
                emit sendSerialSignal(cmdStr);
            }else
            {
                QString cmdStr = fwBin_stringList[0];
                emit sendSerialSignal(cmdStr);
                fwBin_stringList.removeAt(0);
            }
        }

    }
}


//!
//! \brief MainWindow::on_AutoStepping_pushButton_clicked
//!自动步进窗口
void MainWindow::on_AutoStepping_pushButton_clicked()
{
//    autoStep_dia.setModal(true);
    autoStep_dia.show();
}

void MainWindow::on_Vango_DCR_pushButton_clicked()
{
    DCR_test_dia.show();
}

//delayLine 测试窗口
void MainWindow::on_Vango_Delayline_pushButton_clicked()
{
    delayLine_dia.show();
}
