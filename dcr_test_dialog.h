#ifndef DCR_TEST_DIALOG_H
#define DCR_TEST_DIALOG_H

#include <QDialog>
#include<qdebug.h>
#include<QTableWidgetItem>

namespace Ui {
class DCR_test_Dialog;
}

class DCR_test_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit DCR_test_Dialog(QWidget *parent = 0);
    ~DCR_test_Dialog();


    QTableWidgetItem  DCR_Item[17][9];

    float devTime;


signals:
    void sendSerialSignal(QString );             //串口发送信号

public slots:
    void on_start_pushButton_clicked();

    void AckCmd_DCRTest_slot(QString,QString);

private:
    Ui::DCR_test_Dialog *ui;
};

#endif // DCR_TEST_DIALOG_H
