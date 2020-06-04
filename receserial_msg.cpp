#include "receserial_msg.h"

extern Settings currentSettings;

receSerial_msg::receSerial_msg(QObject *parent) : QObject(parent)
{

    qDebug()<<" the thread begin "<<endl;
    isTranslateFlag = true;
    serial = NULL;
    clearFlag = false;

    vangoghHistogram_512.resize(512);




    /******读取本地的配置参数  confidence 与 Dmax ******/

    QSettings paraSetting("parameters.ini", QSettings::IniFormat);
    C1 = paraSetting.value("confidence_para/C1").toString().toFloat();
    C2 = paraSetting.value("confidence_para/C2").toString().toFloat();
    C3 = paraSetting.value("confidence_para/C3").toString().toFloat();
    R0 = paraSetting.value("confidence_para/R0").toString().toFloat();
    row0 = paraSetting.value("confidence_para/row0").toString().toFloat();
    P0 = paraSetting.value("confidence_para/P0").toString().toFloat();

    IT = paraSetting.value("confidence_para/IT").toString().toFloat();
    IT0 = paraSetting.value("confidence_para/IT0").toString().toFloat();

    qDebug()<<"C1 = "<<C1<<"  C2="<<C2<<" C3="<<C3<<"  R0="<<R0<<"  row0="<<IT<<"  row0="<<row0<<"  p0="<<P0;


}

void receSerial_msg::openOrCloseSerial_slot(bool flag)
{
    if(NULL == serial)
    {
        serial = new QSerialPort(this);
        connect(serial, SIGNAL(readyRead()), this, SLOT(readDataSlot()),Qt::DirectConnection);

    }

    if(true == flag)   //打开串口
    {
        serial->setPortName(currentSettings.name);
        serial->setBaudRate(currentSettings.baudRate);
        serial->setDataBits(currentSettings.dataBits);
        serial->setParity(currentSettings.parity);
        serial->setStopBits(currentSettings.stopBits);
        serial->setFlowControl(currentSettings.flowControl);
        if(serial->open(QIODevice::ReadWrite))
        {
            qDebug()<<"serial open success!!";
            emit returnLinkInfo_signal("open",true);
        }else{
            qDebug()<<"serial open error";
            emit returnLinkInfo_signal("open",false);
        }
    }else              //关闭串口
    {
        serial->close();
        emit returnLinkInfo_signal("close",true);
        m_buffer.clear();
    }

}


