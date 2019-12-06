#include "..\Headers\device.h"

#include "device_service.h"
#include "media_service.h"

#include <iostream>

namespace _onvif
{
	Device::Device(const std::string& endpoint, short port)
		: endpoint_(endpoint),
		port_(port)
	{

	}

	Device::~Device()
	{
		soap_destroy(soap_context);
		soap_end(soap_context);
		soap_free(soap_context);
	}

	void Device::Init()
	{
		soap_context = soap_new();
		soap_register_plugin(soap_context, soap_wsse);
	}

	void Device::SetCreds(const char* login, const char* pass)
	{
		login_ = login;
		pass_ = pass;
	}

	void Device::StartLive()
	{
		std::cout << "Start" << std::endl;
	}

	void Device::StopLive()
	{
		std::cout << "Stop" << std::endl;
	}
}
