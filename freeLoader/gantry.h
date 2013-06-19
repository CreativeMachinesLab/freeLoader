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
    // w1ill connect loadcell.maxreached with dyanmixel.stop
    // will connect at startio

    bool hasConnectedDyna();
    bool hasConnectedCell();
    bool initialized();

    float position();
    float getMMPerRev();

public slots:
    void setPosition(float positionInMM);


public:
    Dynamixel* dyna;
    LoadCell* cell;

private:
    // Internal config
    float mmPerRev_;
    float position_;

};

#endif // GANTRY_H
