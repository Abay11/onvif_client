#pragma once

#include "QObject"

class DevicesManager : public QObject
{
    Q_OBJECT

public slots:
    void slotAddDevice(std::string ip, short port);

public:

private:

};
