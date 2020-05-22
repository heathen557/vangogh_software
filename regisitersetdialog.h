#ifndef REGISITERSETDIALOG_H
#define REGISITERSETDIALOG_H

#include <QDialog>
#include<QFileDialog>
#include<QTableWidgetItem>
#include<QMessageBox>
#include<QDebug>
#include<QCloseEvent>

namespace Ui {
class RegisiterSetDialog;
}

class RegisiterSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisiterSetDialog(QWidget *parent = 0);
    ~RegisiterSetDialog();

    QTableWidgetItem  addressItem[300];
    QTableWidgetItem  valueItem[300];

    void changeEvent(QEvent *e);

    void closeEvent(QCloseEvent *event);

    void setItemFont_Black();

    int row_show;
    int col_show;

    QColor alter_color;
    QColor differ_color;
    QColor black_color;


    int select_single_row;
    int select_single_col;

public slots:
    void on_loadLocal_pushButton_clicked();

    void on_saveLocal_pushButton_clicked();

    void on_send_pushButton_clicked();

    void on_read_pushButton_clicked();

    void clearItem();

    //!
    //! \brief AckCmdRegister_signal
    //!寄存器返回命令的相关信号   参数1：“81”：写寄存器应答  参数2 暂无
    //!                              “80”：读寄存器应答  参数2 寄存器的数据区
    void AckCmdRegister_slot(QString,QString);

    void tableWidget_cellChanged(int row, int column);

signals:
    void sendSerialSignal(QString);             //串口发送信号
    void Display_log_signal(QString);

private slots:
    void on_test_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_singleSend_pushButton_clicked();


private:
    Ui::RegisiterSetDialog *ui;
};

#endif // REGISITERSETDIALOG_H
