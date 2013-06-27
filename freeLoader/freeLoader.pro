#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:46:30
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#include( C:/qwt-6.1.0/examples/examples.pri )

CONFIG+=qt release


TARGET = freeLoader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    testdata.cpp \
    dynamixel.cpp \
    loadcell.cpp \
    configfilemanager.cpp \
    motor_funcs.cpp \
    gantry.cpp \
    jogcontroller.cpp \
    experimentcontroller.cpp \
    mastercontrolunit.cpp \
    configdialog.cpp \
    serialdiscoverydialog.cpp\
    plot.cpp\
    curvedata.cpp\
    signaldata.cpp


HEADERS  += mainwindow.h\
        ftd2xx.h\
        motor_funcs.h\
    testdata.h \
    dynamixel.h \
    loadcell.h \
    configfilemanager.h \
    gantry.h \
    jogcontroller.h \
    experimentcontroller.h \
    mastercontrolunit.h \
    configdialog.h \
    serialdiscoverydialog.h\
    plot.h\
    signaldata.h


FORMS    += mainwindow.ui \
    configdialog.ui \
    serialdiscoverydialog.ui

QWT_ROOT = C:/qwt-6.1.0

INCLUDEPATH += $${QWT_ROOT}/src
DEPENDPATH  += $${QWT_ROOT}/src


#I threw the .dll and .a into this directory
#LIBS += -L$$quote($$PWD/lib) -lftd2xx
LIBS += -L$$quote($$PWD/lib) -lftd2xx -lqwt
#LIBS += -L$$quote($$PWD/lib) -lftd2xx -lqwtd -lqwt
#LIBS += -LC:/qwt-6.1.0/lib/libqwt.a
#LIBS += -LC:/qwt-6.1.0/lib/libqwt.dll



#LIBS      += -L$${QWT_ROOT}/lib/libqwt.a
#LIBS      += -L$${QWT_ROOT}/lib/libqwtd.a
#LIBS      += -L$${QWT_ROOT}/lib/qwt.dll
#LIBS      += -L$${QWT_ROOT}/lib/qwtd.dll


MOC_DIR = moc
OBJECTS_DIR = obj
DESTDIR = bin
UI_DIR = ui

OTHER_FILES += \
    default.config \
    mx-64.config\
    todos.txt \
    examples.pri