//!
//! \brief receSerial_msg::readDataSlot  串口接收数据，并对命令进行解析 （暂时约定传输数据为小端模式）
//!   1、首先接收到字符串 以后把它转换成16进制的字符串类型，本次处理时去掉空格；
//!   2、将接收到的数据添加到成员变量m_buffer中；
//!   3、根据长度字段将单个命令提取出来
//!   4、对单个命令进行解析
//!   5、命令解析完毕后，从m_buffer中剔除到这个命令，并更新totallen
void receSerial_msg::readDataSlot()
{
    QByteArray temp = serial->readAll();
    QString strHex;//16进制数据

    if (!temp.isEmpty())
    {
        QDataStream out(&temp,QIODevice::ReadWrite);
        while (!out.atEnd())
        {
            qint8 outChar=0;
            out>>outChar;
            QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

            if (str.length()>1)
            {
                strHex+=str;
            }
            else
            {
                strHex+="0"+str;
            }
        }
        strHex = strHex.toUpper();
        //        qDebug()<<" strHex = "<<strHex;
        //        return;

        m_buffer.append(strHex);
        totallen = m_buffer.size();

        if(isTranslateFlag)    //转换成十进制的tof和peak进行显示解析
        {
            while(totallen)
            {
                if(totallen <12)   //单个命令最少要有6个字节 所以长度为12       5A 03 00 01 3A XX
                    return;

                int indexOf5A = m_buffer.indexOf("5A",0);
                if(indexOf5A < 0)  //没有找到5A
                {
                    qDebug()<<QStringLiteral("接收数据有误，不存在5A")<<"index ="<<indexOf5A<<"buffer"<<m_buffer<<endl;
                    return;
                }else if(indexOf5A>0)  //第一次的时候前面会有冗余数据，删掉
                {
                    qDebug()<<QStringLiteral("接收数据存在冗余");
                    m_buffer = m_buffer.right(totallen-indexOf5A);
                    totallen = m_buffer.size();
                    if(totallen <12)
                        return;
                }

//                qDebug()<<"m_buffer = "<<m_buffer;
                //以下数据为5A打头数据
                //首先根据长度字段 来提取出整条数据，数据长度不足的话返回
                QString lenStr= m_buffer.mid(6,2) + m_buffer.mid(4,2);
                int dataLen = lenStr.toInt(NULL,16) * 2;       //数据区的长度  这个长度包含一个字节的 地址
                int len = dataLen + 5 * 2;                     //5A 03（命令） 04 00（长度）  + 检验位共5个字节   这个len是单个包的总长度
                if(totallen < len)                            //本次接收不够一个包,返回 等待下次接收
                    return;

                dataLen = dataLen - 2;                         //减去一个字节的 地址   2019-12-10

                //进行和校验
                QString single_Data = m_buffer.left(len);       //single_Data就是单个命令
                if(!msgCheck(single_Data))
                {
                    qDebug()<<QStringLiteral("和校验失败,singleData =")<<single_Data;
                    m_buffer = m_buffer.right(totallen - len);                                                  //一帧处理完毕 减去该帧的长度
                    totallen = m_buffer.size();
                    return;
                }


//                qDebug()<<" receive single_Data = "<<single_Data;

                QString returnCmdStr = single_Data.mid(2,2);   //命令标识
                ////////////////////////////////////////////////////////单个命令处理代码块//////////////////////////////////////////////////////////////////////////////////////////

                // 1、 写测试通道校准系数的 返回命令
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("41" == secCmd)
                    {
                        QString returnAck ="8141";
                        QString AckInfo = "00";
                        emit AckCmd_channelParameter_signal(returnAck,AckInfo);
                    }
                }

                // 2、读取通道校准系数的额返回命令
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("41" == secCmd)
                    {
                        if(dataLen != 3*10*2)
                        {
                            qDebug()<<QStringLiteral("读取通道校准系数时，数据有效去长度不对，dataLen = ")<<dataLen;
                        }
                        QString returnAck = "8041";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_channelParameter_signal(returnAck,AckInfo);
                    }
                }

                // 3、芯片中断使能 返回
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("44" == secCmd)
                    {
                        QString returnAck = "8144";
                        QString AckInfo = "00";      //无效
                        emit AckCmd_MainWindow_signal(returnAck,AckInfo);
                    }

                }

                // 4、芯片复位 返回
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("45" == secCmd)
                    {
                        QString returnAck = "8145";
                        QString AckInfo = "00";
                        emit AckCmd_MainWindow_signal(returnAck,AckInfo);
                    }
                }

                // 5、芯片使能 返回
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("46" == secCmd)
                    {
                        QString returnAck = "8146";
                        QString AckInfo = "00";
                        emit AckCmd_MainWindow_signal(returnAck,AckInfo);
                    }

                }


                //6、主动上传 状态信息  1s一次
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("47" == secCmd)
                    {
                        if(dataLen != (2 + 11 + 4*5 +4*5)*2)
                        {
                            qDebug()<<QStringLiteral("读取主动上传状态时，数据有效去长度不对，dataLen = ")<<dataLen;
                        }
                        QString returnAck = "8047";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_MainWindow_signal(returnAck,AckInfo);
                    }
                }

                // 7、写寄存器 返回信息
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("01" == secCmd)
                    {
                        QString returnAck = "8101";
                        QString AckInfo = "00";
                        emit AckCmdRegister_signal(returnAck,AckInfo);
                    }
                }

                //8、读寄存器 的返回信息
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("01" == secCmd)
                    {
                        QString returnAck = "8001";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmdRegister_signal(returnAck,AckInfo);
                    }
                }


                //9 下载固件的返回指令
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("56" == secCmd)
                    {
                        QString returnAck = "8156";
                        //获取包的序号   F0:传输万恒 F1：传输中止 :其他：正在传输中
                        QString AckInfo = single_Data.mid(10,2);
                        emit AckCmd_MainWindow_signal(returnAck,AckInfo);

                    }
                }

                //10、 读取histogram的返回信息
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("57" == secCmd)
                    {
                        QString dataTmp = single_Data.mid(10,dataLen);
                        int histohramIndex = dataTmp.mid(0,2).toInt(NULL,16);   //0-9
                        QString dataStr = dataTmp.right(dataLen-2);    //减去（00-09） 这个标号字节
                        int allLen = dataStr.length();
                        if(2048 != allLen)
                        {
                            qDebug()<<QStringLiteral("vangogh 读取直方图时候接收到的数据长度有误！")<<allLen;

                        }
                        QString hisData;
                        int index = 0;
                        int tmpValue = 0;
                        int maxValue = 0;

                        for(int i =0; i<2048; i=i+4)     //数据格式为大端
                        {
                            hisData = dataStr.mid(i,4);
                            tmpValue = hisData.toInt(NULL,16);

                            vangoghHistogram_512[index] = tmpValue;
                            maxValue = tmpValue>maxValue?tmpValue:maxValue;
                            index++;

                        }

                        emit toShowVangogh_histogramSignal(histohramIndex,vangoghHistogram_512,maxValue);
                    }

                }


                // 11、vangogh 单次测量的返回命令
                if("80" == returnCmdStr)
                {
                    QString secCmd= single_Data.mid(8,2);
                    if("58" == secCmd)
                    {
                        QString dataStr = single_Data.mid(10,dataLen);
                        QString currentSingleData;
                        float tmp_LSB = 0;
                        float tmp_MM = 0;
                        float tmp_peak = 0;
                        float noise_mean = 0;
                        float N1 = 0;
                        float confidence = 0 ;
                        float Dmax = 0;



                        int pointNum = 0;    //该包数据点的个数

                         //16进制数据转化为10进制 然后再转化成字符串
                        for(int i=0; i<dataLen; i+=20)
                        {
                            pointNum++;
                            // 1  16进制数据转化为10进制 然后再转化成字符串
                            QString strTmp = dataStr.mid(i+2,2) + dataStr.mid(i+0,2);
                            tmp_LSB = strTmp.toInt(NULL,16);   //1
                            strTmp = dataStr.mid(i+6,2) + dataStr.mid(i+4,2);
                            tmp_MM = strTmp.toInt(NULL,16);    //2
                            strTmp = dataStr.mid(14,2) + dataStr.mid(12,2) + dataStr.mid(10,2) + dataStr.mid(8,2);
                            tmp_peak = strTmp.toInt(NULL,16);  //3
                            strTmp = dataStr.mid(18,2) + dataStr.mid(16,2);
//                            qDebug()<<"strTmp = "<<strTmp;
                            noise_mean = strTmp.toInt(NULL,16); // 4

                            //2 显示
                            N1 = noise_mean * C1;

                            //3 计算confidence
                            float N2 = noise_mean * C2;
                            float sigma = sqrt(C3 * noise_mean);
                            if(tmp_peak<(N2+3*sigma))
                            {
                                confidence = 0;
                            }else if(tmp_peak > (N2+6*sigma))
                            {
                                confidence = 63.0;
                            }else
                            {
                                float tmpFloat = 8 * (6*sigma - tmp_peak + N2)/(3*sigma);
                                confidence = (64.0 - pow(tmpFloat,2));
                            }

                            // 4 计算DMAX
//                            float tmpFloat_1 = sqrt(C3 * noise_mean);
//                            float tmpFloat_2 = (P0 * P * IT)/(6 * P0 * tmpFloat_1 * IT0 );
//                            Dmax = R0 * sqrt(tmpFloat_2);
                            float tmpFloat_1 = (0.5 * P0)/(6 * row0 * sigma);
                            Dmax = R0 * sqrt(tmpFloat_1);




                            currentSingleData = QString::number(tmp_LSB);
                            currentSingleData.append("   ");
                            currentSingleData.append(QString::number(tmp_MM));
                            currentSingleData.append("   ");
                            currentSingleData.append(QString::number(tmp_peak));
                            currentSingleData.append("   ");
                            currentSingleData.append(QString::number(N1,'f',3));
                            currentSingleData.append("Mcps   ");
                            currentSingleData.append(QString::number(int(confidence)));
                            currentSingleData.append("/64   ");
                            currentSingleData.append(QString::number(Dmax,'f',3));
                            currentSingleData.append("mm   ");

                            vangogh_DistanceStr.append(currentSingleData);
                        }

                        emit toShow_vangogh_ResultMsg_signal(vangogh_DistanceStr,pointNum);
                        vangogh_DistanceStr.clear();

                    }
                }


                // 12、 连续测量的返回命令
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("59" == secCmd)
                    {
                        QString dataStr = single_Data.mid(10,dataLen);
                        QString currentSingleData;
                        float tmp_LSB = 0;
                        float tmp_MM = 0;
                        int pointNum = 0;

                        float tmp_peak = 0;
                        float noise_mean = 0;
                        float N1 = 0;
                        float confidence = 0 ;
                        float Dmax = 0;

                        //16进制数据转化为10进制 然后再转化成字符串
                       for(int i=0; i<dataLen; i+=20)
                       {


                           pointNum++;
                           //16进制数据转化为10进制 然后再转化成字符串
                           // 1  16进制数据转化为10进制 然后再转化成字符串
                           QString strTmp = dataStr.mid(i+2,2) + dataStr.mid(i+0,2);
                           tmp_LSB = strTmp.toInt(NULL,16);   //1
                           strTmp = dataStr.mid(i+6,2) + dataStr.mid(i+4,2);
                           tmp_MM = strTmp.toInt(NULL,16);    //2
                           strTmp = dataStr.mid(14,2) + dataStr.mid(12,2) + dataStr.mid(10,2) + dataStr.mid(8,2);
                           tmp_peak = strTmp.toInt(NULL,16);  //3
                           strTmp = dataStr.mid(18,2) + dataStr.mid(16,2);

                           noise_mean = strTmp.toInt(NULL,16); // 4


                           //2 显示
                           N1 = noise_mean * C1;
//                           qDebug()<<"strTmp = "<<strTmp<<"  noise_mean="<<noise_mean<<"  C1="<<C1;

                           //3 计算confidence
                           float N2 = noise_mean * C2;
                           float sigma = sqrt(C3 * noise_mean);
                           if(tmp_peak<(N2+3*sigma))
                           {
                               confidence = 0;
                           }else if(tmp_peak > (N2+6*sigma))
                           {
                               confidence = 63.0;
                           }else
                           {
                               float tmpFloat = 8 * (6*sigma - tmp_peak + N2)/(3*sigma);
                               confidence = (64.0 - pow(tmpFloat,2));
                           }

                           // 4 计算DMAX
//                           float tmpFloat_1 = sqrt(C3 * noise_mean);
//                           float tmpFloat_2 = (P0 * P * IT)/(6 * P0 * tmpFloat_1 * IT0 );
//                           Dmax = R0 * sqrt(tmpFloat_2);
                           float tmpFloat_1 = (0.5 * P0)/(6 * row0 * sigma);
                           Dmax = R0 * sqrt(tmpFloat_1);


                           currentSingleData = QString::number(tmp_LSB);
                           currentSingleData.append("   ");
                           currentSingleData.append(QString::number(tmp_MM));
                           currentSingleData.append("   ");
                           currentSingleData.append(QString::number(tmp_peak));
                           currentSingleData.append("   ");
                           currentSingleData.append(QString::number(N1,'f',3));
                           currentSingleData.append("Mcps   ");
                           currentSingleData.append(QString::number(int(confidence)));
                           currentSingleData.append("/64   ");
                           currentSingleData.append(QString::number(Dmax,'f',3));
                           currentSingleData.append("mm   ");


                           //统计信息相关的变量存储
                           int StatisticLSB_offset = StatisticLSB_vector.size() -1000;
                           if(StatisticLSB_offset >= 0)
                           {
                               StatisticLSB_vector.erase(StatisticLSB_vector.begin(),StatisticLSB_vector.begin()+StatisticLSB_offset+1);
                           }
                           StatisticLSB_vector.push_back(tmp_LSB);

                           int StatisticMM_offset = StatisticMM_vector.size() - 1000;
                           if(StatisticMM_offset>=0)
                           {
                               StatisticMM_vector.erase(StatisticMM_vector.begin(),StatisticMM_vector.begin()+StatisticMM_offset + 1);
                           }
                           StatisticMM_vector.push_back(tmp_MM);



                           vangogh_DistanceStr.append(currentSingleData);
                       }
                       emit toSendStatistic_signal(StatisticLSB_vector,StatisticMM_vector);
                       emit toShow_vangogh_ResultMsg_signal(vangogh_DistanceStr,pointNum);
                       vangogh_DistanceStr.clear();

                    }

                }

                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("5A" == secCmd)
                    {
                        QString returnAck = "805A";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_autoStepping_signal(returnAck,AckInfo);
                    }
                }



                //13 DCR测试返回命令
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("5C" == secCmd)
                    {
                        QString returnAck = "805C";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_DCRTest_signal(returnAck,AckInfo);
                    }

                }


                // 14 delayLine的测试返回命令
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("5D" == secCmd)
                    {
                        QString returnAck = "805D";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_delayLine_signal(returnAck,AckInfo);
                    }
                }

                // 15 获取时钟相位
                if("80" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("5E" == secCmd)
                    {
                        QString returnAck = "805E";
                        QString AckInfo = single_Data.mid(10,dataLen);
                        emit AckCmd_delayLine_signal(returnAck,AckInfo);
                    }
                }

                // 16 写窗函数的返回指令
                if("81" == returnCmdStr)
                {
                    QString secCmd = single_Data.mid(8,2);
                    if("5F" == secCmd)
                    {
                        QString returnAck = "815F";
                        QString AckInfo = "00"; //无效值
                        emit AckCmd_windowSetting_signal(returnAck,AckInfo);

                    }

                }







