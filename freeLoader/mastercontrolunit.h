#ifndef MASTERCONTROLUNIT_H
#define MASTERCONTROLUNIT_H

#include <QObject>
#include "jogcontroller.h"
#include "experimentcontroller.h"
#include "configfilemanager.h"
#include "gantry.h"
#include "mainwindow.h"
#include "configdialog.h"

class MasterControlUnit : public QObject
{
    Q_OBJECT
public:
    explicit MasterControlUnit(QObject *parent = 0);
    /**
     * Make Mainwindow
     * disable all of mainwindow
     * showmain window
     * promptForConfig
     **/
    
signals:
    
public slots:
    void promtForConfig(); // use configs to get list and prompt user
    void setConfig(QString filename); // make gant, setup connections to UI or loop to prompt
    void beginExperiment(); // start epxeriment and set up connections
    void endExperiment(); // change UI and disable connections
    
public:
    MainWindow *window_;
    ExperimentController *xp_;
    JogController *jog_;
    Gantry *gant_;
    ConfigFileManager *configs_;
    QString config_;

};

#endif // MASTERCONTROLUNIT_H
