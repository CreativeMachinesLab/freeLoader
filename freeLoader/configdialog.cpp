#include "configdialog.h"
#include "ui_configdialog.h"

ConfigDialog::ConfigDialog(QStringList configs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);
    ui->comboBox->addItems(configs);
}

ConfigDialog::~ConfigDialog()
{
    delete ui;
}


void ConfigDialog::selectionMade(){
    emit selected(ui->comboBox->currentText());
}
