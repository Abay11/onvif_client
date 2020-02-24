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
	void sigAsyncGetVideoSettings(const QString& /*deviceID*/);

	//SIGNALS emited when an async request become ready
	void sigAsyncGetProfileReady();
	void sigAsyncGetVideoSettingsReady();

	//SIGNALS emited when to set settings
	void sigAddVideoEncoderConfig(const QString& /*deviceID*/,
																				 const QString& /*profile*/,
																				 const QString& /*config*/);

	//SIGNALS emited when applying settings is finished
	void sigVideoEncoderConfigAdded();

public slots:
	void slotAddDevice(QString ip, short port, QString deviceServiceURI);

public:
	_onvif::IDevice* getDevice(const QString& addressInfo);
	_onvif::IDevice* getDevice(int index);

	//ASYNC REQUEST RUNNERS
	//NOTE: if it is possible, this methods should check
	//some conditionals and return true/false if they are correct

	//these methods return true if a device is found by deviceID
	bool asyncGetProfile(const QString& /*deviceID*/, const QString& profileToken);
	//this method is used to invoke two requests
	bool asyncGetVideoSettings(const QString& /*deviceID*/);

	//ASYNC REQUEST RESULT GETTERS
	_onvif::ProfileSP getAsyncGetProfileResult();
	void getAsyncGetVideoSettingsResult(QStringList& /*tokens*/,
																			_onvif::ProfileSP& /*profile*/);

	//set settings request assumed to be async
	bool addVideoEncoderToProfile(const QString& /*deviceID*/,
																const QString& /*profile*/,
																const QString& /*veToken*/);

private slots:
	void slotAsyncGetProfile(const QString& /*deviceID*/,
													 const QString& /*profileToken*/);
	void slotAsyncGetVideoSettings(const QString& /*deviceID*/);
	void slotAddVideoEncoderConfig(const QString& /*deviceID*/,
																		const QString& /*profile*/,
																		const QString& /*config*/);

private:
	//ASYNC RESULT HOLDERS
	//this holders SHOULD BE reseted after async result
	//getters calling
	_onvif::ProfileSP asyncGetProfileResultHolder_;
	QStringList asyncGetProfileTokensResultHolder_;

	QVector<_onvif::IDevice*> devices;
};
