#include "dynamixel.h"

Dynamixel::Dynamixel(QObject *parent, QDomElement configfile) :
    QObject(parent),initialized_(false), motorNumber_(1),countsPerRevolution_(1005),
    alpha_(-39.7760880746),beta_(0.0362830709),
    maxSpeedCW_(2045),minSpeedCW_(1025),
    maxSpeedCCW_(1022),minSpeedCCW_(2)
{
    // LOAD CONFIG FILE
}


float Dynamixel::getAngle(){
    if(initialized_){
        float angle = float(read_encoder(ftHandleDYNA_, motorNumber_));
        return angle/countsPerRevolution_*360;
    }
    return 0;
}

// Internal values from config. Used for UI display
float Dynamixel::getMaxSpeedCCW(){
    return internalSpeedToSpeed(maxSpeedCCW_);
}
float Dynamixel::getMaxSpeedCW(){
    return internalSpeedToSpeed(maxSpeedCW_);
}
float Dynamixel::getMinSpeedCCW(){
    return internalSpeedToSpeed(minSpeedCCW_);
}
float Dynamixel::getMinSpeedCW(){
    return internalSpeedToSpeed(minSpeedCW_);
}
void Dynamixel::setAlpha(float a){alpha_=a;}
void Dynamixel::setBeta(float b){beta_=b;}

int Dynamixel::speedToInternalSpeed(float speedInMMperMin){
    return (speedInMMperMin-beta)/alpha;
}
float Dynamixel::internalSpeedToSpeed(int speedinticks){
    return alpha*speedinticks+beta;
}

void Dynamixel::connect(){ // Initiallizes the motor and generates the ftHandleDYNA

}
void Dynamixel::setSpeed(float mmPerMin){} // Sets the speed of the unit and runs any future PID control loop. +=CW -=CCW
