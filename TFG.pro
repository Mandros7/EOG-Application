#-------------------------------------------------
#
# Project created by QtCreator 2015-02-04T21:11:26
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia
QT       += webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TFG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    musicdialog.cpp \
    inputdialog.cpp \
    settingsdialog.cpp

HEADERS  += mainwindow.h \
    musicdialog.h \
    settingsdialog.h \
    inputdialog.h

FORMS    += mainwindow.ui \
    musicdialog.ui \
    settingsdialog.ui \
    inputdialog.ui
