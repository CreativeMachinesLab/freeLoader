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

    //TESTING
    files_.append("test1.csv");
    files_.append("test2.csv");


}


QStringList ConfigFileManager::getFileNames(){// returns list of config files
    return files_;
}

 QDomElement loadFile(QString name){
    /// Open file, load into a DOM structure
    /// emit error if failed
 }
