#include "playerwidget.h"
#include "ui_playerwidget.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::PlayerWidget)
{
		ui->setupUi(this);
}

PlayerWidget::~PlayerWidget()
{
		delete ui;
}
