#ifndef PIXELSETTING_DIALOG_H
#define PIXELSETTING_DIALOG_H

#include <QDialog>
#include<QCheckBox>
#include<QTableWidgetItem>
#include<QTableWidget>
#include<qdebug.h>
#include<QMessageBox>

namespace Ui {
class pixelSetting_Dialog;
}

class pixelSetting_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit pixelSetting_Dialog(QWidget *parent = 0);
    ~pixelSetting_Dialog();

    QTableWidgetItem checkBox_byte[18][8];   //18个字节  每个比特8比特
    QList<QTableWidget *> QTableWidget_list;


public slots:
    void on_setting_pushButton_clicked();

    void AckCmd_PixelSetting_slot(QString,QString);
signals:
    void sendSerialSignal(QString);             //串口发送信号


private:
    Ui::pixelSetting_Dialog *ui;
};

#endif // PIXELSETTING_DIALOG_H
