#ifndef GANTRY_H
#define GANTRY_H

#include <QObject>
#include <QtXml/QDomElement>
#include "dynamixel.h"
#include "loadcell.h"
#include <QVector>


class Gantry : public QObject
{
    Q_OBJECT
public:
    explicit Gantry(QDomElement configfile, QObject *parent = 0);
    // this will create the Dynamizel and Loacell at startup
    // will connect loadcell.maxreached with dyanmixel.stop
    // will connect at startio

    bool hasDyna();
    bool hasCell();
    bool initialized();

    float position();
    float getRange();
    
public slots:
    void setPosition(float positionInMM);


public:
    Dynamixel* dyna;
    LoadCell* cell;

private:
    // Internal config
    float mmPerRev_;
    float range_;
    float position_;

};

#endif // GANTRY_H
