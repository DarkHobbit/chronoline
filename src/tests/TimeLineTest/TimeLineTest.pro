#-------------------------------------------------
#
# Project created by QtCreator 2015-12-25T06:42:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeLineTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../cltimeline.cpp

HEADERS  += mainwindow.h \
    ../../cltimeline.h \
    ../../cldefs.h

FORMS    += mainwindow.ui
