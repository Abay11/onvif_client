#ifndef RECORDINGSFORM_H
#define RECORDINGSFORM_H

#include <QWidget>

namespace Ui {
	class RecordingsForm;
}

class RecordingsForm : public QWidget
{
	Q_OBJECT

public:
	explicit RecordingsForm(QWidget *parent = nullptr);
	~RecordingsForm();

private:
	Ui::RecordingsForm *ui;
};

#endif // RECORDINGSFORM_H
