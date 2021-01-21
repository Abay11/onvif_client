#pragma once

#include "QObject"

#include <QList>

namespace _onvif
{
class IDevice;
struct Profile;
using ProfileSP = std::shared_ptr<Profile>;
class VideoEncoderConfiguration;
}


struct DeviceCredentials
{
		bool isOnline;
		QString name;
		QString ip;
		QString port;
		QString uri;
		QString user;
		QString password;

		DeviceCredentials(const QString& ip,
											const QString& port,
											const QString& uri,
											const QString& user,
											const QString& pass)
				: isOnline(false)
				,ip(ip)
				,port(port)
				,uri(uri)
				,user(user)
				,password(pass)
		{
		}

		QString id()  const
		{
				if(ip.isEmpty())
						return {};

				return ip + (port.isEmpty() ? "" : ":" + port);
		}
};
class IDeviceManager
{
public:
		virtual ~IDeviceManager() {}
		virtual void Add(const DeviceCredentials&) = 0;
		// Start to connect asynch. Handler should be called
		virtual void Connect(const QString& id, std::function<void()> handler) = 0;
		virtual void Disconnect(const QString& id) = 0;
		virtual void Remove(const QString& id) = 0;
};


class DevicesManager : public QObject, public IDeviceManager
{
		Q_OBJECT
public:
		DevicesManager(QObject* parent = nullptr);

		// IDeviceManager interface
		void Add(const DeviceCredentials&) override;
		void Connect(const QString &id, std::function<void()> handler) override;
		void Disconnect(const QString &id) override;
		void Remove(const QString &id) override;

signals:

		//SIGNALS emited to run async requests
		void sigAsyncGetProfile(const QString& /*deviceID*/,
														const QString& /*profileToken*/);
		void sigAsyncGetVideoSettings(const QString& /*deviceID*/);
		void sigAsyncGetLiveInfo(const QString& /*deviceID*/);
		void sigAsyncGetLiveUri(const QString& /*deviceID*/,
														const QString& /*profile*/);

		//SIGNALS emited when an async request getting ready
		void sigAsyncGetProfileReady();
		void sigAsyncGetVideoSettingsReady();
		void sigLiveInfoReady();
		void sigLiveUriReady();

		//SIGNALS emited when to set settings
		void sigSetVideoEncoderSettings(const QString& /*deviceID*/);
		void sigAddVideoEncoderConfig(const QString& /*deviceID*/,
																	const QString& /*profile*/,
																	const QString& /*config*/);

		//SIGNALS emited when applying settings is finished
		void sigVideoEncoderConfigAdded();

public slots:


public:
		_onvif::IDevice* getDevice(const QString& addressInfo);
		_onvif::IDevice* getDevice(int index);

		bool connected(const QString& id);

		//ASYNC REQUEST RUNNERS
		//NOTE: if it is possible, this methods should check
		//some conditionals and return true/false if they are correct

		//these methods return true if a device is found by deviceID
		bool asyncGetProfile(const QString& /*deviceID*/, const QString& profileToken);
		//this method is used to invoke two requests
		bool asyncGetVideoSettings(const QString& /*deviceID*/);
		/*Live stream slots*/
		bool asyncGetLiveInfo(const QString& deviceID);
		bool asyncGetLiveUri(const QString& deviceID, const QString& profile);

		//ASYNC REQUEST RESULT GETTERS
		_onvif::ProfileSP getAsyncGetProfileResult();
		void getAsyncGetVideoSettingsResult(QStringList& /*tokens*/,
																				_onvif::ProfileSP& /*profile*/);
		void getLiveInfoResults(QStringList&, QString& uri);
		void getLiveUriResult(QString& uri);

		//set settings request assumed to be async
		void setVideoEncoderSettings(const QString& /*deviceID*/, const _onvif::VideoEncoderConfiguration& /*ve*/);
		bool addVideoEncoderToProfile(const QString& /*deviceID*/,
																	const QString& /*profile*/,
																	const QString& /*veToken*/);

private slots:
		void slotAsyncGetProfile(const QString& /*deviceID*/,
														 const QString& /*profileToken*/);
		void slotAsyncGetVideoSettings(const QString& /*deviceID*/);
		void slotAsyncGetLiveInfo(const QString& /*deviceID*/);
		void slotAsyncGetLiveUri(const QString& /*deviceID*/, const QString& /*profile*/);
		void slotSetVideoEncoderSetting(const QString& /*deviceID*/ );
		void slotAddVideoEncoderConfig(const QString& /*deviceID*/,
																	 const QString& /*profile*/,
																	 const QString& /*config*/);

private:
		//ASYNC RESULT HOLDERS
		//this holders SHOULD BE reseted after async result
		//getters calling
		_onvif::ProfileSP asyncGetProfileResultHolder_;
		QStringList asyncGetProfileTokensResultHolder_;
		QString liveUrlHolder_;

		//holders to save settings should be send to a device
		_onvif::VideoEncoderConfiguration* videoEncoderConfigHolder_ = nullptr;

		QMap<QString, std::pair<DeviceCredentials, _onvif::IDevice*>> devices_;
		QVector<_onvif::IDevice*> devices;

};
