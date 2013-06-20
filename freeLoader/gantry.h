#ifndef GANTRY_H
#define GANTRY_H

#include <QObject>
#include <QtXml/QDomElement>
#include "dynamixel.h"
#include "loadcell.h"
#include <QVector>
#include "tic_toc.h"

class Gantry : public QObject
{
    Q_OBJECT
public:
    explicit Gantry(QDomNode configfile, QObject *parent = 0);
    // this will create the Dynamizel and Loacell at startup
    // w1ill connect loadcell.maxreached with dyanmixel.stop
    // will connect at startio



public:
    Dynamixel* dyna;
    LoadCell* cell;
    float mmPerRev;
    float position;
    int updateIntervalInMS;




};

#endif // GANTRY_H
