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
    motor_funcs.cpp \
    gantry.cpp \
    jogcontroller.cpp


HEADERS  += mainwindow.h\
        ftd2xx.h\
        motor_funcs.h\
        tic_toc.h \
    testdata.h \
    dynamixel.h \
    loadcell.h \
    configfilemanager.h \
    gantry.h \
    jogcontroller.h


FORMS    += mainwindow.ui


LIBS += -L$$quote($$PWD/lib) -lftd2xx

MOC_DIR = moc
OBJECTS_DIR = obj
DESTDIR = bin

OTHER_FILES += \
    default.config
