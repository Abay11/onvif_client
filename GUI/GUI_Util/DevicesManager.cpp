#include "DevicesManager.h"

#include <QDebug>
#include <QString>
#include <QVector>
#include <QTextStream>
#include <QtConcurrent/QtConcurrent>

#include "device.h"


DevicesManager::DevicesManager(QObject *parent)
		: QObject(parent)
{
		connect(this, &DevicesManager::sigAsyncGetProfile,
						this, &DevicesManager::slotAsyncGetProfile);
		connect(this, &DevicesManager::sigAsyncGetVideoSettings,
						this, &DevicesManager::slotAsyncGetVideoSettings);
		connect(this, &DevicesManager::sigAsyncGetLiveInfo,
						this, &DevicesManager::slotAsyncGetLiveInfo);
		connect(this, &DevicesManager::sigAsyncGetLiveUri,
						this, &DevicesManager::slotAsyncGetLiveUri);

		connect(this, &DevicesManager::sigSetVideoEncoderSettings,
						this, &DevicesManager::slotSetVideoEncoderSetting);
		connect(this, &DevicesManager::sigAddVideoEncoderConfig,
						this, &DevicesManager::slotAddVideoEncoderConfig);
}

void DevicesManager::Add(const DeviceCredentials& creds)
{
		if(devices_.contains(creds.id()))
				return;

		devices_.insert(creds.id(), std::make_pair(creds, nullptr));
}

void DevicesManager::Connect(const QString &id, std::function<void()> handler)
{
		QtConcurrent::run([id, handler, this]()
		{
				auto it = devices_.find(id);
				if(it == devices_.end())
						{
								qDebug() << __FUNCTION__ << " not found device id: " << id;
								return;
						}

				auto& [creds, device] = (*it);

				using namespace _onvif;
				if(!device)
						{
								device = new Device(creds.ip.toStdString(), static_cast<short>(creds.port.toInt()));
								device->SetDeviceServiceURI(creds.uri.toStdString());
						}

				try
						{
								device->Init("admin", "admin");
						}
				catch (const std::exception& e)
						{
								qDebug() << "__FUCNTION__" << " issues with connection to device: " << e.what();
						}

				handler();
		});

}

void DevicesManager::Disconnect(const QString &/*id*/)
{

}

void DevicesManager::Remove(const QString &/*id*/)
{

}

void DevicesManager::slotAddDevice(QString ip, short port, QString deviceServiceURI)
{
		/*
			qDebug() << QString("Adding a new device ip: %1 port: %2 URI: %3").arg(ip).arg(port).arg(deviceServiceURI);

		using namespace _onvif;
			IDevice* device = new Device(ip.toStdString(), port);
			device->SetDeviceServiceURI(deviceServiceURI.toStdString());
		device->Init("admin", "admin");

		devices.push_back(device);

		QString deviceInfo;
		QTextStream stream(&deviceInfo);
			stream << ip << ":" << port;
		sigNewDeviceAdded(deviceInfo);
			*/
}

_onvif::IDevice* DevicesManager::getDevice(const QString& addressInfo)
{
		auto parsedInfo = addressInfo.split(":");
		if(parsedInfo.size() != 2) return nullptr;

		std::string ip = parsedInfo.first().toStdString();
		short port = parsedInfo.back().toShort();

		foreach (auto* device, devices)
				if(device->getIP() == ip && device->getPort() == port) return device;

		return nullptr;
}

_onvif::IDevice *DevicesManager::getDevice(int index)
{
		if(devices.size() > index)
				return devices.at(index);

		return nullptr;
}

void DevicesManager::slotAsyncGetProfile(const QString& deviceID, const QString& profileToken)
{
		auto device = getDevice(deviceID);
		//in theory, before emitting signal and invoking this slot
		//the value were checked already and no need to recheck again
		if(device == nullptr)
				return;

		asyncGetProfileResultHolder_ = device->GetProfile(profileToken.toStdString());

		emit sigAsyncGetProfileReady();
}

void DevicesManager::slotAsyncGetVideoSettings(const QString& deviceID)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				return;

		auto stdstrtokens = device->GetProfilesTokens();
		for(const auto& s : stdstrtokens)
				asyncGetProfileTokensResultHolder_.push_back(QString::fromStdString(s));

		//take the first profile as default and set it settings to the form
		asyncGetProfileResultHolder_ = device->GetProfile(stdstrtokens.front());

		emit sigAsyncGetVideoSettingsReady();
}

