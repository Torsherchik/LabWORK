include(gtest_dependency.pri)

INCLUDEPATH += E:\lab9\lab9prog
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS += \
        ../../lab9prog/IKeypad.h \
        ../../lab9prog/ILatch.h \
        ../../lab9prog/lockcontroller.h \
        tst_onetest.h

SOURCES += \
        ../../lab9prog/lockcontroller.cpp \
        main.cpp
