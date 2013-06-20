#include "jogcontroller.h"
#include "tic_toc.h"
#include <math.h>
#include <QDebug>

JogController::JogController(Gantry *gant, QObject *parent) :
    QObject(parent),speed_(1.0), currDirection_(1)
{
    gant_=gant;
    stoptimer_=new QTimer();
    updatetimer_ = new QTimer();

    stoptimer_->setSingleShot(true);
    updatetimer_->setInterval(gant->updateIntervalInMS);
    connect(updatetimer_,SIGNAL(timeout()),this,SLOT(updateState()));
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopMove()));
}

/// NOTE, We need a method for protecting the gant from being used by mutliple controllers at the same time.
/// In theory you could move the gantry using a jog at the same time an experiment is running.
/// We would need to put a flag in the gant object to signify ownership of the commands by a particular object

float JogController::calculateCurrentPosition(QVector<float> state ){

    //deadzone is angle<5 or angle>1005 clicks
    //if each click is 45/128 degrees, that means
    //less than 1.76 degrees or greater than 353.33 degrees

    //if we're in the deadzone, we'll extrapolate the
    //distance travelled based on the speed reading

    //according to John, we have 61440 clicks/in = 21600 degrees/in
    //=850.39370079 degrees/mm

    int clicksPerInch=gant_->dyna->getClicksPerInch();
    int clicksPerRotation=gant_->dyna->getcountsPerRev();
    float degreesPermm=clicksPerInch*360/clicksPerRotation/2.54;

    //deadzone floor and ceiling
    float dzCeiling=gant_->dyna->getdzCeiling();

    float dzFloor=gant_->dyna->getdzFloor();

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

    float lastPos=gant_->position; //query the gantry for last position

    return lastPos+dPos; //return updated position
}

void JogController::setSpeed(float speedInMMPerMinABS){ // speed is in ABS values here
    speed_=fabs(speedInMMPerMinABS);
}
void JogController::setHome(){
    stopMove();
    setHome(0);
}
void JogController::setHome(float distinmm){
    gant_->position=distinmm;
}
void JogController::goHome(){
    float time = fabs(gant_->position/speed_);
    int direction = (int) gant_->position/fabs(gant_->position);
    move(speed_,time,direction);
}

void JogController::jogUp(){
    move(speed_,0.1/60.0,1);
}
void JogController::jogDown(){
    move(speed_,0.1/60.0,-1);
}

void JogController::move(float speedInMMPerMinABS, float timeInMin, int direction){ // we may need to flip directions
    currDirection_=direction;
    stoptimer_->setInterval(timeInMin*60.0*1000.0);
    stoptimer_->start();
    speed_=speedInMMPerMinABS;
    startMove();
}

void JogController::startMove(){
    updatetimer_->start();
    gant_->dyna->setSpeed(speed_*currDirection_);
    qDebug()<<"starting move";
}
void JogController::stopMove(){
    gant_->dyna->stop();
    updatetimer_->stop();
    stoptimer_->stop();
    qDebug()<<"ending move";
}


void JogController::updateState(){
    float position=0;
    float angle= gant_->dyna->getAngle();

    float load=0;

    if(gant_->cell->isInitialized()){
         load = gant_->cell->readLoad();
    }
    float time = (float)milliseconds();
    QVector<float> state(2,0);
    state[0] = time;
    state[1] = angle;

    position = calculateCurrentPosition(state);

    QVector<float> datapoint(3,0);
    datapoint[0]=time;
    datapoint[1]=position;
    datapoint[2]=load;

    gant_->position=position;
    lastState_=state;
    emit dataPoint(datapoint);



}
