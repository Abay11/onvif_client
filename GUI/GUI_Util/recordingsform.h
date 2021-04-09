#ifndef RECORDINGSFORM_H
#define RECORDINGSFORM_H

#include <QWidget>
#include <QFuture>

namespace Ui
{
class RecordingsForm;
}

class Timeline;

namespace _onvif
{
class IDevice;
class IReplaySearch;
}

class RecordingsForm : public QWidget
{
		Q_OBJECT

public:
		explicit RecordingsForm(_onvif::IDevice* device, QWidget *parent = nullptr);
		~RecordingsForm();

private:

		void fillRecordingSummary();

private:
		Ui::RecordingsForm *ui;

		_onvif::IDevice* device_;
		std::shared_ptr<_onvif::IReplaySearch> replaySearch_;
		QFuture<void> future_;

		Timeline* timeline_ = nullptr;
};

#endif // RECORDINGSFORM_H
