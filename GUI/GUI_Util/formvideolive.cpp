#include "formvideolive.h"
#include "ui_formvideolive.h"

#include <QThread>
#include <QDebug>

#include <streamhandler.h>


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

FormVideoLive::FormVideoLive(QStringList profiles, QWidget *parent) :
    QWidget(parent),
		ui(new Ui::FormVideoLive),
		profiles_(profiles)
{
		if(profiles.empty())
				throw std::exception("Media profiles couldn't be empty!");

    ui->setupUi(this);

		ui->cmbTokens->addItems(profiles);

		connect(ui->btnPlay, &QPushButton::clicked,
						this, &FormVideoLive::slotStartLive);
		connect(ui->btnStop, &QPushButton::clicked,
						this, &FormVideoLive::slotStopLive);

		connect(ui->cmbTokens, QOverload<const QString&>::of(&QComboBox::activated),
						this, &FormVideoLive::slotProfileSwitched);
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

		if(streamUri_.isEmpty())
				{
						qWarning() << "Stream Uri is empty";
						return;
				}

		if(stream_handler_ == nullptr)
				{
						stream_handler_ = new StreamHandler(this);
				}

		stream_handler_->startStream(streamUri_, ui->widget);

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

void FormVideoLive::slotProfileSwitched(const QString &profile)
{
		if(isStarted_) slotStopLive();

		//it is expected that Client should update stream URI
		streamUri_.clear();

		emit sigProfileSwitched(profile);
}
