#ifndef DIALOGWAITING_H
#define DIALOGWAITING_H

#include <QDialog>

namespace Ui {
	class DialogWaiting;
}

class DialogWaiting : public QDialog
{
	Q_OBJECT

public:
	explicit DialogWaiting(QWidget *parent = nullptr);
	~DialogWaiting() override;

	void setDialogText(const QString& text);

public slots:
	// QDialog interface
	void open() override;

	void close(); /*override QDialog???? */

private:
	Ui::DialogWaiting *ui;
	QMovie *movie_;
};

#endif // DIALOGWAITING_H
