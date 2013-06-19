#ifndef JOGCONTROLLER_H
#define JOGCONTROLLER_H

#include <QObject>
#include <gantry.h>
#include <QTimer>

class JogController : public QObject
{
    Q_OBJECT
public:
    explicit JogController(Gantry* gant, QObject *parent = 0);

signals:
    void position(float);

public slots:
    void setSpeed(float speed);
    void setHome();
    void setHome(float distinmm);
    float getPostion();
    void start();
    void stop();


private:
    Gantry* gant;
    QTimer* timer;
    QVector<float> lastState_;
};

#endif // JOGCONTROLLER_H
