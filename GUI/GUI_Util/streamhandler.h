#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <QObject>

#include <gst/gst.h>

class StreamHandler : public QObject
{
		Q_OBJECT
public:
		explicit StreamHandler(QObject *parent = nullptr);
		~StreamHandler();

		/*async methods*/
		void startStream(const QString& uri, QWidget* player);
		void stopStream();

signals:
		//signals for internal needs
		void sigStartStream();
		void sigStopStream();

public slots:
		void slotStartStream();
		void slotStopStream();

private:
		QString streamURL_;
		QWidget* playerWidget_ = nullptr;

		GstElement* playbin_ = nullptr;
		GstElement* videosink_ = nullptr;
		GMainLoop* main_loop = nullptr;
		QThread* gstream_context_thread = nullptr;
};

#endif // STREAMHANDLER_H
