#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"globaldata.h"
#include"receserial_msg.h"
#include"cassathistogram_dialog.h"
#include"regisitersetdialog.h"
#include"distancetest_dialog.h"
#include "rowdata_dialog.h"
#include"channekparameter_dialog.h"
#include"autostepping_dialog.h"
#include"dcr_test_dialog.h"
#include"delayline_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initConnect();

    //串口初始化相关
    void initSerial();
    void SerialSetting_Enable_true();
    void SerialSetting_Enable_false();

private slots:

    void Display_log_slot(QString str);  //打印日志信息到控制信息输出窗口的槽函数

    void AckCmd_MainWindow_slot(QString,QString);    //消息返回的槽函数

    void on_portScan_pushButton_clicked();

    void on_openPort_pushButton_clicked();

    void returnLinkInfo_slot(QString str, bool flag);

    void on_CH_IA_import_pushButton_clicked();

    void on_CH_IA_export_pushButton_clicked();

    void on_electricityTest_pushButton_clicked();

    void on_vango_timeCheck_pushButton_clicked();

    void on_vango_vspadCheck_pushButton_clicked();

    void on_vango_MCU_test_pushButton_clicked();

    void on_distance_test_pushButton_clicked();

    void on_inteface_import_pushButton_clicked();

    void on_inteface_export_pushButton_clicked();

    void on_register_import_pushButton_clicked();

    void on_register_export_pushButton_clicked();

    void on_register_I2C_write_pushButton_clicked();

    void on_register_SPI_write_pushButton_clicked();

    void on_vangoFW_select_pushButton_clicked();

    void on_vangoFW_write_pushButton_clicked();

    void on_Register_action_triggered();


    void on_RawData_dia_pushButton_clicked();

    void on_vango_Histogram_pushButton_clicked();

    void on_channelParametre_pushButton_clicked();

    void on_chipZhongduanShineng_checkBox_clicked();

    void on_ChipShineng_checkBox_clicked();

    void on_ChipFuwei_pushButton_clicked();



    void on_zongdianyuan_close_radioButton_clicked();

    void on_zongdianyuan_open_radioButton_clicked();

    void on_CH1_in_radioButton_clicked();

    void on_CH1_out_radioButton_clicked();

    void on_CH2_in_radioButton_clicked();

    void on_CH2_out_radioButton_clicked();

    void on_CH3_in_radioButton_clicked();

    void on_CH3_out_radioButton_clicked();

    void on_CH4_in_radioButton_clicked();

    void on_CH4_out_radioButton_clicked();

    void on_CH5_in_radioButton_clicked();

    void on_CH5_out_radioButton_clicked();

    void on_I2C_MCU_radioButton_clicked();

    void on_I2C_outer_radioButton_clicked();

    void on_SPI_master_MCU_radioButton_clicked();

    void on_SPI_master_outer_radioButton_clicked();

    void on_SPI_slave_MCU_radioButton_clicked();

    void on_SPI_slave_outer_radioButton_clicked();

    void on_interuptReser_MCU_radioButton_clicked();

    void on_interuptReser_outer_radioButton_clicked();

    void on_time_banzai_radioButton_clicked();

    void on_time_outer_radioButton_clicked();

    void on_AutoStepping_pushButton_clicked();

    void on_Vango_DCR_pushButton_clicked();

    void on_Vango_Delayline_pushButton_clicked();

signals:
    void openOrCloseSerial_signal(bool);

    void sendSerialSignal(QString);             //串口发送信号

private:
    Ui::MainWindow *ui;

    receSerial_msg  *receSerial_Obj;      //串口接收数据线程
    QThread *receSerialThread;
    bool isLinked;   //是否连接的标识


    RegisiterSetDialog register_dia;            //寄存器的配置的界面
    distanceTest_Dialog distanceTest_dia;       //测距界面
    RowData_Dialog RawData_dia;                 //rowData界面
    cassatHistogram_Dialog cassatHistogram_dia;  //Cassat直方图
    channekParameter_Dialog channelPara_dia;     //通道系数设置界面

    QStringList fwBin_stringList;

    AutoStepping_Dialog autoStep_dia;
    DCR_test_Dialog DCR_test_dia;

    delayLine_Dialog delayLine_dia;



};

#endif // MAINWINDOW_H
