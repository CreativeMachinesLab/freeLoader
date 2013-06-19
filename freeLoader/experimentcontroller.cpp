#include "experimentcontroller.h"
#include <QDateTime>
#include <QDebug>

ExperimentController::ExperimentController(Gantry *gant, QObject *parent) :
    JogController(gant,parent),type_(kTensile),condi_(kTime),interval_(0.0),percent_(0)
{
    data_ = new TestData(QDateTime::currentDateTimeUtc().toString()+".csv",this);
    startposition_=gant->position;
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopExperiment()));
    connect(percentTimer_,SIGNAL(timeout()),this,SLOT(tic()));

}


ExperimentController::ExperimentController(Gantry *gant, QString filename, QObject *parent):
    JogController(gant, parent),type_(kTensile),condi_(kTime),interval_(0.0),percent_(0)
{
    data_ = new TestData(filename,this);
    startposition_=gant->position;
    connect(stoptimer_,SIGNAL(timeout()),this,SLOT(stopExperiment()));

}

void ExperimentController::setFileName(QString filename)
{
    data_->setFileName(filename);
}

void ExperimentController::startExperiment(){
    if(! ((gant->dyna->isInitialized()) && (gant->cell->isInitialized()))){
        emit unableToStart();
        qDebug()<< "Gant not connected to either dyna or cell";
        return;
    }
    startposition_=gant->position;



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

    if(kTensile==type_){
        move(speed_,time,1);
    }else if (kCompression==type_){
        move(speed_,time,-1);
    }else{
        qDebug()<<"Error unknown test type: "<<type_;
        emit unableToStart();
        disconnect(this,SIGNAL(dataPoint(QVector<float>)),data_,SLOT(addData(QVector<float>)));
        data_->clearData();
        return;
    }
    percentTimer_->setInterval(time*60*10);//*1000/100
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
    emit percentComplete(percent_);
}


void ExperimentController::setTestParameters(TestType type, float speed, EndCondition cond, float interval){
    interval_=interval;
    condi_=cond;
    speed_=speed;
    type_=type;
}


