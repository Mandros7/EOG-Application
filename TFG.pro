#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T21:11:26
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += webkitwidgets
QT       += serialport
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TFG
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    musicdialog.cpp \
    inputdialog.cpp \
    settingsdialog.cpp \
    #BluetoothReader.cpp \
    btreaderthread.cpp \
    dataparserthread.cpp \
    datatreatmentthread.cpp \
    decisionthread.cpp \
    bluetoothwidget.cpp \
    btsettingsdialog.cpp \
    #smtp.cpp

HEADERS  += mainwindow.h \
    musicdialog.h \
    settingsdialog.h \
    inputdialog.h \
    #BluetoothReader.h \
    btreaderthread.h \
    dataparserthread.h \
    datatreatmentthread.h \
    decisionthread.h \
    bluetoothwidget.h \
    btsettingsdialog.h \
    #smtp.h

FORMS    += mainwindow.ui \
    musicdialog.ui \
    settingsdialog.ui \
    inputdialog.ui \
    #BluetoothReader.ui \
    bluetoothwidget.ui \
    btsettingsdialog.ui
