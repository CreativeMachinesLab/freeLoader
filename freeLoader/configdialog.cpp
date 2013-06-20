#include "configdialog.h"
#include "ui_configdialog.h"
#include <QDebug>

ConfigDialog::ConfigDialog(QStringList configs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->comboBox->addItems(configs);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(selectionMade()));
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}


void ConfigDialog::selectionMade(){
    emit selected(ui->comboBox->currentText());
}
