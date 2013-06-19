#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),minSpeed_(0.1),maxSpeed_(30.0),ticksPerMMPerMin_(100)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::disableJog(bool disabled){
    ui->jogFrame->setDisabled(disabled);
}
void MainWindow::disableTestSettings(bool disabled){
    ui->testingFrame->setDisabled(disabled);
}

void MainWindow::disableTesting(bool disabled){
    ui->stackedWidget->widget(1)->setEnabled(!disabled);
}

//set at dyna connect
void MainWindow::setSpeedMin(float min){
    ui->testSpeedDoubleSpinBox->setMinimum(min);
    ui->testSpeedHSlider->setMinimum(min);
    ui->minJogSpeedLabel->setText(QString::number(min));
    ui->minTestSpeedLabel->setText(QString::number(min));
}
void MainWindow::setSpeedMax(float max){
    ui->testSpeedDoubleSpinBox->setMaximum(max);
    ui->testSpeedHSlider->setMaximum(max);
    ui->maxJogSpeedLabel->setText(QString::number(max));
    ui->maxTestSpeedLabel->setText(QString::number(max));
}

void MainWindow::testStarted(){
    ui->stackedWidget->setCurrentIndex(1);
    disableJog(true);
    disableTestSettings(true);
    disableTesting(false);
}
void MainWindow::testEnded(){
    ui->stackedWidget->setCurrentIndex(0);
    disableJog(false);
    disableTestSettings(false);
    disableTesting(true);
}



// Internal because we didnt do this right
void MainWindow::jogSpinUpdated(){}
void MainWindow::jogSliderUpdated(){}
void MainWindow::testSpinUpdated(){}
void MainWindow::testSliderUpdated(){}
