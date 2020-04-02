#include "formvideolive.h"
#include "ui_formvideolive.h"

#include <QThread>
#include <QDebug>

#include <streamhandler.h>

FormVideoLive::FormVideoLive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoLive)
{
    ui->setupUi(this);

		connect(ui->btnPlay, &QPushButton::clicked,
						this, &FormVideoLive::slotStartLive);
		connect(ui->btnStop, &QPushButton::clicked,
						this, &FormVideoLive::slotStopLive);
}

FormVideoLive::~FormVideoLive()
{
		delete ui;

		delete stream_handler_;
}

void FormVideoLive::slotStartLive()
{
		if(isStarted_)
				{
						qWarning() << "Stream is already playing";
						return;
				}

		if(stream_handler_ == nullptr)
				{
						stream_handler_ = new StreamHandler(this);
				}

		QString url = "rtsp://192.168.43.196/test.mp4&t=unicast&p=rtsp&ve=H264&w=1280&h=720&ae=PCMU&sr=8000";
		stream_handler_->startStream(url, ui->widget);

		isStarted_ = true;
}

void FormVideoLive::slotStopLive()
{
		if(!isStarted_)
				{
						qWarning() << "Stream is not playing";
						return;
				}

		stream_handler_->stopStream();

		isStarted_ = false;

		//to remove the last video frame
		ui->widget->update();
}