void DevicesManager::slotAsyncGetLiveInfo(const QString& deviceID)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				return;

		auto stdstrtokens = device->GetProfilesTokens();
		for(const auto& s : stdstrtokens)
				asyncGetProfileTokensResultHolder_.push_back(QString::fromStdString(s));

		//take the first profile as default and get live stream
		if(!stdstrtokens.empty())
				{
						liveUrlHolder_ = device->GetStreamUri(stdstrtokens.front()).c_str();
				}


		emit sigLiveInfoReady();
}

void DevicesManager::slotAsyncGetLiveUri(const QString& deviceID,
				const QString& profileToken)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				return;

		liveUrlHolder_ = device->GetStreamUri(profileToken.toStdString()).c_str();

		emit sigLiveUriReady();
}

void DevicesManager::slotSetVideoEncoderSetting(const QString& deviceID)
{
		auto device = getDevice(deviceID);
		//videoEncoderConfigHolder_ is filled by calling client previously
		//so just take it and send to a device
		//after need to clear the holder
		auto res = device->SetVideoEncoderSettings(*videoEncoderConfigHolder_);

		delete  videoEncoderConfigHolder_;
		videoEncoderConfigHolder_ = nullptr;

		qDebug() << "Sending video encoder settings result: " << res;
}

void DevicesManager::slotAddVideoEncoderConfig(const QString& deviceID,
				const QString& profile,
				const QString& config)
{
		auto device = getDevice(deviceID);
		auto res = device->AddVideoEncoderConfig(profile.toStdString(), config.toStdString());

		emit sigVideoEncoderConfigAdded();

		qDebug() << "DeviceMgr finished adding a new ve config to profile. res:" << res;
}

bool DevicesManager::asyncGetProfile(const QString& deviceID, const QString &profileToken)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async request profile!";
						return false;
				}

		emit sigAsyncGetProfile(deviceID, profileToken);

		return true;
}

bool DevicesManager::asyncGetVideoSettings(const QString & deviceID)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async request profile!";
						return false;
				}

		emit sigAsyncGetVideoSettings(deviceID);

		return true;
}

bool DevicesManager::asyncGetLiveInfo(const QString &deviceID)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async get live stream info!";
						return false;
				}

		emit sigAsyncGetLiveInfo(deviceID);

		return true;
}

bool DevicesManager::asyncGetLiveUri(const QString &deviceID, const QString &profile)
{
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async get live URI!";
						return false;
				}

		if(profile.isEmpty())
				{
						qDebug() << "Profile token could not be empty to async get live URI!";
						return false;
				}

		emit sigAsyncGetLiveUri(deviceID, profile);
		return true;
}

_onvif::ProfileSP DevicesManager::getAsyncGetProfileResult()
{
		_onvif::ProfileSP res = asyncGetProfileResultHolder_;
		asyncGetProfileResultHolder_.reset();
		return res;
}

void DevicesManager::getAsyncGetVideoSettingsResult(QStringList& tokens, _onvif::ProfileSP& profile)
{
		tokens = asyncGetProfileTokensResultHolder_;
		asyncGetProfileTokensResultHolder_.clear();
		profile = asyncGetProfileResultHolder_;
		asyncGetProfileResultHolder_.reset();
}

void DevicesManager::getLiveInfoResults(QStringList& tokens, QString &uri)
{
		std::swap(tokens, asyncGetProfileTokensResultHolder_);
		uri = liveUrlHolder_;

		liveUrlHolder_.clear();
		asyncGetProfileTokensResultHolder_.clear();
}

void DevicesManager::getLiveUriResult(QString &uri)
{
		uri = liveUrlHolder_;
		liveUrlHolder_.clear();
}

void DevicesManager::setVideoEncoderSettings(const QString& deviceID, const _onvif::VideoEncoderConfiguration& configs)
{
		qDebug() << "DevicesManager started sending video encoder settings";
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async request profile!";
						return;
				}

		videoEncoderConfigHolder_ = new _onvif::VideoEncoderConfiguration(configs);

		emit sigSetVideoEncoderSettings(deviceID);
}

bool DevicesManager::addVideoEncoderToProfile(const QString& deviceID, const QString& profile, const QString& config)
{
		qDebug() << "DevicesManager started adding video config to profile";
		auto device = getDevice(deviceID);
		if(device == nullptr)
				{
						qDebug() << "Could not find a device to async request profile!";
						return false;
				}

		//videoEncoderConfigHolder_ = new _onvif::VideoEncoderConfiguration(configs);

		emit sigAddVideoEncoderConfig(deviceID, profile, config);
		return true;
}
