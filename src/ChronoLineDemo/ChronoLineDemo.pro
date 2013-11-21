#-------------------------------------------------
#
# Project created by QtCreator 2013-10-30T18:21:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChronoLineDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ../chronoline.cpp \
    ../cltimeline.cpp

HEADERS  += mainwindow.h \
    ../chronoline.h \
    ../cltimeline.h \
    ../cldefs.h

FORMS    += mainwindow.ui
