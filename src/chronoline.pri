QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
    $$PWD

HEADERS += \
    $$PWD/chronoline.h \
    $$PWD/cltimeline.h \
    $$PWD/cldefs.h \
    $$PWD/clperiod.h \
    $$PWD/clflag.h \
    ../clflagpair.h \
    ../clselectableobject.h

SOURCES += \
    $$PWD/chronoline.cpp \
    $$PWD/cltimeline.cpp \
    $$PWD/clperiod.cpp \
    $$PWD/clflag.cpp \
    ../clflagpair.cpp \
    ../clselectableobject.cpp
