#include "distancetest_dialog.h"
#include "ui_distancetest_dialog.h"

distanceTest_Dialog::distanceTest_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::distanceTest_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);

    ui->plainTextEdit->document()->setMaximumBlockCount(5000);   //最多存储5000条数据

    ui->splitter->setStretchFactor(0,5);
    ui->splitter->setStretchFactor(1,3);
    connect(&saveData_Timer,SIGNAL(timeout()),this,SLOT(saveData_Timer_slot()));   //保存
    connect(&showTimer,SIGNAL(timeout()),this,SLOT(showTimer_slot()));
    connect(&oneSec_Timer,SIGNAL(timeout()),this,SLOT(oneSec_Timer_slot()));


    saveIndex = 1;
}

distanceTest_Dialog::~distanceTest_Dialog()
{
    delete ui;
}

//!
//! \brief distanceTest_Dialog::on_single_distanceTest_pushButton_clicked
//!单次测距 5A 00 02 00 58 00 
void distanceTest_Dialog::on_single_distanceTest_pushButton_clicked()
{

    qDebug()<<"on_single_distanceTest_pushButton_clicked";
    QString cmdStr = "5A 00 02 00 58 00";   //ef
    emit sendSerialSignal(cmdStr);
}

/**
*@date 2020-04-29
*@brief:  读取TOF
*/
void distanceTest_Dialog::on_TOF_read_pushButton_clicked()
{

}

/**
*@date 2020-04-29
*@brief:连续测距
*/
//连续测距命令 5A 00 02 00 59 00
void distanceTest_Dialog::on_ContinuityTest_pushButton_clicked()
{
    QString cmdStr ="5A 00 02 00 59 00";
    emit sendSerialSignal(cmdStr);

    showTimer.start(90);
    oneSec_Timer.start(1000);
}
/**
*@date 2020-04-29
*@brief: 停止测距
*/
void distanceTest_Dialog::on_stop_pushButton_clicked()
{
    qDebug()<<"distanceTest_Dialog::on_stop_pushButton_clicked()";

    showTimer.stop();
    oneSec_Timer.stop();
}


//! \brief distanceTest_Dialog::AckCmd_distanceTest_slot
//! \param returnCmdStr  命令字
//! \param cmdInfoList
//!
void distanceTest_Dialog::toShow_vangogh_ResultMsg_slot(QStringList DisStr,int pointNum)
{
    Count_num += pointNum;
    DistanceStr.append(DisStr);

}

//!
//! \brief showTimer_slot
//! 显示数据
void distanceTest_Dialog::showTimer_slot()
{
    int len = DistanceStr.length();
    len = len>50?50:len;
    for(int i=0;i<len;i++)
    {
        ui->plainTextEdit->appendPlainText(DistanceStr[i]);
    }
    DistanceStr.clear();


    //计算LSB的均值和方差
    len = StatisticLSB_vector.size();
    if(len<1)
        return;
    //计算LSB的均值和方差
    LSB_mean = std::accumulate(std::begin(StatisticLSB_vector),std::end(StatisticLSB_vector),0.0);

    float LSB_Accum = 0.0;
    std::for_each(std::begin(StatisticLSB_vector),std::end(StatisticLSB_vector),[&](const double d){
        LSB_Accum += (d-LSB_mean)*(d-LSB_mean);
    });
    LSB_std = sqrt(LSB_Accum/(len-1));

    ui->LSB_mean_label->setText(QString::number(LSB_mean));
    ui->LSB_std_label->setText(QString::number(LSB_std));


    //
    len = StatisticMM_vector.size();
    if(len<1)
        return;
    MM_mean = std::accumulate(std::begin(StatisticMM_vector),std::end(StatisticMM_vector),0.0);

    float MM_Accum = 0.0;
    std::for_each(std::begin(StatisticMM_vector),std::end(StatisticMM_vector),[&](const double d){
        MM_Accum += (d-MM_mean)*(d-MM_mean);
    });
    MM_std = sqrt(MM_Accum/(len-1));
    ui->MM_mean_label->setText(QString::number(MM_mean));
    ui->MM_std_label->setText(QString::number(MM_std));




}




