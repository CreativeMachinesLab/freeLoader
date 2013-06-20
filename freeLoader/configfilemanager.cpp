#include "configfilemanager.h"
#include <QFile>
#include <QDomElement>
#include <QDebug>
ConfigFileManager::ConfigFileManager(QObject *parent): QObject(parent)
{
    ConfigFileDir_ = QDir(QDir::currentPath()); // +"\\\\configs"
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


QStringList ConfigFileManager::getFileNames(){// returns list of config files
    return files_;
}


QDomNode ConfigFileManager::loadFile(QString name){
    /// Open file, load into a DOM structure
    /// emit error if failed



      qDebug()<<"\n\nopening file";
     //open up the file in 'name'
     QString fname = name;
     QFile theConfigFile(fname);
     QDomDocument configDomDocument;
     QString error;
     int line, col;
     if( !configDomDocument.setContent( &theConfigFile, &error, &line , &col) )
     {
       qDebug()<<"Failed to parse the file"<<fname<<"into a DOM tree:" ;
       qDebug()<<error<<" at line"<<line<<", column "<<col;
       theConfigFile.close();
       QDomElement EmptyElement;
       return EmptyElement;
     }

     QDomElement configDomElement = configDomDocument.documentElement();

     return configDomElement;

 }

