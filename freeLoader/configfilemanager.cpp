#include "configfilemanager.h"

ConfigFileManager::ConfigFileManager(QObject *parent = 0): QObject(parent)
{
    ConfigFileDir_ = QDir(QDir::currentPath()+"\\\\configs");
    if(!ConfigFileDir_.exists()){
        emit noConfigs();
    }else{
        QStringList filters;
        filters<<"*.config"<<"*.xml";
        ConfigFileDir_.setNameFilters(filters);
        files_ = ConfigFileDir_.entryList();
        if(files_.size()==0){
            emit noConfigs();
        }
    }


}


QList<QString> ConfigFileManager::getFileNames(){// returns list of config files
    return files_;
}

QVector<QDomElement> ConfigFileManager::parseFile(QString name){//returns dynamixel and loadcell configs
    if (!files_.contains(name)){return QVector<QDomElement>();}

    /// NEED TO WRITE CONFIG FILE PARSER
}
