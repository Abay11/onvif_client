#pragma once

#include "IDevice.h"
#include "types.h"

#include <string>

struct soap;

namespace _onvif
{
	class DeviceService;
	class MediaService;

	class Device : public IDevice
	{
	public:
		Device(const std::string& endpoint, short port);
		~Device();

		//IDevice
		virtual void Init();
		virtual void SetAddressInfo(const std::string& ip, short port, const std::string& deviceServiceURI);
		virtual void SetCreds(const char* login, const char* pass);
		virtual void StartLive();
		virtual void StopLive();

	private:
		soap* soap_context_;
		DeviceService* device_service_;
		MediaService* media_service_;
	};
}