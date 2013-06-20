#include "mainwindow.h"
#include <QApplication>
#include "dynamixel.h"
#include "loadcell.h"
#include "configfilemanager.h"
#include "testdata.h"
#include <QtXml/QDomDocument>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    QDomElement e;
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
    w.setSpeedMax(30.0);
    w.setSpeedMin(0.1);
    QVector<float> d(3,10.0);
    w.addPoint(d);
    w.addPoint(d);
    w.addPoint(d);
    return a.exec();
}
