#pragma once

#include "types.h"

#include <string>

namespace _onvif
{
	class IDevice
	{
	public:
		virtual void Init(const std::string& login, const std::string& pass) = 0;

		virtual void SetAddressInfo(const std::string& ip, short port, const std::string& deviceServiceURI) = 0;

		virtual void SetCreds(const std::string& login, const std::string& pass) = 0;

		virtual void StartLive() = 0;

		virtual void StopLive() = 0;

		std::string getIP() { return ip_; }
		short getPort() { return port_; };

		const CapabilitiesSP getCapabilities() const { return capabilities_; };
		const DeviceInformationSP getDeviceInfo() const { return device_info_; };
	
	protected:		
		Services services_;
		DeviceInformationSP device_info_;
		CapabilitiesSP capabilities_;

		std::string ip_;
		short port_;
		std::string device_service_uri_;

		std::string login_;
		std::string pass_;
	};
}