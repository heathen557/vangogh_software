#ifndef ROWDATA_DIALOG_H
#define ROWDATA_DIALOG_H

#include <QDialog>
#include"globaldata.h"

namespace Ui {
class RowData_Dialog;
}

class RowData_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit RowData_Dialog(QWidget *parent = 0);
    ~RowData_Dialog();

signals:

    void sendSerialSignal(QString);             //串口发送信号

    void Display_log_signal(QString);


public slots:

    void AckCmd_rawDataTest_slot(QString,QStringList);

    void saveData_Timer_slot();

private slots:
    void on_save_pushButton_clicked();

    void on_timingSave_checkBox_clicked();

private:
    Ui::RowData_Dialog *ui;

    QTimer saveData_Timer;

    int saveIndex;

};

#endif // ROWDATA_DIALOG_H
