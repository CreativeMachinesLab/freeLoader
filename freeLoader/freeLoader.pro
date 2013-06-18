#-------------------------------------------------
#
# Project created by QtCreator 2013-06-18T12:46:30
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = freeLoader
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    testdata.cpp \
    dynamixel.cpp \
    loadcell.cpp \
    configfilemanager.cpp \
    motor_funcs.cpp


HEADERS  += mainwindow.h\
        ftd2xx.h\
        motor_funcs.h\
        tic_toc.h \
    testdata.h \
    dynamixel.h \
    loadcell.h \
    configfilemanager.h


FORMS    += mainwindow.ui


win32: LIBS += -LC:/Users/Ethan/Documents/GitHub/freeLoader/libs -lftd2xx

INCLUDEPATH += C:/Users/Ethan/Documents/GitHub/freeLoader/libs
DEPENDPATH += C:/Users/Ethan/Documents/GitHub/freeLoader/libs


win32: PRE_TARGETDEPS += C:/Users/Ethan/Documents/GitHub/freeLoader/libs/ftd2xx.lib
