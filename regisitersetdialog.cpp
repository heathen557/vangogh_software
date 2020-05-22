#include "regisitersetdialog.h"
#include "ui_regisitersetdialog.h"

RegisiterSetDialog::RegisiterSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisiterSetDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Widget);

//    ui->test_pushButton->setVisible(false);

//    ui->tableWidget->setMouseTracking(true);

    alter_color = QColor(10,10,250);
    differ_color = QColor(250,0,0);
    black_color = QColor(Qt::black);

    row_show = 26;
    col_show = 20;

    ui->tableWidget->setAlternatingRowColors(true);
//    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(row_show);
    ui->tableWidget->setColumnCount(col_show);
//    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中
//    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);   //禁止编辑
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:rgb(240,240,240)}"); //设置表头背景色
    ui->tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{background:rgb(240,240,240)}");


    int itemIndex = 0;
    for(int col=0; col<col_show; col+=2)
    {
        for(int row = 0; row<row_show; row++)
        {
            ui->tableWidget->setItem(row,col,&addressItem[itemIndex]);
            ui->tableWidget->setItem(row,col+1,&valueItem[itemIndex]);
            addressItem[itemIndex].setTextAlignment(Qt::AlignCenter);
            addressItem[itemIndex].setFlags(addressItem[itemIndex].flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);
            valueItem[itemIndex].setTextAlignment(Qt::AlignCenter);

            itemIndex++;
        }
    }

    setItemFont_Black();


}


//!
//! \brief RegisiterSetDialog::setItemFont_Black
//!所有字体恢复成黑色
void RegisiterSetDialog::setItemFont_Black()
{
    for(int i=0;i<300;i++)
    {
        valueItem[i].setTextColor(QColor(Qt::black));
    }
}



RegisiterSetDialog::~RegisiterSetDialog()
{
    delete ui;
}

//清除控件内容
//!
//! \brief RegisiterSetDialog::clearItem
//! 1 清空控件上的内容
//! 2 同时将控件上的文字显示 为黑色
void RegisiterSetDialog::clearItem()
{
    for(int i=0; i<300 ; i++)
    {
        addressItem[i].setText("");
        valueItem[i].setText("");
        valueItem[i].setTextColor(black_color);
    }

    ui->tableWidget->setCurrentCell(25,19);   //将焦点移动到最后一个单元格
}


//加载本地
//!
//! \brief RegisiterSetDialog::on_loadLocal_pushButton_clicked
//! 1 断开信号与槽的连接
//! 2 首先清空内容
//! 3  加载配置文件 并显示在控件上
void RegisiterSetDialog::on_loadLocal_pushButton_clicked()
{
    //加载本地文件之前，先断开信号与槽的连接
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),0,0);

    clearItem();   //首先清空内容


    QString file_path;
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    //定义文件对话框标题
    fileDialog->setWindowTitle(QStringLiteral("请选择配置文件"));
    //设置默认文件路径
    fileDialog->setDirectory(".");
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);
    //打印所有选择的文件的路径
    QStringList mimeTypeFilters;
    mimeTypeFilters <<QStringLiteral("寄存器配置文件(*.para)|*.para") ;
    fileDialog->setNameFilters(mimeTypeFilters);
    QStringList fileNames;
    if(fileDialog->exec())
    {
        fileNames = fileDialog->selectedFiles();
    }else
    {
        return;
    }
    file_path = fileNames[0];
    qDebug()<<" file_path = "<<fileNames[0]<<endl;

    QString checkStr = file_path.right(4);
    if("para" != checkStr)
    {
        QMessageBox::information(NULL,QStringLiteral("告警"),QStringLiteral("请选择正确的配置文件！"));
        return ;
    }

    QFile file(file_path);
    QString line[400];
    QStringList strList;
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        int i = 0;
        while (!in.atEnd())
        {
            line[i] = in.readLine();
            if(!line[i].isEmpty())
            {
                line[i] = line[i].left(5);
                strList = line[i].split(" ");
               if(strList.size()>1)
               {
                   addressItem[i].setText(strList[0]);
                   valueItem[i].setText(strList[1]);
               }
            }
            i++;
        }
        file.close();
    }

    //全部加载完以后，建立信号与槽的连接
    connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableWidget_cellChanged(int,int)));
}



