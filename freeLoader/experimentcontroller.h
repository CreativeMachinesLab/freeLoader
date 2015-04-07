#ifndef EXPERIMENTCONTROLLER_H
#define EXPERIMENTCONTROLLER_H

#include "jogcontroller.h"
#include "testdata.h"

enum TestType{
    kTensile,
    kCompression,
    kReading,
    kCycle
};
enum EndCondition{
  kTime,
  kDistance
};

class ExperimentController : public JogController
{
    Q_OBJECT
public:
    explicit ExperimentController(Gantry* gant_, QString filename, QObject *parent = 0);

    QString getFileName();

signals:

    void percentComplete(int);
    void experimentCompleted();
    void experimentStarted();
    void unableToStart();
    
public slots:
    void setFileName(QString filename);
    void startExperiment();
    void stopExperiment();
    void setTestParameters(TestType type, float speed, EndCondition cond, float interval, int times=1);
    //                      tensile or compresison 0-30,  time or distance,    min or mm
    void tic();

private slots:
    void cycle();

private:
    TestData* data_;
    TestType type_;
    EndCondition condi_;
    float interval_;
    float startposition_;
    QTimer *percentTimer_;
    int percent_;
    int n_;
    float time;
    int cyc;
    int dir_;
    QTimer* cycletimer_;
};

/**
 * in the future If we need more types of experiments we should change the class in the following ways.
 * 1) each experiment would be a subclass of this function and we would have a nother class that would interface with the UI to select the experiment controller
 *    the enum would not be in this class and would link the subclasses to a list for selection.
 * 2) we would use a speed profile generator function that would be over written by each subclass that would be used in the updateState function.
 *    it would take in the current state and generate the next speed based on the current state (time, poisiton load, etc)
 * 3) We would use a vector of parameters for the test to make it more general
 * 4) trivial profiles would be written for tensile and compressive.
 * this is the correct way to do it, but We just dont have the time or pressing need\
 * - Jeff and Ethan
 *
 **/

#endif // EXPERIMENTCONTROLLER_H


