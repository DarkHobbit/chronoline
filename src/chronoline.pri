# QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += \
    $$PWD

HEADERS += \
    $$PWD/chronoline.h \
    $$PWD/cltimeline.h \
    $$PWD/cldefs.h \
    $$PWD/clperiod.h \
    $$PWD/clflag.h \
    $$PWD/clflagpair.h \
    $$PWD/clselectableobject.h \
    $$PWD/colorautoselector.cpp

SOURCES += \
    $$PWD/chronoline.cpp \
    $$PWD/cltimeline.cpp \
    $$PWD/clperiod.cpp \
    $$PWD/clflag.cpp \
    $$PWD/clflagpair.cpp \
    $$PWD/clselectableobject.cpp \
    $$PWD/colorautoselector.cpp

