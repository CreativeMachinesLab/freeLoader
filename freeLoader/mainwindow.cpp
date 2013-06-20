#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),minSpeed_(0.1),maxSpeed_(30.0),ticksPerMMPerMin_(1000),jogToggle_(true),testToggle_(true)
{
    ui->setupUi(this);
    connect(ui->jogSpeedSlider,SIGNAL(sliderMoved(int)),this,SLOT(jogSliderUpdated()));
    connect(ui->jogSpeedDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(jogSpinUpdated()));
    connect(ui->testSpeedHSlider,SIGNAL(sliderMoved(int)),this,SLOT(testSliderUpdated()));
    connect(ui->testSpeedDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(testSpinUpdated()));
    connect(ui->fileNameLineEdit,SIGNAL(editingFinished()),this,SLOT(filenameChanged()));

    //FOR TESTING
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(testStarted()));
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

void MainWindow::disableMovementTest(bool disabled){
    ui->tensileRadioButton->setDisabled(disabled);
    ui->compressionRadioButton->setDisabled(disabled);
    ui->distanceRadioButton->setDisabled(disabled);
}

//set at dyna connect
void MainWindow::setSpeedMin(float min){
    ui->testSpeedDoubleSpinBox->setMinimum(min);
    ui->testSpeedHSlider->setMinimum(min*ticksPerMMPerMin_);
    ui->jogSpeedDoubleSpinBox->setMinimum(min);
    ui->jogSpeedSlider->setMinimum(min*ticksPerMMPerMin_);
    ui->minJogSpeedLabel->setText(QString::number(min));
    ui->minTestSpeedLabel->setText(QString::number(min));
}
void MainWindow::setSpeedMax(float max){
    ui->testSpeedDoubleSpinBox->setMaximum(max);
    ui->testSpeedHSlider->setMaximum(max*ticksPerMMPerMin_);
    ui->jogSpeedDoubleSpinBox->setMaximum(max);
    ui->jogSpeedSlider->setMaximum(max*ticksPerMMPerMin_);
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
void MainWindow::filenameChanged(){
    ui->fileNameLineEditDisplay->setText(ui->fileNameLineEdit->text());
}

void MainWindow::jogSpinUpdated(){
    if(jogToggle_){
        ui->jogSpeedSlider->setValue((int)ticksPerMMPerMin_*ui->jogSpeedDoubleSpinBox->value());
    }
    jogToggle_=!jogToggle_;
}
void MainWindow::jogSliderUpdated(){
    if(jogToggle_){
        ui->jogSpeedDoubleSpinBox->setValue(ui->jogSpeedSlider->value()/ticksPerMMPerMin_);
    }
    jogToggle_=!jogToggle_;
}
void MainWindow::testSpinUpdated(){
    if(testToggle_){
        ui->testSpeedHSlider->setValue((int)ticksPerMMPerMin_*ui->testSpeedDoubleSpinBox->value());
    }
    testToggle_=!testToggle_;
}
void MainWindow::testSliderUpdated(){
    if(testToggle_){
        ui->testSpeedDoubleSpinBox->setValue(ui->testSpeedHSlider->value()/ticksPerMMPerMin_);
    }
    testToggle_=!testToggle_;
}
