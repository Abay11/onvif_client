#include "formdevicemaintenance.h"
#include "ui_formdevicemaintenance.h"

FormDeviceMaintenance::FormDeviceMaintenance(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDeviceMaintenance)
{
    ui->setupUi(this);
}

FormDeviceMaintenance::~FormDeviceMaintenance()
{
    delete ui;
}
