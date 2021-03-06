#include "gantry.h"
#include <QDebug>

Gantry::Gantry(QDomNode configfile, QObject *parent) :
    QObject(parent),mmPerRev(1.0),position(0.0),updateIntervalInMS(100)
{
    ///PARSE CONFIG FILE,
    ///MAKE DYNA AND CELL
    ///Connect DYNA AND CELL
    ///Connect cell.maxforce to dyna.stop



    QDomNode dynaConfig;
    QDomNode cellConfig;


    for(QDomNode gantchild = configfile.firstChild(); !gantchild.isNull(); gantchild = gantchild.nextSibling())
    {

        if(gantchild.parentNode().nodeName()=="freeloader")
        {

            if (gantchild.isComment()) {continue;}

            if ("dynamixel" == gantchild.nodeName().toLower()) {
                    dynaConfig = gantchild;
            } else if("loadcell"==gantchild.nodeName().toLower()){
                    cellConfig = gantchild;

                 //we just stored the other elements, but we can populate the
                 //axis elements right away
            } else if("axis"==gantchild.nodeName().toLower()){
                QDomNodeList locChildren = gantchild.childNodes();
                for (unsigned int j = 0; j < locChildren.length(); j++) {

                    QDomNode lchild = locChildren.at(j);

                    if ("mmperrev"==lchild.nodeName().toLower()) {
                            mmPerRev = lchild.firstChild().nodeValue().toDouble();
                    }else if("updaterate"==lchild.nodeName().toLower()){
                            updateIntervalInMS = (int) (1000.0 / lchild.firstChild().nodeValue().toFloat());
                            qDebug()<<"hz: "<<lchild.firstChild().nodeValue().toFloat()<< "\tms:"<< updateIntervalInMS;
                    }
                }




            }

        }

    }

    //create the gantry elements with the information we just extracted
//    qDebug()<<"Dynaconfig: "<<dynaConfig.nodeName().toLower();
//    qDebug()<<"LoadCell config: "<<cellConfig.nodeName().toLower();

    dyna = new Dynamixel(dynaConfig, this);
    cell = new LoadCell(cellConfig,this);


    dyna->connect();
    cell->open();

     ///Connect cell.maxforce to dyna.stop
}