//                if("86" == returnCmdStr)
//                {
//                    QString secCmd = single_Data.mid(4,2);     //这个实际是长度
//                    if("01" == secCmd)
//                    {
//                        QString cmdAck = single_Data.mid(8,2);
//                        emit AckCmdUpgrade_signal(returnCmdStr,cmdAck);
//                    }
//                }




                ////////////////////////////////////////////////////////单个命令处理代码块//////////////////////////////////////////////////////////////////////////////////////////
                m_buffer = m_buffer.right(totallen - len);                                                  //一帧处理完毕 减去该帧的长度
                totallen = m_buffer.size();
                //         qDebug()<<"total ="<<totallen<<endl;
            }
        }
        else   //直接打印16进制的数据
        {
            vangogh_DistanceStr.append(m_buffer);
            emit showResultMsg_signal(vangogh_DistanceStr,0);                                                   //发送用于界面显示的数据  显示TOF或者PEAK 或者16进制数据
            vangogh_DistanceStr.clear();                                                                      //清空
            m_buffer.clear();
            totallen = m_buffer.size();
        }

    }
}



//字节校验 从第二个字节开始，到倒数第二个字节 求和并取反 判断是否与最后一个字节相等
bool receSerial_msg::msgCheck(QString msg)
{
    int len = msg.length();
    int i=2;
    int num = 0;
    for(;i<len-2;i+=2)
    {
        num += msg.mid(i,2).toInt(NULL,16);
    }

    int checkNum = msg.mid(i,2).toInt(NULL,16);
    if(quint8(~num) == checkNum)
    {
        return true;
    }else
    {
        return false;
    }
}


