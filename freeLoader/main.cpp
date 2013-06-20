#include "mainwindow.h"
#include <QApplication>
#include "dynamixel.h"
#include "loadcell.h"
#include "configfilemanager.h"
#include "testdata.h"
#include "gantry.h"
#include <QtXml/QDomDocument>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ConfigFileManager conf;
    QDomElement e=conf.loadFile("default.config");
    Gantry * gant;
    gant = new Gantry(e);
    //qDebug()<<gant->mmPerRev;

    //MainWindow w;
    //w.show();
    

    /*Dynamixel d( e,0);
    d.connect();
    qDebug()<<"angle: "<<d.getAngle();
    d.setAlpha(1.0);
    d.setBeta(0.0);
    d.setSpeed(250);
    Sleep(1000);
    qDebug()<<"angle: "<<d.getAngle();
    d.stop();*/

    //LoadCell l(e,0);
    //l.open();

    //qDebug()<<"Load: "<<l.readLoad();
   // w.disableJog(true);
    return a.exec();
}
