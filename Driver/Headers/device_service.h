#pragma once

#include "types.h"

#include <string>
#include <list>

class DeviceBindingProxy;

namespace _onvif
{
	class ConnectionInfo;
		
	class DeviceService
	{
	public:
		/**
		A soap context should be valid until a class object will be destroyed
		**/
		DeviceService(ConnectionInfo* connInfo, const std::string& endpoint);
		~DeviceService();

		CapabilitiesSP get_capabilities();

		std::string get_date_time();
		DeviceInformationSP get_device_info();

		Services get_service_addresses();

		std::list<std::string> get_scopes();

	private:
		ConnectionInfo* conn_info_;
		DeviceBindingProxy* deviceProxy;

		std::string endpoint_reference_;
	};
	
	//returns the address of the specific service
	std::string get_service_address(const Services* services, SERVICES service);
}