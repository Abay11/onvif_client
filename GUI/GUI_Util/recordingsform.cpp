#include "recordingsform.h"
#include "ui_recordingsform.h"

#include "logging.h"

#include "IDevice.h"
#include "IReplaySearch.h"

#include <QSlider>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>

class Timeline : public QObject
{
		Q_OBJECT

private:
		const static uint SECONDS_IN_DAY = 86399u;

public:
		Timeline(QSlider* s, QTimeEdit* te)
				:slider_(s)
				,time_edit_(te)
		{
				if(!s || !te)
						throw std::runtime_error("Pointer could not be null!");

				slider_->setMaximum(SECONDS_IN_DAY);
				//slider_->setSingleStep(1);

				connect(slider_, &QSlider::valueChanged, this, &Timeline::cursorMoved);
		}

private:
		void cursorMoved(int value);

private:
		QSlider* const slider_;
		QTimeEdit* const time_edit_;
};

RecordingsForm::RecordingsForm(_onvif::IDevice* device, QWidget *parent) :
		QWidget(parent)
		, ui(new Ui::RecordingsForm)
		, device_(device)
{
		ui->setupUi(this);

		timeline_ = new Timeline(ui->horizontalSlider, ui->timeEdit);


		replaySearch_ = device_->ReplaySearch();


		future_ = QtConcurrent::run([this]()
		{
				fillRecordingSummary();
		});
}

RecordingsForm::~RecordingsForm()
{
		delete ui;
		delete timeline_;
}

void RecordingsForm::fillRecordingSummary()
{
		for (int i=0; i<5000; ++i)
				{
						LOG_DEBUG("Filling recording summary");
						auto res = replaySearch_->RecordingSummary();
				}
}

void Timeline::cursorMoved(int value)
{
		time_edit_->setTime(QTime::fromMSecsSinceStartOfDay(value * 1000));
}

#include "recordingsform.moc"
