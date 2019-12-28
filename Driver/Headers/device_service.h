#pragma once

#include "types.h"

#include <string>
#include <list>

struct soap;
class DeviceBindingProxy;

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
		DeviceInformationSP get_device_info();

		Services get_service_addresses();
		static std::string get_service_address(const Services* services, const char* service_namespace);

		std::list<std::string> get_scopes();

	private:
		soap* soap_context_;
		DeviceBindingProxy* deviceProxy;

		std::string endpoint_reference_;
	};
}