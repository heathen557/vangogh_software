#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtSerialPort/QSerialPort>
#include<QtSerialPort/QSerialPortInfo>
#include<QDebug>
#include<QDataStream>
#include<vector>
#include<QTimer>
#include<QList>
#include<QMessageBox>
#include<QThread>
#include"qcustomplot.h"
#include<QSettings>

struct Settings {
    QString name;
    qint32 baudRate;
    QString stringBaudRate;
    QSerialPort::DataBits dataBits;
    QString stringDataBits;
    QSerialPort::Parity parity;
    QString stringParity;
    QSerialPort::StopBits stopBits;
    QString stringStopBits;
    QSerialPort::FlowControl flowControl;
    QString stringFlowControl;
    bool localEchoEnabled;
};


using namespace std;



#endif // GLOBALDATA_H
