#ifndef JOGCONTROLLER_H
#define JOGCONTROLLER_H

#include <QObject>
#include <gantry.h>
#include <QTimer>

class JogController : public QObject
{
    Q_OBJECT
public:
    explicit JogController(Gantry *gant, QObject *parent = 0);
    /// NOTE, We need a method for protecting the gant from being used by mutliple controllers at the same time.
    /// In theory you could move the gantry using a jog at the same time an experiment is running.
    /// We would need to put a flag in the gant object to signify ownership of the commands by a particular object

    float calculateCurrentPosition(QVector<float> state);


public slots:
    // Used for all versions, very general
    void setSpeed(float speed);
    void move(float speed, float timeInMin, int direction); // speed = abs mm/min, time = time in min,  bool +1 = CW = up , -1 = CCW = down.... maybe
    void startMove();
    void stopMove();
    void updateState();

    //JOG ONLY, WOuld be seperated from class if we made this into a base class for Jog and Experiment
    void setHome();
    void setHome(float distinmm);
    void goHome();
    void jogUp();
    void jogDown();

signals:
    void dataPoint(QVector<float>);

public:
    QTimer *updatetimer_; //  only on when moving !=0
    QTimer *stoptimer_;
    Gantry *gant;
    float speed_;

private:
    int currDirection_; //added by Ethan
    QVector<float> lastState_;
};

#endif // JOGCONTROLLER_H
