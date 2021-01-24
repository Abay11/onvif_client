#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>

namespace Ui
{
class PlayerWidget;
}

class PlayerWidget : public QWidget
{
		Q_OBJECT

public:
		explicit PlayerWidget(QWidget *parent = nullptr);
		~PlayerWidget();

private:
		Ui::PlayerWidget *ui;

		// QWidget interface
protected:
		void resizeEvent(QResizeEvent *event) override;
};

#endif // PLAYERWIDGET_H
