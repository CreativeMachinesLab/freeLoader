#include "mastercontrolunit.h"
#include <QMessageBox>
#include <QDebug>
#include <QtXml>

bool TESTING = false;

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    //DEFAULT CONSTRUCTION
    window_=new MainWindow();
    configs_=new ConfigFileManager(this);


    QDomNode n;
    gant_ = new Gantry(n,this);
    jog_ = new JogController(gant_,this);
    xp_ = new ExperimentController(gant_,QDateTime::currentDateTimeUtc().toString().replace(":","-")+".csv", this);

    window_->show();

    window_->disableJog(true);
    window_->disableTesting(true);
    window_->disableTestSettings(true);


    QTimer::singleShot(0,this,SLOT(promtForConfig()));
}

void MasterControlUnit::promtForConfig(){
    QStringList l = configs_->getFileNames();

    ConfigDialog *cd = new ConfigDialog(l);
    connect(cd,SIGNAL(selected(QString)),this,SLOT(setConfig(QString)));
    cd->exec();
}
void MasterControlUnit::setConfig(QString filename){
//    qDebug()<<"selected "<< filename;

    delete gant_;
    delete jog_;
    delete xp_;

    QDomNode n= configs_->loadFile(filename);// make gant, setup connections to UI or loop to prompt
    gant_ = new Gantry(n,this);
    jog_ = new JogController(gant_,this);
    xp_ = new ExperimentController(gant_,QDateTime::currentDateTimeUtc().toString().replace(":","-")+".csv", this);
    window_->setFileName(xp_->getFileName());
    connect(gant_->dyna,SIGNAL(failedToClose()),this,SLOT(failedToCloseDyna()));
    connect(gant_->cell,SIGNAL(failedToClose()),this,SLOT(failedToCloseLoadCell()));
    connect(gant_->cell,SIGNAL(failedToRead()),this,SLOT(failedToReadLoadCell()));
    connect(gant_->cell,SIGNAL(maxForceReached()),this,SLOT(forceOverLoad()));



    if(!gant_->dyna->isInitialized()){
        failedToOpenDyna();
    }

    if(!gant_->cell->isInitialized()){
        failedToOpenLoadCell();
    }

    if( !( gant_->dyna->isInitialized() && gant_->cell->isInitialized() ) && (true!=TESTING)){
        QTimer::singleShot(0,this,SLOT(promtForConfig()));
        return;
    }

    // enable ui elements
    window_->disableJog(false);
    window_->disableTesting(false);
    window_->disableTestSettings(false);
//    window_->setDisabled(false);


    if(!gant_->dyna->isInitialized() && (true!=TESTING)){
        window_->disableJog(true);
        window_->disableMovementTest(true);
    }else{
        window_->setSpeedMax(gant_->dyna->getMaxSpeedCW());
        window_->setSpeedMin(gant_->dyna->getMinSpeedCW());
    }
    if(!gant_->cell->isInitialized() && (true!=TESTING)){
        window_->disableTesting(true);
        window_->disableTestSettings(true);
    }

    connect(window_,SIGNAL(jogUp()),jog_,SLOT(jogUp()));
    connect(window_,SIGNAL(jogDown()),jog_,SLOT(jogDown()));
    connect(window_,SIGNAL(setHome()),jog_,SLOT(setHome()));
    connect(window_,SIGNAL(goHome()),jog_,SLOT(goHome()));
    connect(window_,SIGNAL(startXp()),this,SLOT(beginExperiment()));
    connect(window_,SIGNAL(endXp()),this,SLOT(endExperiment()));
    connect(window_,SIGNAL(jogSpeedChanged(float)),jog_,SLOT(setSpeed(float)));

}
void MasterControlUnit::beginExperiment(){
    TestType type = window_->getType();
    EndCondition cond = window_->getEndCond();
    float speed = fabs(window_->getSpeed());
    float interval = fabs(window_->getInterval());
    QString filename = window_->getFileName();

    delete xp_;
    xp_ = new ExperimentController(gant_,filename, this);
    xp_->setTestParameters(type,speed,cond,interval);
    connect(window_,SIGNAL(endXp()),xp_,SLOT(stopExperiment()));
    connect(xp_,SIGNAL(dataPoint(QVector<float>)),window_,SLOT(addPoint(QVector<float>)));
    connect(xp_,SIGNAL(percentComplete(int)),window_,SLOT(setPercent(int)));
    connect(xp_,SIGNAL(experimentCompleted()),this,SLOT(endExperiment()));
    connect(xp_,SIGNAL(experimentCompleted()),window_,SLOT(testEnded()));
    connect(xp_,SIGNAL(experimentStarted()),window_,SLOT(testStarted()));
    connect(xp_,SIGNAL(unableToStart()),this,SLOT(cantStartXP()));
    xp_->startExperiment();

}
void MasterControlUnit::endExperiment(){
//    qDebug()<<"end experiment";
    window_->setFileName(QDateTime::currentDateTimeUtc().toString().replace(":","-")+".csv");
}

void MasterControlUnit::cantStartXP(){
    QMessageBox::warning(window_,"","Failled to Start Experiment");
}
void MasterControlUnit::forceOverLoad(){
    xp_->stopExperiment();
    jog_->stopMove();
    QMessageBox::warning(window_,"","FORCE OVERLOAD!!!!");
}
void MasterControlUnit::failedToOpenDyna(){
    QMessageBox::warning(window_,"","Failed to Open Dynamixel");
}
void MasterControlUnit::failedToCloseDyna(){
    QMessageBox::warning(window_,"","Failled to Close Dynamixel");
}
void MasterControlUnit::failedToOpenLoadCell(){
    QMessageBox::warning(window_,"","Failled to Open Load Cell");
}
void MasterControlUnit::failedToCloseLoadCell(){
    QMessageBox::warning(window_,"","Failled to Close Load Cell");
}
void MasterControlUnit::failedToReadLoadCell(){
    QMessageBox::warning(window_,"","Failled to Read Load Cell");
}
