#ifndef STREAMHANDLER_H
#define STREAMHANDLER_H

#include <QObject>

class StreamHandler : public QObject
{
	Q_OBJECT
public:
	explicit StreamHandler(QObject *parent = nullptr);

	void startStream(const QString& uri, quintptr wID);

signals:
	void sigStartResult(int res);

	//signals for internal needs
	void sigStartStream(const QString& uri, quintptr wID);

public slots:
	void slotStartStream(const QString& uri, quintptr windowID);
	void slotStopStream();

};

#endif // STREAMHANDLER_H
