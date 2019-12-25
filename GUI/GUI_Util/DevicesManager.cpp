#include "DevicesManager.h"

#include <QDebug>
#include <QString>
#include <QTextStream>

#include "device.h"

void DevicesManager::slotAddDevice(std::string ip, short port, std::string deviceServiceURI)
{
    qDebug() << QString("Adding a new device ip: %1 port: %2 URI: %3").arg(ip.c_str()).arg(port).arg(deviceServiceURI.c_str());

    using namespace _onvif;
    IDevice* device = new Device(ip, port);
    device->Init();

    devices.push_back(device);

    QString deviceInfo;
    QTextStream stream(&deviceInfo);
    stream << ip.c_str() << ":" << port;
    sigNewDeviceAdded(deviceInfo);
}

const _onvif::IDevice* DevicesManager::getDevice(const QString& addressInfo)
{
    auto parsedInfo = addressInfo.split(":");
    if(parsedInfo.size() != 2) return nullptr;

    std::string ip = parsedInfo.first().toStdString();
    short port = parsedInfo.back().toShort();

    foreach (auto* device, devices)
      if(device->getIP() == ip && device->getPort() == port) return device;

    return nullptr;
}
