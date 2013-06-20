#include "mastercontrolunit.h"

MasterControlUnit::MasterControlUnit(QObject *parent) :
    QObject(parent)
{
}

void MasterControlUnit::promtForConfig(){}
void MasterControlUnit::setConfig(QString filename){}
void MasterControlUnit::beginExperiment(){}
void MasterControlUnit::endExperiment(){}
