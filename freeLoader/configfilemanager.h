#ifndef CONFIGFILEMANAGER_H
#define CONFIGFILEMANAGER_H

#include <QObject>
#include <QDir>
#include <QtXML/QDomDocument>
#include <QVector>

class ConfigFileManager: public QObject
{
    Q_OBJECT
public:
    explicit ConfigFileManager(QObject *parent = 0);
    QList<QString> getFileNames();// returns list of config files
    QDomElement loadFile(QString name);//returns dynamixel and loadcell configs

signals:
    void noConfigs();
    void failedToOpenFile();

private:
    QDir ConfigFileDir_;
    QList<QString> files_;
};

#endif // CONFIGFILEMANAGER_H
