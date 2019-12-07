#include "..\Headers\device.h"

#include "device_service.h"
#include "media_service.h"

#include <iostream>
#include <sstream>

namespace _onvif
{
	Device::Device(const std::string& endpoint, short port)
	{
		std::stringstream ss;
		ss << endpoint << ":" << port;
		endpoint_ = ss.str();
	}

	Device::~Device()
	{
		soap_destroy(soap_context_);
		soap_end(soap_context_);
		soap_free(soap_context_);
	}

	void Device::Init()
	{
		soap_context_ = soap_new();
		soap_register_plugin(soap_context_, soap_wsse);
		
		std::stringstream device_address;
		device_address << "http://" << endpoint_ << "/onvif/device_service";

		device_service_ = new DeviceService(soap_context_, device_address.str());

		services_ = device_service_->get_service_addresses();

		capabilities_ = device_service_->get_capabilities();
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
