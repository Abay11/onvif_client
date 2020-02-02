#include "DevicesManager.h"

#include <QDebug>
#include <QString>
#include <QTextStream>

#include "device.h"

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
