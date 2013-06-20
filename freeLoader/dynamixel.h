#ifndef DYNAMIXEL_H
#define DYNAMIXEL_H

#include <QObject>
#include "ftd2xx.h"
#include "motor_funcs.h"
#include <QtXML/QDomDocument>


typedef char serialnumber[64];


class Dynamixel : public QObject
{
    Q_OBJECT
public:
    explicit Dynamixel( QDomNode confignode,QObject *parent = 0);
    ~Dynamixel();

    float getAngle(); // returns in degree
    bool isInitialized();

    // Internal values from config. Used for UI display
    float getMaxSpeedCCW();
    float getMaxSpeedCW();
    float getMinSpeedCCW();
    float getMinSpeedCW();

    float getdzFloor();
    float getdzCeiling();

    float getcountsPerRev();

public slots:
    void connect(); // Initiallizes the motor and generates the ftHandleDYNA
    void setSpeed(float mmPerMin); // Sets the speed of the unit and runs any future PID control loop. +=CW -=CCW
    void stop();


signals:
    void opened();
    void failedToOpen();
    void closed();
    void failedToClose();
    void failedToRead();

private:
    int speedToInternalSpeed(float speedInMMperMin);
    float internalSpeedToSpeed(int speedinticks, int direction);

    serialnumber serialNumber_;
    FT_HANDLE ftHandleDYNA_;
    bool initialized_;

    // Set in Config file
    int motorNumber_;
    float countsPerRevolution_;
    float alphaCW_;
    float betaCW_;
    float alphaCCW_;
    float betaCCW_;
    int dzFloor_;   //deadzone floor and ceiling in ticks
    int dzCeiling_;
    float maxSpeedCW_; // in internalspeed
    float minSpeedCW_;// in internalSpeed
    float maxSpeedCCW_;// in internalSpeed
    float minSpeedCCW_;// in InternalSpeed

};

#endif // DYNAMIXEL_H
