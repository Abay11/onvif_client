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
		Param device_serivce_uri should to point to the full device service address
		including protocol, ip and port
		**/
		DeviceService(ConnectionInfo* connInfo, const std::string& device_service_uri);
		~DeviceService();

		CapabilitiesSP get_capabilities();

		std::string get_date_time();
		DeviceInformationSP get_device_info();

		Services get_service_addresses();

		std::list<std::string> get_scopes();

	private:
		ConnectionInfo* conn_info_;
		DeviceBindingProxy* deviceProxy;

		std::string device_service_uri_;
	};
	
	//returns the address of the specific service
	std::string get_service_address(const Services* services, SERVICES service);
}