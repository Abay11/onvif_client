#include "formvideolive.h"
#include "ui_formvideolive.h"

#include <QThread>

#include <streamhandler.h>

FormVideoLive::FormVideoLive(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVideoLive)
{
    ui->setupUi(this);

		connect(ui->pushButton, &QPushButton::clicked,
						this, &FormVideoLive::slotStartLive);
}

FormVideoLive::~FormVideoLive()
{
	delete ui;
}

void FormVideoLive::slotStartLive()
{
	worker_thread_ = new QThread(this);

	stream_handler_ = new StreamHandler(this);

	stream_handler_->moveToThread(worker_thread_);

	stream_handler_->startStream("rtsp://192.168.43.196/test.mp4&t=unicast&p=rtsp&ve=H264&w=1280&h=720&ae=PCMU&sr=8000",
																	 winId());
}
