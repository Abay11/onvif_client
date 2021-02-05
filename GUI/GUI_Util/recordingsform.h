#ifndef RECORDINGSFORM_H
#define RECORDINGSFORM_H

#include <QWidget>

namespace Ui
{
class RecordingsForm;
}

class Timeline;

class RecordingsForm : public QWidget
{
		Q_OBJECT

public:
		explicit RecordingsForm(QWidget *parent = nullptr);
		~RecordingsForm();

private:
		Ui::RecordingsForm *ui;

		Timeline* timeline_ = nullptr;
};

#endif // RECORDINGSFORM_H
