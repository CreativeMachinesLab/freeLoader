#include "gantry.h"

Gantry::Gantry(QDomElement configfile, QObject *parent) :
    QObject(parent),mmPerRev(1.0),position(0.0)
{
    ///PARSE CONFIG FILE,
    ///MAKE DYNA AND CELL
    ///Connect DYNA AND CELL
    ///Connect cell.maxforce to dyna.stop

}


