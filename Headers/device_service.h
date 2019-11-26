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

	private:
		soap* soap_context;
		DeviceBindingProxy deviceProxy;

		char endpoint_reference[128];
	};
}