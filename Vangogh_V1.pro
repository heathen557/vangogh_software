#-------------------------------------------------
#
# Project created by QtCreator 2020-04-20T16:19:53
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Vangogh_V6.7
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    receserial_msg.cpp \
    globaldata.cpp \
    qcustomplot.cpp \
    cassathistogram_dialog.cpp \
    regisitersetdialog.cpp \
    distancetest_dialog.cpp \
    rowdata_dialog.cpp \
    channekparameter_dialog.cpp \
    autostepping_dialog.cpp \
    dcr_test_dialog.cpp \
    delayline_dialog.cpp \
    windowsetting_dialog.cpp

HEADERS  += mainwindow.h \
    receserial_msg.h \
    globaldata.h \
    qcustomplot.h \
    cassathistogram_dialog.h \
    regisitersetdialog.h \
    distancetest_dialog.h \
    rowdata_dialog.h \
    channekparameter_dialog.h \
    autostepping_dialog.h \
    dcr_test_dialog.h \
    delayline_dialog.h \
    windowsetting_dialog.h

FORMS    += mainwindow.ui \
    cassathistogram_dialog.ui \
    regisitersetdialog.ui \
    distancetest_dialog.ui \
    rowdata_dialog.ui \
    channekparameter_dialog.ui \
    autostepping_dialog.ui \
    dcr_test_dialog.ui \
    delayline_dialog.ui \
    windowsetting_dialog.ui

RESOURCES += \
    images/img.qrc


OTHER_FILES += myapp.rc
RC_FILE += myapp.rc
