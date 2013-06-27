#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <qmath.h>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),minSpeed_(0.1),maxSpeed_(30.0),ticksPerMMPerMin_(1000),jogToggle_(true),testToggle_(true)
{

    ui->setupUi(this);


    QMenu *menuFile = new QMenu("File");
    QAction *actionSerial;
    actionSerial = new QAction("&Discover Serial Numbers",this);
    actionSerial->setShortcut(QKeySequence("d"));
    menuFile->addAction(actionSerial);
    menuBar()->addMenu(menuFile);
    connect(actionSerial,SIGNAL(triggered()),this,SLOT(serialDiscovery()));



    connect(ui->jogSpeedSlider,SIGNAL(sliderMoved(int)),this,SLOT(jogSliderUpdated()));
    connect(ui->jogSpeedDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(jogSpinUpdated()));
    connect(ui->testSpeedHSlider,SIGNAL(sliderMoved(int)),this,SLOT(testSliderUpdated()));
    connect(ui->testSpeedDoubleSpinBox,SIGNAL(valueChanged(double)),this,SLOT(testSpinUpdated()));
    connect(ui->fileNameLineEdit,SIGNAL(editingFinished()),this,SLOT(filenameChanged()));

    connect(ui->jogUpButton,SIGNAL(clicked()),this,SLOT(jogUpClicked()));
    connect(ui->jogDownButton,SIGNAL(clicked()),this,SLOT(jogDownClicked()));
    connect(ui->setHomeButton,SIGNAL(clicked()),this,SLOT(setHomeClicked()));
    connect(ui->goHomeButton,SIGNAL(clicked()),this,SLOT(goHomeClicked()));
    connect(ui->startButton,SIGNAL(clicked()),this,SLOT(startClicked()));
    connect(ui->endButton,SIGNAL(clicked()),this,SLOT(endClicked()));


    //FOR TESTING
    //connect(ui->startButton,SIGNAL(clicked()),this,SLOT(testStarted()));
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
    if(min<=0){
        min=0.1;
    }

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
    ui->dataPlainTextEdit->clear();
    ui->scatterPlot->removeWidget(d_plot);
    _samples.clear();
    d_plot = new Plot( this );
    d_plot->setTitle( "Force (N) versus Displacement (mm)" );

    ui->scatterPlot->addWidget( d_plot);
;


    disableJog(true);
    disableTestSettings(true);
    disableTesting(false);
}
void MainWindow::testEnded(){
    ui->stackedWidget->setCurrentIndex(0);
    delete d_plot;
    _samples.clear();
    disableJog(false);
    disableTestSettings(false);
    disableTesting(true);
}

void MainWindow::setPercent(int per){
    ui->progressBar->setValue(per);
}

void MainWindow::serialDiscovery(){
    SerialDiscoveryDialog d;
    d.exec();
}

void MainWindow::addPoint(QVector<float> point){
    if(point.size()<3){return;}

    QString toadd="\n";
    QTextStream ss(&toadd,QIODevice::WriteOnly);
    ss<<QString::number(point[0])<<",\t"
      <<QString::number(point[1])<<",\t"
      <<QString::number(point[2]);
    ui->dataPlainTextEdit->appendPlainText(toadd);

    const double pos=point[1];
    const double force=point[2];

    _samples += QPointF( pos, force );

    if(_samples.size()%10==0){ //refresh plot after ten points
    d_plot->setAttribute( _samples);

    d_plot->setSamples( _samples );


    d_plot->replot();

    }

}


TestType MainWindow::getType(){
    if (ui->tensileRadioButton->isChecked()){
        return kTensile;
    }else if(ui->compressionRadioButton->isChecked()){
        return kCompression;
    }
    return kReading;
}

float MainWindow::getSpeed(){
    return ui->testSpeedDoubleSpinBox->value();
}

EndCondition MainWindow::getEndCond(){
    if(ui->distanceRadioButton->isChecked()){
        return kDistance;
    }
    return kTime;

}
float MainWindow::getInterval(){
    if (ui->distanceRadioButton->isChecked()){
        return ui->distanceDoubleSpinBox->value();
    }

    return ui->timeDoubleSpinBox->value();
}


QString MainWindow::getFileName(){
    return ui->fileNameLineEdit->text();
}

void MainWindow::setFileName(QString s){
     ui->fileNameLineEdit->setText(s);
     filenameChanged();
}


// Internal because we didnt do this right
void MainWindow::filenameChanged(){
    ui->fileNameLineEditDisplay->setText(ui->fileNameLineEdit->text());
}

void MainWindow::jogSpinUpdated(){
    if(jogToggle_){
        ui->jogSpeedSlider->setValue((int)ticksPerMMPerMin_*ui->jogSpeedDoubleSpinBox->value());
        emit jogSpeedChanged(ui->jogSpeedDoubleSpinBox->value());
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
        emit jogSpeedChanged(ui->jogSpeedDoubleSpinBox->value());
    }
    testToggle_=!testToggle_;
}
void MainWindow::testSliderUpdated(){
    if(testToggle_){
        ui->testSpeedDoubleSpinBox->setValue(ui->testSpeedHSlider->value()/ticksPerMMPerMin_);
    }
    testToggle_=!testToggle_;
}




void MainWindow::jogUpClicked(){
    emit jogUp();
}
void MainWindow::jogDownClicked(){
    emit jogDown();
}
void MainWindow::goHomeClicked(){
    emit goHome();
}
void MainWindow::setHomeClicked(){
    emit setHome();
}
void MainWindow::startClicked(){
    emit startXp();
}
void MainWindow::endClicked(){
    emit endXp();
}

static double randomValue()
{
    // a number between [ 0.0, 1.0 ]
    return ( qrand() % 100000 ) / 100000.0;
}


void MainWindow::setSamples()
{


//    for ( int i = 0; i < 1000; i++ )
//    {
//         pos = randomValue() * 24.0 + 1.0;
//         force = ::log( 10.0 * ( pos - 1.0 ) + 1.0 )
//            * ( randomValue() * 0.5 + 0.9 );

//        samples += QPointF( pos, force );
//    }
    //_samples += QPointF( pos, force );
    qDebug()<<"Made it inside setSamples";
    d_plot->setSamples( _samples );
}



