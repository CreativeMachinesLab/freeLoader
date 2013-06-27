#include "mainwindow.h"
#include <QApplication>
#include "dynamixel.h"
#include "loadcell.h"
#include "configfilemanager.h"
#include "testdata.h"
#include "configdialog.h"
#include "mastercontrolunit.h"
#include <QtXml/QDomDocument>
#include <QDebug>
#include "serialdiscoverydialog.h"
#include <qwt_plot.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MasterControlUnit mcu;

    return a.exec();
}
