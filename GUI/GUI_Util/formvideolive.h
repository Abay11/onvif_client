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

private:
    Ui::FormVideoLive *ui;
		QThread* worker_thread_;
		StreamHandler* stream_handler_;
};

#endif // FORMVIDEOLIVE_H
