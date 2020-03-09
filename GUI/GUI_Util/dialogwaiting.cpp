#include "dialogwaiting.h"
#include "ui_dialogwaiting.h"

#include <QMovie>
#include <QCommonStyle>
#include <QDebug>

const static int HEIGHT = 50;
const static int WIDTH = 50;

const static char* DEFAULT_TEXT = "Please wait...";

DialogWaiting::DialogWaiting(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogWaiting)
{
	ui->setupUi(this);

	movie_ = new QMovie(this);
	movie_->setFileName("://resources/loader.gif");
	movie_->setScaledSize(QSize(WIDTH, HEIGHT));

	ui->lblLoading->setMovie(movie_);
	// remove a shape which was set for convinience on the UI editor
	ui->lblLoading->setFrameShape(QFrame::Shape::NoFrame);
	setWindowFlag(Qt::WindowType::FramelessWindowHint);

	setStyleSheet("QWidget#DialogWaiting {border: 1px solid gray}");
}

DialogWaiting::~DialogWaiting()
{
	delete ui;
}

void DialogWaiting::setDialogText(const QString &text)
{
	ui->lblText->setText(text);
}

void DialogWaiting::open()
{
	//need to call each time
	//because may be passed
	//text with diff length
	adjustSize();

	movie_->start();

	QDialog::open();
}

void DialogWaiting::close()
{
	movie_->stop();

	//if default text were changed restore it
	ui->lblText->setText(DEFAULT_TEXT);

	QWidget::close();
}
