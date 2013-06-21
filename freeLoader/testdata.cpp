#include "testdata.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

TestData::TestData(QString filename, QObject *parent) :
    QObject(parent),filename_(filename)
{
}


QString TestData::getFileName(){
    return QString(filename_);
}

void TestData::setFileName(QString filename){
    filename_=filename;
}

void TestData::clearData(){
    data_.clear();
}

void TestData::addData(QVector<float> datapoint){
    if(datapoint.size()!=3){return;}
    data_.append(datapoint);
}

void TestData::addData(int time, float mm, float newtons){
    QVector<float> point(3);
    point[0]=float(time);
    point[1]=mm;
    point[2]=newtons;
    addData(point);
}

bool TestData::writeToFile(){

    QVector <float> thisDataPoint(3); //length 3 -> time, pos, load

    QFile fileToWrite(filename_);
    if(!(fileToWrite.open(QIODevice::WriteOnly | QIODevice::Text))){
        qDebug()<<"Failed to open writefile "<<filename_;
        return false;
    }else{
        QTextStream writeStream(&fileToWrite);
        writeStream<<"freeLoader data file: " <<filename_<<"\n";
        writeStream<<"Data is time (ms), position (mm), load (N) \n \n";

        for(unsigned int i=0; i<data_.size(); ++i)
        {
            thisDataPoint=data_[i];
            writeStream<<thisDataPoint[0]<<","<<thisDataPoint[1]<<","<<thisDataPoint[2]<<"\n";

        }
        //write successful
        fileToWrite.close();
        return true;
    }

}

