#ifndef FORMVIDEOLIVE_H
#define FORMVIDEOLIVE_H

#include <QWidget>

class StreamHandler;
			//StreamHandler

namespace Ui {
    class FormVideoLive;
}

class FormVideoLive : public QWidget
{
    Q_OBJECT

public:
    explicit FormVideoLive(QWidget *parent = nullptr);
    ~FormVideoLive();

private slots:
		void slotStartLive();

private:
    Ui::FormVideoLive *ui;
		QThread* worker_thread_;
		StreamHandler* stream_handler_;
};

#endif // FORMVIDEOLIVE_H
