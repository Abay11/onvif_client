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
		void slotAddDevice(QString ip, short port, QString deviceServiceURI);

public:
		_onvif::IDevice* getDevice(const QString& addressInfo);
		_onvif::IDevice* getDevice(int index);

private:
    QList<_onvif::IDevice*> devices;
};
