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
    void state(QVector<float>);

public slots:
    void setSpeed(float speed);
    void setHome();
    void setHome(float distinmm);
    QVector<float> getState();


private:
    Gantry* gant;
    QTimer* timer;
    QVector<float> lastState_;
};

#endif // JOGCONTROLLER_H