//保存本地   具体格式为 ：XX XX   16进制数据
void RegisiterSetDialog::on_saveLocal_pushButton_clicked()
{
    QString filePath;

    QFileDialog *fileDialog = new QFileDialog(this);//创建一个QFileDialog对象，构造函数中的参数可以有所添加。
    fileDialog->setWindowTitle(tr("Save As"));//设置文件保存对话框的标题
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);//设置文件对话框为保存模式
    fileDialog->setFileMode(QFileDialog::AnyFile);//设置文件对话框弹出的时候显示任何文件，不论是文件夹还是文件
    fileDialog->setViewMode(QFileDialog::Detail);//文件以详细的形式显示，显示文件名，大小，创建日期等信息；
    fileDialog->setGeometry(10,30,300,200);//设置文件对话框的显示位置
    fileDialog->setDirectory(".");//设置文件对话框打开时初始打开的位置
    QStringList mimeTypeFilters;
    mimeTypeFilters <<QStringLiteral("芯片配置文件(*.para)|*.para") ;
    fileDialog->setNameFilters(mimeTypeFilters);


    if(fileDialog->exec() == QDialog::Accepted)
    {
        filePath = fileDialog->selectedFiles()[0];//得到用户选择的文件名
        qDebug()<<" filePath = "<<filePath<<endl;
    }else
    {
        return ;
    }

    //获取当前控件上的内容,组装成 字符串
    int index = 0;
    QString textString;
    QString addrStr,valStr;
    while(1)
    {
        addrStr = addressItem[index].text().toUpper();
        valStr = valueItem[index].text().toUpper();
        if(addrStr.isEmpty() || valStr.isEmpty())
        {
            break;
        }
        textString.append(addrStr).append(" ").append(valStr).append("\n");
        index++;
    }


    QFile file(filePath);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(&file);
    out<<textString.toLocal8Bit()<<endl;

    QString str = QStringLiteral("文件保存成功，路径：") + filePath;
    QMessageBox::information(NULL,QStringLiteral("提示"),str);
}

//发送 寄存器配置   5A 01 LL LL  01  DD...DD XX
void RegisiterSetDialog::on_send_pushButton_clicked()
{
//    //发送完数据信息之后，断开信号与槽的连接,接收完数据之后重新建立连接
//    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),0,0);


    //获取当前控件上的内容,组装成 字符串
    int index = 0;
    QStringList textString;
    QString addrStr,valStr;
    while(1)
    {
        addrStr = addressItem[index].text().toUpper();
        valStr = valueItem[index].text().toUpper();
        addrStr = addrStr.trimmed();
        valStr = valStr.trimmed();
        if(addrStr.isEmpty() || valStr.isEmpty())
        {
            break;
        }
        addrStr = "00" + addrStr;
        valStr = "00" + valStr;

        addrStr = addrStr.right(2);
        valStr = valStr.right(2);


        textString.append(addrStr);
        textString.append(valStr);
        index++;
    }

    //命令组帧   5A 01 NN DD...DD XX
    QString cmdStr = "5A 01 ";
    int dataLen = textString.length();
    QString lenStrTmp = QString("%1").arg(dataLen + 1 ,4,16,QLatin1Char('0'));  //长度2个字节
    QString lenStr  = lenStrTmp.mid(2,2) + lenStrTmp.mid(0,2);             //转换成小端
    cmdStr.append(lenStr);
    cmdStr.append("01");                //寄存器地址
    foreach (QString str, textString) {
        cmdStr.append(str);
    }
    emit sendSerialSignal(cmdStr);



}

//读取 设备寄存器配置  5A 00 02 00 01 00
void RegisiterSetDialog::on_read_pushButton_clicked()
{
    //发送完数据信息之后，断开信号与槽的连接,接收完数据之后重新建立连接
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),0,0);


    ui->tableWidget->setCurrentCell(25,19);   //将光标移动至最末尾的地方

    //命令组帧
    QString cmdStr = "5A 00 02 00 01 00 ";
    emit sendSerialSignal(cmdStr);

}




