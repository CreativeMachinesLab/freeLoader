#include "dynamixel.h"
#include "ftd2xx.h"
#include "motor_funcs.h"
#include <QDebug>

Dynamixel::Dynamixel(QDomNode confignode, QObject *parent) :
    QObject(parent),initialized_(false), motorNumber_(1),countsPerRevolution_(1005),
    alpha_(-39.7760880746),beta_(0.0362830709),
    maxSpeedCW_(2045),minSpeedCW_(1025),
    maxSpeedCCW_(1022),minSpeedCCW_(2)
{
    // LOAD CONFIG FILE
    if("dynamixel"==confignode.nodeName().toLower()){
        QDomNodeList locChildren = confignode.childNodes();
        for (unsigned int j = 0; j < locChildren.length(); j++) {
            QDomNode lchild = locChildren.at(j);
            if ("serial"==lchild.nodeName().toLower()) {

                //probably not the most elegant way to do this...
                QString tempString = QString::fromStdString(lchild.nodeName().toStdString());
                strcpy(serialNumber_,tempString.toStdString().c_str());


            }else if("motor"==lchild.nodeName().toLower()){
                motorNumber_ = lchild.nodeValue().toInt();
            }else if("countersperrev"==lchild.nodeName().toLower()){
                countsPerRevolution_ = lchild.nodeValue().toFloat();
            }else if("defaultalpha"==lchild.nodeName().toLower()){
                alpha_ = lchild.nodeValue().toFloat();
            }else if("defaultbeta"==lchild.nodeName().toLower()){
                beta_ = lchild.nodeValue().toFloat();
            }else if("maxcw"==lchild.nodeName().toLower()){
                maxSpeedCW_ = lchild.nodeValue().toFloat();
            }else if("mincw"==lchild.nodeName().toLower()){
                minSpeedCW_ = lchild.nodeValue().toFloat();
            }else if("maxccw"==lchild.nodeName().toLower()){
                maxSpeedCCW_ = lchild.nodeValue().toFloat();
            }else if("minccw"==lchild.nodeName().toLower()){
                minSpeedCCW_ = lchild.nodeValue().toFloat();
            }
        }
    } else {
        qDebug()<<"Dynamixel: Could not read config file";
    }


  //  strcpy(serialNumber_,"AD01UY23");//"A600cUyt"
    qDebug()<< "name:"<< serialNumber_;
}

Dynamixel::~Dynamixel(){
    if(initialized_){
        FT_STATUS ftStatus = FT_Close(ftHandleDYNA_);
        if (ftStatus != FT_OK){
            emit failedToClose();
        }else{
            emit closed();
        }
    }
}


bool Dynamixel::isInitialized(){
    return initialized_;
}

float Dynamixel::getAngle(){
    if(initialized_){
        float angle =(float)read_encoder(ftHandleDYNA_, motorNumber_);
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
    return (speedInMMperMin-beta_)/alpha_;
}
float Dynamixel::internalSpeedToSpeed(int speedinticks){
    return alpha_*speedinticks+beta_;
}

void Dynamixel::connect(){ // Initiallizes the motor and generates the ftHandleDYNA
    FT_STATUS ftStatus = FT_OpenEx(serialNumber_,FT_OPEN_BY_SERIAL_NUMBER,&ftHandleDYNA_);
    if (ftStatus != FT_OK)
    {
        qDebug()<<"failed to open";
        emit failedToOpen();
        Sleep(2000);
        return;
    }

    //Initialize the USB2Dynamixel

    initialized_=DYNA_initialize(ftHandleDYNA_);
    if(!initialized_){
        qDebug()<<"failed to initialize";
        emit failedToOpen();
        return;
    }
    Sleep(50);
    qDebug()<< "OPENED!";
}

void Dynamixel::setSpeed(float mmPerMin){ // Sets the speed of the unit and runs any future PID control loop. +=CW -=CCW
    if(!initialized_){return;}
    int speed = speedToInternalSpeed(mmPerMin);
    motor_spin(ftHandleDYNA_,motorNumber_,speed);
    qDebug()<<"Speed: "<<speed;
}

void Dynamixel::stop(){
    if(!initialized_){return;}
    motor_spin(ftHandleDYNA_,motorNumber_,0);
}
