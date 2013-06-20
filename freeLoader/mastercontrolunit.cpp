#include "mastercontrolunit.h"
#include <QDebug>

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
    window_=new MainWindow();
    window_->show();
    window_->setDisabled(true);

    configs_=new ConfigFileManager();
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
}
void MasterControlUnit::beginExperiment(){}
void MasterControlUnit::endExperiment(){}
