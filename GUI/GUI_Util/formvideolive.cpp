#include "formvideolive.h"
#include "ui_formvideolive.h"

#include <QThread>
#include <QDebug>
#include <QResizeEvent>

#include <streamhandler.h>
#include <playerwidget.h>
#include "audioinputgrabber.h"


/*
 * Here `Client` is a program code uses this class.
 * And `User` is a real user.
 *
 * This class holds current stream uri as a member;
 * Client should set a stream URI before invoking the Start method;
 * When User select another profile, signal 'PROFILE SWITCHED' is emitted
 * and playing is stopped and return to ready state;
 * Processing 'PROFILE SWITCHED' signal, a client should update a stream URI;
 * User should start stream again manually(click Play button again).
*/

void handler(void* /*data*/, size_t /*size*/)
{

}

FormVideoLive::FormVideoLive(QWidget* parent) :
    QWidget(parent),
		ui(new Ui::FormVideoLive)
{

    ui->setupUi(this);

		connect(ui->btnPlay, &QPushButton::clicked,
						this, &FormVideoLive::slotStartLive);
		connect(ui->btnStop, &QPushButton::clicked,
						this, &FormVideoLive::slotStopLive);

		connect(ui->cmbTokens, QOverload<const QString&>::of(&QComboBox::activated),
						this, &FormVideoLive::slotProfileSwitched);

		audio_grabber_ = new AudioInputGrabber(&handler);
		connect(ui->btnAdStart, &QPushButton::clicked, audio_grabber_, &AudioInputGrabber::Start);
		connect(ui->btnAdStop, &QPushButton::clicked, audio_grabber_, &AudioInputGrabber::Stop);
}

FormVideoLive::~FormVideoLive()
{
		delete ui;

		delete stream_handler_;
}

void FormVideoLive::SetProfileTokens(const QStringList &profiles)
{
		if(profiles.empty())
				qWarning() << ("Media profiles couldn't be empty!");

		//profiles_ = profiles_;

		ui->cmbTokens->addItems(profiles);
}

void FormVideoLive::SetStreamUri(const QString &uri)
{
		streamUri_ = uri;

		ui->leLiveStreamUri->setText(uri);
}

QRect FormVideoLive::PlayerRegion()
{
		return ui->playerLayout->geometry();
}

void FormVideoLive::slotStartLive()
{
		if(isStarted_)
				{
						qWarning() << "Stream is already playing";
						return;
				}

		if(streamUri_.isEmpty())
				{
						qWarning() << "Stream Uri is empty";
						return;
				}

		if(stream_handler_ == nullptr)
				{
						stream_handler_ = new StreamHandler(this);
				}

		stream_handler_->startStream(streamUri_, ui->playerWidget);

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
		ui->playerWidget->update();
}

void FormVideoLive::slotProfileSwitched(const QString &profile)
{
		if(isStarted_) slotStopLive();

		//it is expected that Client should update stream URI
		SetStreamUri("");

		emit sigProfileSwitched(profile);
}
