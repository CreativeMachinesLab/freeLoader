#include "testdata.h"

TestData::TestData(QObject *parent) :
    QObject(parent),filename_("")
{
}

TestData::TestData(QString filename):filename_(filename)
{
}

QString TestData::getFileName(){
    return QString(filename_);
}

void TestData::setFileName(QString filename){
    filename_=filename;
}
void TestData::addData(QVector<float> datapoint){
    if(datapoint.size()==3){
        data_.append(data);
    }
}

void TestData::addData(int time, float mm, float newtons){
    QVector<float> point(3);
    point[0]=float(time);
    point[1]=mm;
    point[2]=newtons;
    addData(point);
}

bool TestData::writeToFile(){
    return true;
}

