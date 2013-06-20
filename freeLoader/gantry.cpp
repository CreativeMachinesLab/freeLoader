#include "gantry.h"

Gantry::Gantry(QDomNode configfile, QObject *parent) :
    QObject(parent),mmPerRev(1.0),position(0.0),updateIntervalInMS(100)
{
    ///PARSE CONFIG FILE,
    ///MAKE DYNA AND CELL
    ///Connect DYNA AND CELL
    ///Connect cell.maxforce to dyna.stop
    QDomElement e;
    dyna= new Dynamixel(e);
    cell = new LoadCell(e);
//    dyna->connect();
//    cell->open();

}


