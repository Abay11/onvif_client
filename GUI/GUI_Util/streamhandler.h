#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <QObject>

class StreamHandler : public QObject
{
		Q_OBJECT
public:
		explicit StreamHandler(QObject *parent = nullptr);

		/*async method*/
		void startStream(const QString& uri, uint64_t wID);

signals:
		//signals for internal needs
		void sigStartStream();

public slots:
		void slotStartStream();
		void slotStopStream();

private:
		QString streamURL_;
		uint64_t windowID_;
};

#endif // STREAMHANDLER_H
