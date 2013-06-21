#include "experimentcontroller.h"
#include <QDateTime>
#include <QDebug>

bool XPTESTING=false;


ExperimentController::ExperimentController(Gantry *gant, QString filename, QObject *parent):
    JogController(gant, parent),type_(kTensile),condi_(kTime),interval_(0.0),percent_(0)
{
    data_ = new TestData(filename,this);
    percentTimer_ = new QTimer();
    startposition_=gant->position;
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopExperiment()));
    connect(percentTimer_,SIGNAL(timeout()),this,SLOT(tic()));

}

void ExperimentController::setFileName(QString filename)
{
    data_->setFileName(filename);
}

QString ExperimentController::getFileName(){
    return data_->getFileName();
}

void ExperimentController::startExperiment(){
    if(!((gant_->dyna->isInitialized()) && (gant_->cell->isInitialized()))  && (XPTESTING != true)){
        emit unableToStart();
        qDebug()<< "Gant not connected to either dyna or cell";
        return;
    }
    startposition_=gant_->position;



    float time=0;
    if(kTime==condi_){
        time = interval_;
    }else if (kDistance==condi_){
        time = interval_/speed_;
    }else{
        emit unableToStart();
        qDebug()<<"unknown test end condition: "<<condi_;
        return;
    }

    connect(this,SIGNAL(dataPoint(QVector<float>)),data_,SLOT(addData(QVector<float>)));

    qDebug()<< "EXP: TIME: "<<time <<" (min)";
    switch(type_){
    case kTensile:
        move(speed_,time,1);
        break;
    case kCompression:
        move(speed_,time,-1);
        break;
    case kReading:
        if (kTime==condi_){
            move(0,time,1);
            speed_=0;
            break;
        }
        qDebug()<<"Error Can only run time tests for loadcell readings";

    default:
        qDebug()<<"Error  with test type: "<<type_;
        emit unableToStart();
        disconnect(this,SIGNAL(dataPoint(QVector<float>)),data_,SLOT(addData(QVector<float>)));
        data_->clearData();
        return;
    }
    percentTimer_->setInterval(time*60*10);//time in min * 60= time in s  time in s*1000 = time in ms  / 100 = intervals
    qDebug()<<"time:"<< time*60*10;
    percentTimer_->start();
    percent_=0;

    emit experimentStarted();
    emit percentComplete(0);
}
void ExperimentController::stopExperiment(){
    stopMove();
    updateState();
    percentTimer_->stop();
    percent_=100;
    emit percentComplete(100);
    emit experimentCompleted();

    data_->writeToFile();
}

void ExperimentController::tic(){
    percent_++;
    qDebug()<<"tic: "<<percent_;
    emit percentComplete(percent_);
}


void ExperimentController::setTestParameters(TestType type, float speed, EndCondition cond, float interval){
    interval_=interval;
    condi_=cond;
    speed_=speed;
    type_=type;
}


