#ifndef FORMVIDEOLIVE_H
#define FORMVIDEOLIVE_H

#include <QWidget>

class StreamHandler;

namespace Ui
{
class FormVideoLive;
}

class FormVideoLive : public QWidget
{
    Q_OBJECT

public:
    explicit FormVideoLive(QWidget *parent = nullptr);
    ~FormVideoLive();

signals:
		void sigStartLiveStream(const QString& url);

private slots:
		void slotStartLive();
		void slotStopLive();

private:
    Ui::FormVideoLive *ui;

		StreamHandler* stream_handler_ = nullptr;

		bool isStarted_ = false;
};

#endif // FORMVIDEOLIVE_H
