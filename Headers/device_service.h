#pragma once

#include <list>

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	class DeviceService
	{
	public:
		DeviceService(soap* soap, const char* endpoint);
		~DeviceService();

		std::list<std::string> get_scopes();
		std::string get_date_time();

		struct DeviceInformation;
		DeviceInformation get_device_info();

	public:
		struct DeviceInformation
		{
			std::string manufacturer;
			std::string model;
			std::string firmwareVersion;
			std::string serialNumber;
			std::string hardwareId;

			bool filled = false;
		};

	private:
		soap* soap_context;
		DeviceBindingProxy deviceProxy;

		char endpoint_reference[128];
	};
}