include(../chronoline.pri)
QT += core \
    gui
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
TARGET = ChronoLineDemo
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    dialogaedperiod.cpp \
    dialogaedevflag.cpp
HEADERS += mainwindow.h \
    dialogaedperiod.h \
    dialogaedevflag.h
FORMS += mainwindow.ui \
    dialogaedperiod.ui \
    dialogaedevflag.ui

