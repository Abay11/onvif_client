#include "DevicesManager.h"

#include <QDebug>
#include <QString>

void DevicesManager::slotAddDevice(std::string ip, short port)
{
    qDebug() << QString("Adding a new device ip: %1 port: %2").arg(ip.c_str()).arg(port);
    //do adding a new device

    //using namespace _onvif;
    //IDevice* device = new Device("192.168.43.196", 8000);
    //device->Init();
}
