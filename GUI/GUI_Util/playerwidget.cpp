#include "playerwidget.h"
#include "ui_playerwidget.h"

#include <QDebug>
#include <QResizeEvent>
#include "formvideolive.h"

PlayerWidget::PlayerWidget(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::PlayerWidget)
{
		auto sp = sizePolicy();
		sp.setVerticalPolicy(QSizePolicy::Expanding);
		sp.setHorizontalPolicy(QSizePolicy::Expanding);

		sizeHint();

		setSizePolicy(sp);
		updateGeometry();

		ui->setupUi(this);
}

PlayerWidget::~PlayerWidget()
{
		delete ui;
}

void PlayerWidget::resizeEvent(QResizeEvent *event)
{
		auto* holder = qobject_cast<FormVideoLive*>(parent());
		if(!holder)
				{
						return;
				}

		auto region = holder->PlayerRegion();
		int parentw = region.width();
		int parenth = region.height();
		int neww = event->size().width();
		int newh = event->size().height();

		if(event->oldSize().width() != event->size().width()) // changed width
				{
						newh = neww * 3 / 4;
						if(newh>parenth) // we can't set a new height greater than the parent's height, so get max allowed width
								{
										newh = parenth;
										neww = newh * 4 / 3;
								}
				}
		else // changed height
				{
						neww = newh * 4 / 3;
						if(neww > parentw) // no, we can't set a width greater than the parent's width
								{
										neww = parentw;
										newh = neww * 3 / 4;
								}

				}

		resize(neww, newh);
		event->accept();
}
