#include "DevicesManager.h"

#include <QDebug>
#include <QString>
#include <QTextStream>

#include "device.h"

DevicesManager::DevicesManager(QObject *parent)
	: QObject(parent)
{
	connect(this, &DevicesManager::sigAsyncGetProfile,
					this, &DevicesManager::slotAsyncGetProfile);
	connect(this, &DevicesManager::sigAsyncGetVideoSettings,
					this, &DevicesManager::slotAsyncGetVideoSettings);
	connect(this, &DevicesManager::sigAddVideoEncoderConfig,
					this, &DevicesManager::slotAddVideoEncoderConfig);
}

void DevicesManager::slotAddDevice(QString ip, short port, QString deviceServiceURI)
{
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

#include <QVector>

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
