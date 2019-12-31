#include "..\Headers\device.h"

#include "device_service.h"
#include "media_service.h"

#include "soapStub.h"
#include "wsseapi.h"

#include <iostream>
#include <sstream>

namespace _onvif
{
	Device::Device(const std::string& endpoint, short port)
	{
		ip_ = endpoint;
		port_ = port;
	}

	Device::~Device()
	{
		delete device_service_;
		delete media_service_;

		soap_destroy(soap_context_);
		soap_end(soap_context_);
		soap_free(soap_context_);
	}

	void Device::Init(const std::string& login, const std::string& pass)
	{
		soap_context_ = soap_new();
		soap_register_plugin(soap_context_, soap_wsse);

		login_ = login;
		pass_ = pass;

		std::stringstream device_address;
		device_address << "http://" << ip_ << ":" << port_ << device_service_uri_;

		device_service_ = new DeviceService(soap_context_, device_address.str());

		capabilities_ = device_service_->get_capabilities();
		
		services_ = device_service_->get_service_addresses();

		device_info_ = device_service_->get_device_info();

		std::string addr = DeviceService::get_service_address(&services_, MEDIA_SERVICE_NS);
		media_service_ = new MediaService(soap_context_, addr);
	}

	void Device::SetAddressInfo(const std::string& ip, short port, const std::string& deviceServiceURI)
	{
		ip_ = ip;
		port_ = port;
		device_service_uri_ = deviceServiceURI;
	}

	void Device::SetCreds(const std::string& login, const std::string& pass)
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
