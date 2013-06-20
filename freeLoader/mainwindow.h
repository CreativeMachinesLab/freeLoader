#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "experimentcontroller.h"
#include "serialdiscoverydialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    ///getTestSettings(); //???????

public slots:

    void disableJog(bool disabled);
    void disableTestSettings(bool disabled);
    void disableTesting(bool disabled);
    void disableMovementTest(bool disabled);

    //set at dyna connect
    void setSpeedMin(float min);
    void setSpeedMax(float max);

    void testStarted();
    void testEnded();

    void setFileName(QString s);

    void addPoint(QVector<float> point);
    void setPercent(int per);


    void serialDiscovery();

    // Internal because we didnt do this right
    void jogSpinUpdated();
    void jogSliderUpdated();
    void testSpinUpdated();
    void testSliderUpdated();
    void filenameChanged();

    // get test info
    TestType getType();
    float getSpeed();
    EndCondition getEndCond();
    float getInterval();
    QString getFileName();


    // Re-emmiter
    void jogUpClicked();
    void jogDownClicked();
    void goHomeClicked();
    void setHomeClicked();
    void startClicked();
    void endClicked();

signals:
    void jogUp();
    void jogDown();
    void goHome();
    void setHome();
    void startXp();
    void endXp();
    void jogSpeedChanged(float f);

private:
    Ui::MainWindow *ui;
    float minSpeed_;
    float maxSpeed_;
    float ticksPerMMPerMin_;
    bool jogToggle_;
    bool testToggle_;
};

#endif // MAINWINDOW_H
