#include "jogcontroller.h"
#include "tic_toc.h"
#include <math.h>

JogController::JogController(Gantry *gant, QObject *parent) :
    QObject(parent),speed_(1.0), currDirection_(1)
{
    stoptimer_->setSingleShot(true);
    updatetimer_->setInterval(gant->updateIntervalInMS);
    connect(updatetimer_,SIGNAL(timeout()),this,SLOT(updateState()));
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopMove()));
}

/// NOTE, We need a method for protecting the gant from being used by mutliple controllers at the same time.
/// In theory you could move the gantry using a jog at the same time an experiment is running.
/// We would need to put a flag in the gant object to signify ownership of the commands by a particular object

float JogController::calculateCurrentPosition(QVector<float> state ){
    /// TO BE WRITTEN BY ETHAN BASED ON THE MATLAB
    //deadzone is angle<5 or angle>1005 clicks
    //if each click is 45/128 degrees, that means
    //less than 1.76 degrees or greater than 353.33 degrees

    //if we're in the deadzone, we'll extrapolate the
    //distance travelled based on the speed reading

    //according to John, we have 61440 clicks/in = 21600 degrees/in
    //=850.39370079 degrees/mm

    int clicksPerInch=gant->dyna->getClicksPerInch();
    int clicksPerRotation=gant->dyna->getcountsPerRev();
    float degreesPermm=clicksPerInch*360/clicksPerRotation/2.54;

    //deadzone floor and ceiling
    float dzCeiling=gant->dyna->getdzCeiling();

    float dzFloor=gant->dyna->getdzFloor();

    //state info
    float currAngle=state[1];
    float lastAngle=lastState_[1];
    float currTime=state[0];
    float lastTime=lastState_[0];

    float dAngle;   //the angle travelled in this timestep
    float dTime;    //amount of time between this step and last step
    float dPos;     //the position travelled in this timestep
    float dPosAbs;  //the absolute value of position travelled, useful when extrapolating through deadzone

    if((currAngle>dzFloor)&&(currAngle<dzCeiling)&&(lastAngle>dzFloor)&&(lastAngle<dzCeiling)){ //if everything is normal
        dAngle=currAngle-lastAngle;
        dPos=dAngle/degreesPermm;
    }else{ //if either end is in the deadzone, then we use the motor spin speed to extrapolate dPos
        dTime=currTime-lastTime;
        dPosAbs=dTime*speed_;  //speed is in mm/min, time is in min
        if(currDirection_>0){ //CW movement
            dPos=dPosAbs;
        }else{                //CCW movement
            dPos=-dPosAbs;
        }
    }

    float lastPos=gant->position; //query the gantry for last position

    return lastPos+dPos; //return updated position
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

void JogController::move(float speed, float timeInMin, int direction){ // we may need to flip directions
    currDirection_=direction;
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
    float position=0;
    float angle= gant->dyna->getAngle();
    float load = gant->cell->readLoad();
    float time = (float)milliseconds();
    QVector<float> state(2,0);
    state[0] = time;
    state[1] = angle;

    position = calculateCurrentPosition(state);

    QVector<float> datapoint(3,0);
    datapoint[0]=time;
    datapoint[1]=position;
    datapoint[2]=load;

    gant->position=position;
    lastState_=state;
    emit dataPoint(datapoint);



}
