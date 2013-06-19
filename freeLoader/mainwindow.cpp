#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::disableJog(bool disabled){}
void MainWindow::disableTestSettings(bool disabled){}
void MainWindow::disableAll(bool disabled){}
void MainWindow::disableTesting(bool disabled){}

//set at dyna connect
void MainWindow::setTestSpeedMin(float min){}
void MainWindow::setTestSpeedMax(float max){}

void MainWindow::testStarted(){}
void MainWindow::testEnded(){}



// Internal because we didnt do this right
void MainWindow::jogSpinUpdated(){}
void MainWindow::jogSliderUpdated(){}
void MainWindow::testSpinUpdated(){}
void MainWindow::testSliderUpdated(){}
