#pragma once

#include "QObject"

#include <QList>

namespace _onvif
{
class IDevice;
}

class DevicesManager : public QObject
{
    Q_OBJECT

signals:
    void sigNewDeviceAdded(QString address);

public slots:
    void slotAddDevice(std::string ip, short port, std::string deviceServiceURI);

public:
		_onvif::IDevice* getDevice(const QString& addressInfo);
		_onvif::IDevice* getDevice(int index);

private:
    QList<_onvif::IDevice*> devices;
};
