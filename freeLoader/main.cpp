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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MasterControlUnit mcu;

    /*
    QStringList l;
    l.append("test.csv");
    ConfigDialog cd(l);
    cd.exec();
    */
    /*
    MainWindow w;
    w.show();
    w.disableJog(true);
    w.disableTesting(true);
    w.disableTestSettings(true);
    */


    /*
    QDomElement e;
    Dynamixel d( e,0);
    d.connect();
    qDebug()<<"angle: "<<d.getAngle();
    d.setAlpha(1.0);
    d.setBeta(0.0);
    d.setSpeed(250);
    Sleep(1000);
    qDebug()<<"angle: "<<d.getAngle();
    d.stop();
    */

    /*
    QDomElement e;
    LoadCell l(e,0);
    l.open();
    qDebug()<<"Load: "<<l.readLoad();
    */
    return a.exec();
}
