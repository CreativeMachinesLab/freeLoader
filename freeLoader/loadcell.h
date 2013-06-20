#ifndef LOADCELL_H
#define LOADCELL_H

#include <QObject>
#include <QtXML/QDomDocument>
#include <Windows.h>

class LoadCell : public QObject
{
    Q_OBJECT
public:
    explicit LoadCell( QDomNode confignode, QObject *parent = 0);

    bool open();
    bool isInitialized();
    float readLoad();


signals:
    void failedToOpen();
    void opened();
    void closed();
    void failedToClose();
    void failedToRead();
    void maxForceReached();
    
public slots:
    
private:
    std::wstring comID_;
    HANDLE hSerial_;
    bool initialized_;
    int ReadIntervalTimeout_;
    int ReadTotalTimeoutConstant_;
    int ReadTotalTimeoutMultiplier_;
    int WriteTotalTimeoutConstant_;
    int WriteTotalTimeoutMultiplier_;
    int BaudRate_ ;
    int ByteSize_ ;
    int StopBits_ ;
    int Parity_ ;
    float maxForce_;
    float mlbfToN;
};

#endif // LOADCELL_H
