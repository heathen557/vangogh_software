#ifndef DISTANCETEST_DIALOG_H
#define DISTANCETEST_DIALOG_H

#include <QDialog>
#include"globaldata.h"
#include<QTimer>
#include<QTextStream>
#include<iostream>

using namespace std;

namespace Ui {
class distanceTest_Dialog;
}

class distanceTest_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit distanceTest_Dialog(QWidget *parent = 0);
    ~distanceTest_Dialog();

public slots:
    void on_single_distanceTest_pushButton_clicked();

    void on_TOF_read_pushButton_clicked();

    void on_ContinuityTest_pushButton_clicked();

    void on_stop_pushButton_clicked();

    void saveData_Timer_slot();


    void toShow_vangogh_ResultMsg_slot(QStringList,int);
    void toSendStatistic_slot(vector<double>,vector<double>);



signals:
    void sendSerialSignal(QString);             //串口发送信号
    void Display_log_signal(QString);

private slots:
    void on_save_pushButton_clicked();

    void on_TimingSave_checkBox_clicked();


    /*****显示数据******/
    void showTimer_slot();

    void oneSec_Timer_slot();  //1sec 的槽函数

    void on_single_distanceTest_pushButton_2_clicked();

    void on_ContinuityTest_pushButton_2_clicked();

    void on_stop_pushButton_2_clicked();

private:
    Ui::distanceTest_Dialog *ui;

    QTimer saveData_Timer;

    int saveIndex;


    /**********显示tof和实际距离的相关变量*********/
    QStringList DistanceStr;
    int Count_num;
    QTimer showTimer;  //70ms

    //统计信息相关
    vector<double> StatisticLSB_vector;     //统计相关  均值方差
    vector<double> StatisticMM_vector;
    float LSB_mean,LSB_std;
    float MM_mean,MM_std;

    //1s的定时器
    QTimer oneSec_Timer;


};

#endif // DISTANCETEST_DIALOG_H