//!单帧数据的保存
//!
void distanceTest_Dialog::on_save_pushButton_clicked()
{
    QString sFilePath = ui->filePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        return;
    }

    QString text = ui->plainTextEdit->toPlainText();

    if("txt" != sFilePath.right(3))
    {
        sFilePath.append(".txt");
    }

    QFile file(sFilePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out<<text.toLocal8Bit()<<endl;
        file.close();
        ui->filePath_lineEdit->setText(sFilePath);
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("文件已保存！"));
    }else
    {
        QString strMsg = QStringLiteral("文件保存失败，请检查路径");
        QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
    }

}

//!
//! \brief distanceTest_Dialog::on_TimingSave_checkBox_clicked
//!定时保存
//! 1、 如果.txt结尾的则去掉.txt;
//! 2、如果没有添加“\\”,则添加这样的后缀
void distanceTest_Dialog::on_TimingSave_checkBox_clicked()
{
    QString sFilePath = ui->filePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        ui->TimingSave_checkBox->setChecked(false);
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


    if(ui->TimingSave_checkBox->isChecked())
    {
        saveIndex = 1;
        ui->timeOffset_lineEdit->setEnabled(false);
        ui->filePath_lineEdit->setEnabled(false);
        ui->save_pushButton->setEnabled(false);
        int timOffset = ui->timeOffset_lineEdit->text().toInt();
        saveData_Timer.start(timOffset * 1000);
    }else
    {
        ui->timeOffset_lineEdit->setEnabled(true);
        ui->filePath_lineEdit->setEnabled(true);
        ui->save_pushButton->setEnabled(true);
        saveData_Timer.stop();
    }


}


//!
//! \brief distanceTest_Dialog::saveData_Timer_slot
//! 定时器保存
void  distanceTest_Dialog::saveData_Timer_slot()
{
    QString filePath = ui->filePath_lineEdit->text();
    QString sFilePath = filePath+QString::number(saveIndex)+".txt";

    QString text = ui->plainTextEdit->toPlainText();

    QFile file(sFilePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out<<text.toLocal8Bit()<<endl;
        file.close();
        ui->filePath_lineEdit->setText(sFilePath);
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("文件已保存！"));
    }else
    {
        QString strMsg = QStringLiteral("文件保存失败，请检查路径");
        QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
    }
}

//!
//! \brief distanceTest_Dialog::toSendStatistic_slot
//! 接收统计信息的槽函数，存储相关的变量
void distanceTest_Dialog::toSendStatistic_slot(vector<double> LSB_vector,vector<double> MM_vector)
{
    StatisticLSB_vector = LSB_vector;
    StatisticMM_vector = MM_vector;
}


//!
//! \brief distanceTest_Dialog::oneSec_Timer_slot
//! 1 second 对应的槽函数
void distanceTest_Dialog::oneSec_Timer_slot()
{
    ui->DPS_label->setText(QString::number(Count_num));
    Count_num = 0;
}






//!
//! \brief distanceTest_Dialog::on_single_distanceTest_pushButton_2_clicked
//!单次测量的槽函数
void distanceTest_Dialog::on_single_distanceTest_pushButton_2_clicked()
{
    qDebug()<<"on_single_distanceTest_pushButton_clicked";
    QString cmdStr = "5A 00 02 00 58 00";   //ef
    emit sendSerialSignal(cmdStr);
    showTimer.start(90);
    oneSec_Timer.start(1000);
}

//!
//! \brief distanceTest_Dialog::on_ContinuityTest_pushButton_2_clicked
//!连续测量的槽函数
void distanceTest_Dialog::on_ContinuityTest_pushButton_2_clicked()
{
    QString cmdStr ="5A 00 02 00 59 01";
    emit sendSerialSignal(cmdStr);

    showTimer.start(90);
    oneSec_Timer.start(1000);
}

//!
//! \brief distanceTest_Dialog::on_stop_pushButton_2_clicked
//!停止测量的槽函数
void distanceTest_Dialog::on_stop_pushButton_2_clicked()
{
    QString cmdStr = "5A 00 02 00 59 00";
    emit sendSerialSignal(cmdStr);

    showTimer.stop();
    oneSec_Timer.stop();

}

void distanceTest_Dialog::on_clear_pushButton_clicked()
{
   ui->plainTextEdit->clear();
}
