include(../chronoline.pri)
QT += core \
    gui
greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
TARGET = ChronoLineDemo
TEMPLATE = app
SOURCES += main.cpp \
    mainwindow.cpp \
    dialogaedperiod.cpp \
    dialogaedevflag.cpp \
    dialogmanagepf.cpp
HEADERS += mainwindow.h \
    dialogaedperiod.h \
    dialogaedevflag.h \
    dialogmanagepf.h
FORMS += mainwindow.ui \
    dialogaedperiod.ui \
    dialogaedevflag.ui \
    dialogmanagepf.ui

unix {
 UI_DIR = .ui
 MOC_DIR = .moc
 OBJECTS_DIR = .obj
}

