#include "recordingsform.h"
#include "ui_recordingsform.h"

#include "logging.h"

#include <QSlider>
#include <QTimer>

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



RecordingsForm::RecordingsForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::RecordingsForm)
{
		ui->setupUi(this);

		timeline_ = new Timeline(ui->horizontalSlider, ui->timeEdit);
}

RecordingsForm::~RecordingsForm()
{
		delete ui;
		delete timeline_;
}

void Timeline::cursorMoved(int value)
{
		time_edit_->setTime(QTime::fromMSecsSinceStartOfDay(value * 1000));
}

#include "recordingsform.moc"
