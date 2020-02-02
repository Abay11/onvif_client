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
	~DialogWaiting();

	void setDialogText(const QString& text);

private:
	Ui::DialogWaiting *ui;
	QMovie *movie;

	// QDialog interface
public slots:
	void open();

	// QWidget interface
protected:
	void closeEvent(QCloseEvent *event);
};

#endif // DIALOGWAITING_H
