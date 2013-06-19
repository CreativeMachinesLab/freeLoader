#include "loadcell.h"
#include <QDebug>
#include <stdio.h>

LoadCell::LoadCell(QDomElement config, QObject *parent) :
    QObject(parent),
    initialized_(false),
    ReadIntervalTimeout_(50),
    ReadTotalTimeoutConstant_(50),
    ReadTotalTimeoutMultiplier_(10),
    WriteTotalTimeoutConstant_(50),
    WriteTotalTimeoutMultiplier_(10),
    BaudRate_(CBR_9600),
    ByteSize_(8),
    StopBits_(ONESTOPBIT),
    Parity_(NOPARITY),
    maxForce_(12),
    mlbfToN(0.00444822162)

{
    ///SET THE CONFIGURATION FROM THE CONFIG FILE
    ///    std::wcin >> comNum;
        std::wstring comNum = L"com14";
        std::wstring comPrefix = L"\\\\.\\";
        comID_ = comPrefix+comNum;
}



bool LoadCell::open(){

//Open serial port at comID
    hSerial_ = CreateFileW(	comID_.c_str(),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            0);
    if(hSerial_==INVALID_HANDLE_VALUE)
    {
        qDebug()<<"failed to open loadcell ";
        emit failedToOpen();
        Sleep(2000);
        return false;
    }

//Set serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength=sizeof(dcbSerialParams);

    if (!GetCommState(hSerial_, &dcbSerialParams))
    {
        emit failedToOpen();
        qDebug() << "\n\nError getting port state!";
        Sleep(2000);
        return false;
    }

    dcbSerialParams.BaudRate=BaudRate_;
    dcbSerialParams.ByteSize=ByteSize_;
    dcbSerialParams.StopBits=StopBits_;
    dcbSerialParams.Parity=Parity_;

    if(!SetCommState(hSerial_, &dcbSerialParams))
    {
        emit failedToOpen();
        qDebug() << "\n\nError setting port state!";
        Sleep(2000);
        return false;
    }

//Setting timeouts
    COMMTIMEOUTS timeouts={0};
    timeouts.ReadIntervalTimeout=ReadIntervalTimeout_;
    timeouts.ReadTotalTimeoutConstant=ReadTotalTimeoutConstant_;
    timeouts.ReadTotalTimeoutMultiplier=ReadTotalTimeoutMultiplier_;
    timeouts.WriteTotalTimeoutConstant=WriteTotalTimeoutConstant_;
    timeouts.WriteTotalTimeoutMultiplier=WriteTotalTimeoutMultiplier_;

    if(!SetCommTimeouts(hSerial_, &timeouts))
    {
        emit failedToOpen();
        qDebug() << "\n\nError setting timeouts!";
        Sleep(2000);
        return false;
    }

//Write a <CR> to the port.
    DWORD dwBytesWrite = 0;
    if(!WriteFile(hSerial_, "\r", 1, &dwBytesWrite, NULL))
    {
        emit failedToOpen();
        qDebug() << "\n\n error occurred writing CR to port";
        Sleep(2000);
        return false;
    }

    Sleep (200);

//Read the input buffer and discard till there are no characters to read.
    char szBuff[5] = {0};
    DWORD dwBytesRead = 0;
    if(!ReadFile(hSerial_, szBuff, 4, &dwBytesRead, NULL))
    {
        emit failedToOpen();
        qDebug() << "error occurred reading the input buffer";
        Sleep(2000);
        return false;
    }

    initialized_=true;
    emit opened();
    qDebug()<<"Opened Load Cell ";
}
float LoadCell::readLoad(){
    if(!initialized_){return 0;}
    DWORD dwBytesWrite = 0;
    DWORD dwBytesRead = 0;


    //Write O0W1<CR> to the port (this sends a bad load but prepares the good load for sending)
    if(!WriteFile(hSerial_, "O0W1\r", 5, &dwBytesWrite, NULL)){
        emit failedToRead();
        qDebug() << "\n\n error occurred writing O0W1<CR> to port";
    }



    //Read load cell input buffer and discard
    char SzBuff[13] = {0};
    dwBytesRead = 0;
    if(!ReadFile(hSerial_, SzBuff, 12, &dwBytesRead, NULL)){
        emit failedToRead();
        qDebug() << "error occurred reading the load cell input buffer";
    }


    //Write O0W1<CR> to the port (this sends the good load and prepares a the bad one for next loop)
    if(!WriteFile(hSerial_, "O0W1\r", 5, &dwBytesWrite, NULL)){
        emit failedToRead();
        qDebug() << "\n\n error occurred writing O0W1<CR>  to port";
    }



    //Read load cell input buffer and keep it
    char SZBuff[13] = {0};
    dwBytesRead = 0;
    if(!ReadFile(hSerial_, SZBuff, 12, &dwBytesRead, NULL)){
        emit failedToRead();
        qDebug()<< "error occurred reading the load cell input buffer";
    }


    //Convert load cell buffer to Newtons
    float load = mlbfToN*atof(SZBuff);
    if(load >=maxForce_){
        qDebug()<<"Max Force reached";
        emit maxForceReached();
    }
    return load;
}
