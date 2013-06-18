#include "configfilemanager.h"

ConfigFileManager::ConfigFileManager(QObject *parent): QObject(parent)
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
    QVector<QDomElement> parsed;
    if (!files_.contains(name)){return parsed;}
    return parsed;
    /// NEED TO WRITE CONFIG FILE PARSER
}
