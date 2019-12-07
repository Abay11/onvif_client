#pragma once

#include "IDevice.h"
#include "types.h"

#include <string>

#include "soapStub.h"

namespace _onvif
{
	class DeviceService;

	class Device : public IDevice
	{
	public:
		Device(const std::string& endpoint, short port);
		~Device();

		//IDevice
		virtual void Init();
		virtual void SetCreds(const char* login, const char* pass);
		virtual void StartLive();
		virtual void StopLive();

	private:
		std::string endpoint_;
		std::string login_;
		std::string pass_;

		soap* soap_context_;
		DeviceService* device_service_;
	};

}