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

		void SetProfileTokens(const QStringList& profiles);
		void SetStreamUri(const QString& uri);

signals:
		//internal signals
		void sigStartLiveStream(const QString& url);

		//external signals
		void sigProfileSwitched(const QString& profile);

private slots:
		void slotStartLive();
		void slotStopLive();

		void slotProfileSwitched(const QString& profile);

private:
    Ui::FormVideoLive *ui;

		StreamHandler* stream_handler_ = nullptr;

		QString streamUri_;

		bool isStarted_ = false;

		QStringList profiles_;
};

#endif // FORMVIDEOLIVE_H