//!
//! \brief receSerial_msg::sendSerialSlot
//! \param sendCmdStr: 不带校验位的 QString 类型的字符串  QString cmdStr = "5A 03 04 00 0A 00 88 13 ";
//! 1、添加校验位
//! 2、转换为字节数,存储在QByteArray
//! 3、向串口发送数据
//! 4、清空缓存区
void receSerial_msg::sendSerialSlot(QString sendCmdStr)
{
    QByteArray sendArray;
    QString wholeStr;
    wholeStr = addCheck(sendCmdStr);     //添加校验
    sendArray = StringToByte(wholeStr);  //转换成字节数据
    if(serial!=NULL && serial->isWritable())
    {
        totallen = 0;
        m_buffer.clear();
        serial->write(sendArray);            //串口发送字节数据
        serial->flush();                     //清空缓冲区
    }
    else
    {
        qDebug()<<"can not write right now ";
    }

}


//为命令添加校验位，并重新计算长度
QString receSerial_msg::addCheck(QString str)
{
    "5A 06 00 01 01 ";
    //    str = "5A 07 14 00 0800a180 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00";

    QString tmpStr = str;
    tmpStr.replace(" ","");
    int len = tmpStr.length();
    //数据区的长度为 len-4
    int dataLen = len/2-4;

    QString dataTmp = QString("%1").arg(dataLen,4,16,QLatin1Char('0'));
    QString lenStr = dataTmp.mid(2,2)+dataTmp.mid(0,2);
    tmpStr = tmpStr.replace(4,4,lenStr);
    int sum = 0;
    for(int i = 2; i<len; i+=2)
    {
        sum += tmpStr.mid(i,2).toInt(NULL,16);
    }
    quint8 checkValue = (quint8)(~sum);
    QString resStr = tmpStr.append(QString("%1").arg(checkValue,2,16,QLatin1Char('0')));

    //    qDebug()<<"addCheck = "<<resStr;
    return resStr;
}


