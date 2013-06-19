#include "jogcontroller.h"
#include "tic_toc.h"
#include <math.h>

JogController::JogController(Gantry *gant, QObject *parent) :
    QObject(parent),speed_(1.0)
{
    stoptimer_->setSingleShot(true);
    updatetimer_->setInterval(gant->updateIntervalInMS);
    connect(updatetimer_,SIGNAL(timeout()),this,SLOT(updateState()));
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopMove()));
}

/// NOTE, We need a method for protecting the gant from being used by mutliple controllers at the same time.
/// In theory you could move the gantry using a jog at the same time an experiment is running.
/// We would need to put a flag in the gant object to signify ownership of the commands by a particular object

float JogController::calculateCurrentPosition(float angle){
    /// TO BE WRITTEN BY ETHAN BASED ON THE MATLAB
}

void JogController::setSpeed(float speed){ // speed is in ABS values here
    speed_=fabs(speed);
}
void JogController::setHome(){
    setHome(0);
}
void JogController::setHome(float distinmm){
    gant->position=distinmm;
}
void JogController::goHome(){
    float time = fabs(gant->position/speed_);
    int direction = (int) gant->position/fabs(gant->position);
    move(speed_,time,direction);
}

void JogController::jogUp(){
    move(speed_,0.1*1000,1);
}
void JogController::jogDown(){
    move(speed_,0.1*1000,-1);
}

void JogController::move(float speed, float timeInMin, int){ // we may need to flip directions
    stoptimer_->setInterval(timeInMin*60.0*1000.0);
    stoptimer_->start();
    speed_=speed;
    startMove();
}

void JogController::startMove(){
    updatetimer_->start();
    gant->dyna->setSpeed(speed_);
}
void JogController::stopMove(){
    gant->dyna->stop();
    updatetimer_->stop();
    stoptimer_->stop();
}


void JogController::updateState(){
    float position = calculateCurrentPosition( gant->dyna->getAngle());
    float load = gant->cell->readLoad();
    float time = (float)milliseconds();
    QVector<float> state(3,0);
    state[0] = time;
    state[1] = position;
    state[2] = load;

    gant->position=position;
    lastState_=state;
    emit dataPoint(state);



}