//!
//! \brief AckCmdRegister_signal
//!寄存器返回命令的相关信号   参数1：“8101”：写寄存器应答  参数2 暂无
//!                              “8001”：读寄存器应答  参数2 寄存器的数据区
void RegisiterSetDialog::AckCmdRegister_slot(QString returnCmdStr,QString cmdInfo)
{
    if("8101" == returnCmdStr)
    {
        QMessageBox::information(NULL,QStringLiteral("提示"),QStringLiteral("写入寄存器成功！"));
        return;
    }else if("8001" == returnCmdStr)
    {
//        clearItem();                            //首先清空控件上已有的内容
        int index = 0;
        QString tmpStr;
        for(int i=0; i<cmdInfo.length();i+=4)    //4个为一组 前两个字符为地址 后两个地址为值
        {
            QString tmp = valueItem[index].text();
            QString tmp2 =  cmdInfo.mid(2+i,2);

//            qDebug()<<"index = "<<index<<"   tmp="<<tmp<<"  tmp2="<<tmp2;

            if((!tmp.isEmpty()) && (tmp != tmp2))
            {
                valueItem[index].setTextColor(differ_color);
            }else
            {
                if(valueItem[index].textColor() != alter_color)
                {
                    valueItem[index].setTextColor(black_color);
                }


            }



            addressItem[index].setText(cmdInfo.mid(i,2));
            valueItem[index].setText(cmdInfo.mid(i+2,2));
            index++;
        }


        //全部加载完以后，建立信号与槽的连接
        connect(ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(tableWidget_cellChanged(int,int)));
    }
}




//窗口关闭
/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-28
*@brief:
*/
void RegisiterSetDialog::closeEvent(QCloseEvent *event)
{
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),0,0);
    clearItem();   //首先清空内容
}


void RegisiterSetDialog::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

/**
*@projectName Vangogh_V1
*@auther zwt
*@date 2020-04-29
*@brief:  触发时   标识用户进行了修改，将字体修改为蓝色
*    5A 01 LL LL DD DD CC
*/
void RegisiterSetDialog::tableWidget_cellChanged(int row, int column)
{
//    qDebug()<<"row = "<<row<<" column ="<<column<<" cell changed";

//    int regIndex = column * row_show + row;
//    QString regStr = addressItem[regIndex].text();

    int valueIndex = column/2 * row_show + row;
//    QString tmpStr = valueItem[valueIndex].text();

//    QString regStr = addressItem[valueIndex].text();

//    qDebug()<<" regStr = "<<regStr<<"   tmpStr="<<tmpStr;
    valueItem[valueIndex].setTextColor(alter_color);

//    QString cmdStr = "5A 01 03 00 01 ";




}

void RegisiterSetDialog::on_test_pushButton_clicked()
{

    //发送完数据信息之后，断开信号与槽的连接,接收完数据之后重新建立连接
    disconnect(ui->tableWidget,SIGNAL(cellChanged(int,int)),0,0);

    QString returnCmd = "8001";
    QString cmdInfo = "00112233445566778899";
    AckCmdRegister_slot(returnCmd,cmdInfo);


    ui->tableWidget->setCurrentCell(25,19);

}


void RegisiterSetDialog::on_tableWidget_cellClicked(int row, int column)
{
    qDebug()<<"on_tableWidget_cellClicked row="<<row<<",col="<<column;
    select_single_row = row;
    select_single_col = column;
}

void RegisiterSetDialog::on_singleSend_pushButton_clicked()
{
    int Index = select_single_col/2 * row_show + select_single_row;
    QString tmpStr = valueItem[Index].text();
    QString regStr = addressItem[Index].text();

    qDebug()<<"regStr = "<<regStr<<"  tmpStr ="<<tmpStr;

    QString cmdStr = "5A 01 03 00 01 ";
    cmdStr.append(regStr).append(tmpStr);
    emit sendSerialSignal(cmdStr);


}
