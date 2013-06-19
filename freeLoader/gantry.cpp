#include "gantry.h"

Gantry::Gantry(QDomElement configfile, QObject *parent) :
    QObject(parent),mmPerRev_(1.0),position_(0.0)
{
    ///PARSE CONFIG FILE,
    ///MAKE DYNA AND CELL
    ///Connect DYNA AND CELL
    ///Connect cell.maxforce to dyna.stop

}


bool Gantry::hasConnectedDyna(){
    return dyna->isInitialized();
}
bool Gantry::hasConnectedCell(){
    return cell->isInitialized();
}
bool Gantry::initialized(){
    return hasConnectedCell()&&hasConnectedDyna();
}

float Gantry::position(){
    return position_;
}

void Gantry::setPosition(float positionInMM){
    position_=positionInMM;
}

void Gantry::getMMPerRev(){
    return mmPerRev_;
}

