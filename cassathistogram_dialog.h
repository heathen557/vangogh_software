#ifndef CASSATHISTOGRAM_DIALOG_H
#define CASSATHISTOGRAM_DIALOG_H

#include <QDialog>
#include"globaldata.h"

namespace Ui {
class cassatHistogram_Dialog;
}

class cassatHistogram_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit cassatHistogram_Dialog(QWidget *parent = 0);
    ~cassatHistogram_Dialog();

private:
    Ui::cassatHistogram_Dialog *ui;


    QCPBars *regen_vangogh[10];          //Vangogh直方图用   *****标签相关


    QVector<double> vangogh_HistorgramTicks;      //标签
    QVector<QString> vangogh_HistorgramLabels;    //标识
    int binSize;

    QCPBars *regen_cassatt;          //直方图用   *****标签相关
    QVector<double> cassatt_HistorgramTicks;      //标签
    QVector<QString> cassatt_HistorgramLabels;    //标识

    QCPBars *regen_4300;          //直方图用   *****标签相关
    QVector<double> vi4300_HistorgramTicks;      //标签
    QVector<QString> vi4300_HistorgramLabels;    //标识

    void init_histogram();      //初始化直方图数据


    //VANGOGH
    QTimer vangogh_read_timer;

    int lasthistogramIndex;
    bool readAll_flag;

    QString saveFileText;  //存储用的
    QString saveText[10];
    bool isSaveFileFlag;
    int saveFile_index;

public slots:
    void toShowVangogh_histogramSlot(int,QVector<double>,int);   //显示梵高的直方图
    void toShowCassatt_histogramSlot(QVector<double>,int);   //显示卡萨特直方图
    void toShow4300_histogramSlot(QVector<double>,int);      //显示4300直方图

    //vangogh
    void vangogh_read_timer_slot();

    void sendCmd_slot();



signals:
    void sendSerialSignal(QString);             //串口发送信号
    void Display_log_signal(QString);

private slots:
    void on_pushButton_clicked();

    void on_read_vangogh_Histogram_pushButton_clicked();
    void on_saveData_pushButton_clicked();

    void writeTxt_slot(QString ,QString );
};

#endif // CASSATHISTOGRAM_DIALOG_H
