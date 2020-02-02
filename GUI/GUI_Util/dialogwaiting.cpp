#include "dialogwaiting.h"
#include "ui_dialogwaiting.h"

#include <QMovie>

const static int HEIGHT = 50;
const static int WIDTH = 50;

DialogWaiting::DialogWaiting(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogWaiting)
{
	ui->setupUi(this);

	movie = new QMovie("://resources/loader.gif");
	movie->setScaledSize(QSize(WIDTH, HEIGHT));
	ui->lblLoading->setMovie(movie);
	// remove a shape which was set for convinience on the UI editor
	ui->lblLoading->setFrameShape(QFrame::Shape::NoFrame);
	setWindowFlag(Qt::WindowType::FramelessWindowHint);
	adjustSize();
}

DialogWaiting::~DialogWaiting()
{
	delete ui;
}

void DialogWaiting::setDialogText(const QString &text)
{
	ui->label->setText(text);
}

void DialogWaiting::open()
{
	movie->start();

	QDialog::open();
}

void DialogWaiting::closeEvent(QCloseEvent *event)
{
	movie->stop();
	QDialog::closeEvent(event);
}
