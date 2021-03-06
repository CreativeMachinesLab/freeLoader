#ifndef CONFIGFILEMANAGER_H
#define CONFIGFILEMANAGER_H

#include <QObject>
#include <QDir>
#include <QtXML>
#include <QVector>

class ConfigFileManager: public QObject
{
    Q_OBJECT
public:
    explicit ConfigFileManager(QObject *parent = 0);
    QStringList getFileNames();// returns list of config files
    QDomNode loadFile(QString name);//returns dynamixel and loadcell configs

signals:
    void noConfigs();
    void failedToOpenFile();

private:
    QDir ConfigFileDir_;
    QStringList files_;
};

#endif // CONFIGFILEMANAGER_H
