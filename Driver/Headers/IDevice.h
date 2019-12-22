#pragma once

#include "types.h"

#include <string>

namespace _onvif
{
	class IDevice
	{
	public:
		virtual void Init() = 0;

		virtual void SetAddressInfo(const std::string& ip, short port, const std::string& deviceServiceURI) = 0;

		virtual void SetCreds(const char* login, const char* pass) = 0;

		virtual void StartLive() = 0;

		virtual void StopLive() = 0;
	
	protected:		
		Services services_;
		CapabilitiesSP capabilities_;

		std::string ip_;
		short port_;
		std::string device_service_uri_;

		std::string login_;
		std::string pass_;
	};
}