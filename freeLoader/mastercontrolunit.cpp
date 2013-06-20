#include "mastercontrolunit.h"
#include <QMessageBox>
#include <QDebug>
#include <QtXml>

bool TESTING = true;

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    //DEFAULT CONSTRUCTION
    window_=new MainWindow();
    configs_=new ConfigFileManager(this);


    QDomNode n;
    gant_ = new Gantry(n,this);
    jog_ = new JogController(gant_,this);
    xp_ = new ExperimentController(gant_, this);

    window_->show();
    window_->setDisabled(true);


    QTimer::singleShot(0,this,SLOT(promtForConfig()));


    /**
     * Make Mainwindow
     * disable all of mainwindow
     * showmain window
     * promptForConfig
     **/
}

void MasterControlUnit::promtForConfig(){
    QStringList l = configs_->getFileNames();

    ConfigDialog *cd = new ConfigDialog(l);
    connect(cd,SIGNAL(selected(QString)),this,SLOT(setConfig(QString)));
    cd->exec();
}
void MasterControlUnit::setConfig(QString filename){
    qDebug()<<"selected "<< filename;

    delete gant_;
    delete jog_;
    delete xp_;

    QDomNode n= configs_->loadFile(filename);// make gant, setup connections to UI or loop to prompt
    gant_ = new Gantry(n,this);
    jog_ = new JogController(gant_,this);
    xp_ = new ExperimentController(gant_, this);


    bool connected=false;
    connected = ( gant_->dyna->isInitialized() && gant_->cell->isInitialized() );
    if( !connected && (true!=TESTING)){
        QMessageBox::warning(window_,"","Failled to Connect to system");
        QTimer::singleShot(0,this,SLOT(promtForConfig()));
        return;
    }

    // enable ui elements
    window_->setDisabled(false);

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
void MasterControlUnit::beginExperiment(){}
void MasterControlUnit::endExperiment(){}