//!
//! \brief receSerial_msg::StringToByte
//! \return
//! 将QString QByteArray
QByteArray receSerial_msg::StringToByte(QString str)
{
    QByteArray byte_arr;
    bool ok;
    //如果str的长度 不是2的倍数  那么直接返回空
    //    if(str.size()%2!=0){
    //        qDebug()<<"is not twice "<<endl;
    //        return QByteArray::fromHex("字符串不符合标准");
    //    }

    str = str.replace(" ","");    //去掉空格键
    int len=str.size();
    for(int i=0;i<len;i+=2){
        byte_arr.append(char(str.mid(i,2).toUShort(&ok,16)));
    }
    //    qDebug()<<" byte_arr's len = "<<byte_arr.size()<<"    "<<byte_arr;

    //下面这段程序是将Byte 转化为QString的类型，将结果输出 对比发出的数据是否正确
    QDataStream out(&byte_arr,QIODevice::ReadWrite);
    QString strHex;
    while (!out.atEnd())
    {
        qint8 outChar=0;
        out>>outChar;
        QString str=QString("%1").arg(outChar&0xFF,2,16,QLatin1Char('0'));

        if (str.length()>1)
        {
            strHex+=str+" ";
        }
        else
        {
            strHex+="0"+str+" ";
        }
    }
    strHex = strHex.toUpper();
    qDebug()<<QStringLiteral("发送的原始数据为：")<<strHex<<"   len="<< strHex.length()/3<<endl;
    return byte_arr;
}





