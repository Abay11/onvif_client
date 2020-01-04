#include "formvideoconfiguration.h"
#include "ui_formvideoconfiguration.h"

#include <QDebug>

FormVideoConfiguration::FormVideoConfiguration(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoConfiguration)
{
    ui->setupUi(this);
}

FormVideoConfiguration::~FormVideoConfiguration()
{
    delete ui;
	qDebug() << "FormVideoConfiguration deleted";
}
