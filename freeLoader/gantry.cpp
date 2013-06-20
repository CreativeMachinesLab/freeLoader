#include "gantry.h"
#include <QDebug>

Gantry::Gantry(QDomElement configfile, QObject *parent) :
    QObject(parent),mmPerRev(1.0),position(0.0)
{
    ///PARSE CONFIG FILE,
    ///MAKE DYNA AND CELL
    ///Connect DYNA AND CELL
    ///Connect cell.maxforce to dyna.stop


    QDomNode dynaConfig;
    QDomNode cellConfig;


    for(QDomNode topnode = configfile.firstChild(); !topnode.isNull(); topnode = topnode.nextSibling())
    {
        if(topnode.nodeName()=="freeloader")
        {

            qDebug()<<topnode.nodeName();
            QDomNodeList gantChildren = topnode.childNodes();

            for (unsigned int i=0; i<gantChildren.length();i++) {
                QDomNode gantchild = gantChildren.at(i);
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
                            mmPerRev = lchild.nodeValue().toDouble();
                        }else if("updaterate"==lchild.nodeName().toLower()){
                            updateIntervalInMS = lchild.nodeValue().toInt();
                        }
                }

             }


            }
        }

    }

    //create the gantry elements with the information we just extracted
    dyna = new Dynamixel(dynaConfig, this);
    cell = new LoadCell(cellConfig,this);

    dyna->connect();
    cell->open();

     ///Connect cell.maxforce to dyna.stop
}
