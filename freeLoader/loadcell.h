#ifndef LOADCELL_H
#define LOADCELL_H

#include <QObject>
#include <QDomDocument>
#include <Windows.h>

class LoadCell : public QObject
{
    Q_OBJECT
public:
    explicit LoadCell(QObject *parent = 0, QDomElement config);

    bool Open();
    float readLoad();

signals:
    void failedToOpen();
    void opened();
    void closed();
    void failedToClose();
    void failedToRead();
    
public slots:
    
private:
    QString comID_;
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
    float mlbfToN;
};

#endif // LOADCELL_H
