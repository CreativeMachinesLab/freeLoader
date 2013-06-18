#include "loadcell.h"

LoadCell::LoadCell(QObject *parent, QDomElement config) :
    QObject(parent),
    comID_(''),
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
    mlbfToN(0.00444822162)

{
    ///SET THE CONFIGURATION FROM THE CONFIG FILE
    ///    std::wcin >> comNum;
    ///    std::wstring comPrefix = L"\\\\.\\";
    ///    std::wstring comID = comPrefix+comNum;
}



bool LoadCell::Open(){

//Open serial port at comID
    hSerial_ = CreateFileW(	comID.c_str(),
                            GENERIC_READ | GENERIC_WRITE,
                            0,
                            0,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            0);
    if(hSerial_==INVALID_HANDLE_VALUE)
    {
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
        /// std::cerr << "\n\nError getting port state!";
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
        ///std::cerr << "\n\nError setting port state!";
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
        //std::cerr << "\n\nError setting timeouts!";
        Sleep(2000);
        return false;
    }

//Write a <CR> to the port.
    DWORD dwBytesWrite = 0;
    if(!WriteFile(hSerial_, "\r", 1, &dwBytesWrite, NULL))
    {
        emit failedToOpen();
        ///std::cerr << "\n\n error occurred writing CR to port";
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
        ///std::cerr << "error occurred reading the input buffer";
        Sleep(2000);
        return false;
    }

    initialized_=true;
    emit opened();
}
float LoadCell::readLoad(){
    if(!initialized_){return 0;}
    DWORD dwBytesWrite = 0;
    DWORD dwBytesRead = 0;


    //Write O0W1<CR> to the port (this sends a bad load but prepares the good load for sending)
    if(!WriteFile(hSerial_, "O0W1\r", 5, &dwBytesWrite, NULL)){
        emit failedToRead();
        ///std::cerr << "\n\n error occurred writing O0W1<CR> to port";
    }



    //Read load cell input buffer and discard
    char SzBuff[13] = {0};
    dwBytesRead = 0;
    if(!ReadFile(hSerial_, SzBuff, 12, &dwBytesRead, NULL)){
        emit failedToRead();
        ///        std::cerr << "error occurred reading the load cell input buffer";
    }


    //Write O0W1<CR> to the port (this sends the good load and prepares a the bad one for next loop)
    if(!WriteFile(hSerial_, "O0W1\r", 5, &dwBytesWrite, NULL)){
        emit failedToRead();
        ///std::cerr << "\n\n error occurred writing O0W1<CR>  to port";
    }



    //Read load cell input buffer and keep it
    char SZBuff[13] = {0};
    dwBytesRead = 0;
    if(!ReadFile(hSerial_, SZBuff, 12, &dwBytesRead, NULL)){
        emit failedToRead();
        ///std::cerr << "error occurred reading the load cell input buffer";
    }


    //Convert load cell buffer to Newtons
    float load = mlbfToN*atof(SZBuff);
    return load;
}
