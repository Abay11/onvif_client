#pragma once

#include "QObject"

#include <QList>

namespace _onvif
{
class IDevice;
struct Profile;
using ProfileSP = std::shared_ptr<Profile>;
}

class DevicesManager : public QObject
{
    Q_OBJECT
public:
	DevicesManager(QObject* parent = nullptr);

signals:
    void sigNewDeviceAdded(QString address);

		//SIGNALS emited to run async requests
		void sigAsyncGetProfile(const QString& /*deviceID*/,
														const QString& /*profileToken*/);

		//SIGNALS emited when an async request become ready
		void sigAsyncGetProfileReady();

public slots:
		void slotAddDevice(QString ip, short port, QString deviceServiceURI);

public:
		_onvif::IDevice* getDevice(const QString& addressInfo);
		_onvif::IDevice* getDevice(int index);

		//ASYNC REQUEST RUNNERS
		//NOTE: if it is possible, this methods should check
		//some conditionals and return true/false if they are correct

		//this method return true if is found by deviceID
		//otherwise false
		bool asyncGetProfile(const QString& deviceID, const QString& profileToken);

		//ASYNC REQUEST RESULT GETTERS
		_onvif::ProfileSP getAsyncGetProfileResult();

private slots:
		void slotAsyncGetProfile(const QString& /*deviceID*/,
														 const QString& /*profileToken*/);

private:
		//ASYNC RESULT HOLDERS
		//this holders SHOULD BE reseted after async result
		//getters calling
		_onvif::ProfileSP asyncGetProfileResultHolder_;

		QVector<_onvif::IDevice*> devices;
};
