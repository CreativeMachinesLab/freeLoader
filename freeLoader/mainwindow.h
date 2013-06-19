#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:

    void disableJog(bool disabled);
    void disableTestSettings(bool disabled);
    void disableAll(bool disabled);
    void disableTesting(bool disabled);

    //set at dyna connect
    void setTestSpeedMin(float min);
    void setTestSpeedMax(float max);

    void testStarted();
    void testEnded();



    // Internal because we didnt do this right
    void jogSpinUpdated();
    void jogSliderUpdated();
    void testSpinUpdated();
    void testSpinUpdated();

private:
    Ui::MainWindow *ui;
    float minSpeed_;
    float maxSpeed_;
};

#endif // MAINWINDOW_H
