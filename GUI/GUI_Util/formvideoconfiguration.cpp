#include "formvideoconfiguration.h"
#include "ui_formvideoconfiguration.h"

FormVideoConfiguration::FormVideoConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoConfiguration)
{
    ui->setupUi(this);
}

FormVideoConfiguration::~FormVideoConfiguration()
{
    delete ui;
}
