#include "rowdata_dialog.h"
#include "ui_rowdata_dialog.h"

RowData_Dialog::RowData_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RowData_Dialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);
    ui->splitter->setStretchFactor(0,5);
    ui->splitter->setStretchFactor(1,3);

    connect(&saveData_Timer,SIGNAL(timeout()),this,SLOT(saveData_Timer_slot()));

    saveIndex = 1;
}

RowData_Dialog::~RowData_Dialog()
{
    delete ui;
}


//ack的返回命令
void RowData_Dialog:: AckCmd_rawDataTest_slot(QString returnCmdStr,QStringList cmdInfoList)
{

}

void RowData_Dialog::on_save_pushButton_clicked()
{
    QString sFilePath = ui->fileSavePath_lineEdit->text();
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
        ui->fileSavePath_lineEdit->setText(sFilePath);
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("文件已保存！"));
    }else
    {
        QString strMsg = QStringLiteral("文件保存失败，请检查路径");
        QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
    }
}


//!
//! \brief RowData_Dialog::on_timingSave_checkBox_clicked
//!开启定时保存槽函数
void RowData_Dialog::on_timingSave_checkBox_clicked()
{
    QString sFilePath = ui->fileSavePath_lineEdit->text();
    if(sFilePath.isEmpty())
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("请先设置文件保存路径"));
        ui->timingSave_checkBox->setChecked(false);
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
    ui->fileSavePath_lineEdit->setText(sFilePath);


    if(ui->timingSave_checkBox->isChecked())
    {
        saveIndex = 1;
        ui->timeOffset_lineEdit->setEnabled(false);
        ui->fileSavePath_lineEdit->setEnabled(false);
        ui->save_pushButton->setEnabled(false);
        int timOffset = ui->timeOffset_lineEdit->text().toInt();
        saveData_Timer.start(timOffset * 1000);
    }else
    {
        ui->timeOffset_lineEdit->setEnabled(true);
        ui->fileSavePath_lineEdit->setEnabled(true);
        ui->save_pushButton->setEnabled(true);
        saveData_Timer.stop();
    }
}

//!
//! \brief RowData_Dialog::saveData_Timer_slot
//!
void RowData_Dialog::saveData_Timer_slot()
{
    QString filePath = ui->fileSavePath_lineEdit->text();
    QString sFilePath = filePath+QString::number(saveIndex)+".txt";

    QString text = ui->plainTextEdit->toPlainText();

    QFile file(sFilePath);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream out(&file);
        out<<text.toLocal8Bit()<<endl;
        file.close();
        ui->fileSavePath_lineEdit->setText(sFilePath);
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("文件已保存！"));
    }else
    {
        QString strMsg = QStringLiteral("文件保存失败，请检查路径");
        QMessageBox::information(NULL,QStringLiteral("提示"),strMsg);
    }
}

