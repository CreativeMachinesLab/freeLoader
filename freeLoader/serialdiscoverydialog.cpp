#include "serialdiscoverydialog.h"
#include "ui_serialdiscoverydialog.h"
#include "ftd2xx.h"
#include <QTextStream>

SerialDiscoveryDialog::SerialDiscoveryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialDiscoveryDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);

    QString message;
    QTextStream msg(&message);
    FT_STATUS ftStatus;
    DWORD numDevs;

    ftStatus = FT_ListDevices(&numDevs,NULL,FT_LIST_NUMBER_ONLY);
    if (!ftStatus == FT_OK){
         msg << "\nConnection to FT_ListDevices failed!";
    }else{
        msg<< "\nDetected " << numDevs <<" USB to Serial devices";
        if (numDevs==0){
            msg << "\n\nNo devices found" ;
        }

       //List device serial numbers
        DWORD devIndex;
        char Buffer [(int)numDevs][64]; // more than enough room, maximum 3 USB to serial devices
        for(int i=0;i<numDevs;i++){ //for each device detected
            devIndex = i;
            ftStatus = FT_ListDevices((PVOID)devIndex,Buffer[i],FT_LIST_BY_INDEX|FT_OPEN_BY_SERIAL_NUMBER);
            if (ftStatus == FT_OK)
                msg << "\n Device " << i+1 << " serial number: " << Buffer[i];
            else {
                msg<< "\nFT_ListDevices failed to get serial number!";
            }
        }
    }
    ui->plainTextEdit->appendPlainText(message);


}

SerialDiscoveryDialog::~SerialDiscoveryDialog()
{
    delete ui;
}
