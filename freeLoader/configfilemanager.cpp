#include "configfilemanager.h"
#include <QFile>

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

 QDomElement loadFile(QString name){
    /// Open file, load into a DOM structure
    /// emit error if failed

     //open up the file in 'name'
     QFile theConfigFile(name);
     QDomDocument configDomDocument;
     if( !configDomDocument.setContent( &theConfigFile ) )
     {
       qDebug( "Failed to parse the file into a DOM tree." );
       theConfigFile.close();
       QDomElement EmptyElement;
       return EmptyElement;
     }

     QDomElement configDomElement = configDomDocument.documentElement();

     return configDomElement;

 }
