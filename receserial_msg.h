#ifndef RECESERIAL_MSG_H
#define RECESERIAL_MSG_H

#include <QObject>
#include"globaldata.h"






class receSerial_msg : public QObject
{
    Q_OBJECT
public:
    explicit receSerial_msg(QObject *parent = 0);

    QSerialPort *serial;

    QByteArray m_buffer;

    bool isTranslateFlag;     //解析数据 还是直接显示16进制的 切换标识 true：则对数据进行解析

    //统计相关
    vector<double> StatisticLSB_vector;     //统计相关  均值方差
    vector<double> StatisticMM_vector;



    QStringList   vangogh_DistanceStr;               //显示tof peak相关

    bool clearFlag;

    int totallen;

    QVector<double> vangoghHistogram_512;


    /******计算confidence 以及 dmax 相关**********/
    float C1;
    float C2;
    float C3;
    float R0;
    float row0;
    float row;
    float P0;

    float IT;
    float IT0;



signals:
    void dealedData_signal(QString,vector<double>,vector<double>);     //当前的tof值 ; plotData ; statisticData

    void showResultMsg_signal(QStringList,int);  //显示tof peak相关  主界面显示;  主线程中设定一个暂存变量，每秒钟在result窗口中显示append(),然后清空result  ,该包数据点的个数

    void returnLinkInfo_signal(QString, bool);

    //!
    //! \brief AckCmdUpgrade_signal
    //!//升级相关的信号，参数为两个，1：“86”：开始升级命令应答    参数2： “01”：成功
    //!                                                            “00” ：失败
    //!                                                            其他：命令有误
    //!                              “87”：升级过程中的命令应答 参数2： 应答的数据
    void AckCmdUpgrade_signal(QString,QString);

    void AckCmdRegister_signal(QString ,QString );

    void AckCmd_distanceTest_signal(QString,QStringList);   //距离测试的信号 参数1：命令    参数2：所需的返回信息

    void AckCmd_rawDataTest_signal(QString,QStringList);                     //rowData返回命令相关

    void AckCmd_channelParameter_signal(QString,QString);

    void AckCmd_MainWindow_signal(QString,QString);

    void AckCmd_DCRTest_signal(QString,QString);

    void AckCmd_delayLine_signal(QString,QString);

    void AckCmd_autoStepping_signal(QString,QString);

    void AckCmd_windowSetting_signal(QString,QString);


    void toShowVangogh_histogramSignal(int,QVector<double>,int);   //显示梵高的直方图
    void toShowCassatt_histogramSignal(QVector<double>,int);   //显示卡萨特直方图
    void toShow4300_histogramSignal(QVector<double>,int);

    void toShow_vangogh_ResultMsg_signal(QStringList,int);
    void toSendStatistic_signal(vector<double>,vector<double>);     //tof ,MM的容器




public slots:
    void readDataSlot();

    bool msgCheck(QString msg);

    void openOrCloseSerial_slot(bool);

    void sendSerialSlot(QString);              //串口发送数据的槽函数

    QString addCheck(QString);

    QByteArray StringToByte(QString str);      //将QString 转换为 Byte的槽函数




};

#endif // RECESERIAL_MSG_H
