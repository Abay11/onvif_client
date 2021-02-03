#include "recordingsform.h"
#include "ui_recordingsform.h"

RecordingsForm::RecordingsForm(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::RecordingsForm)
{
	ui->setupUi(this);
}

RecordingsForm::~RecordingsForm()
{
	delete ui;
}
