#pragma once

#include "types.h"

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "wsseapi.h"

#include <string>
#include <list>

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	class DeviceService
	{
	public:
		/**
		A soap context should be valid until a class object will be destroyed
		**/
		DeviceService(soap* soap, const std::string& endpoint);
		~DeviceService();

		CapabilitiesSP get_capabilities();

		std::string get_date_time();
		struct DeviceInformation;
		DeviceInformation get_device_info();

		Services get_service_addresses();

		std::list<std::string> get_scopes();

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

		std::string endpoint_reference_;
	};
}