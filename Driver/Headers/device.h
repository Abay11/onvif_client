#pragma once

#include "IDevice.h"
#include "types.h"

#include <string>

struct soap;

namespace _onvif
{
	class ConnectionInfo;
	class DeviceService;
	class MediaService;

	class Device : public IDevice
	{
	public:
		Device(const std::string& endpoint, short port);
		~Device();

		///IDevice
		virtual void Init(const std::string& login, const std::string& pass);
		virtual void SetCreds(const std::string& login, const std::string& pass);
		virtual void StartLive();
		virtual void StopLive();
		virtual std::string GetServiceAddress(SERVICES service);

		//Media
		virtual VideoSourcesSP GetVideoSources();

		///IDevice

	public:
		//some helpers
		void fillONVIFGeneralInfo(); //should be used in the Init(), to fill IDevice member params


	private:
		soap* soap_context_;
		ConnectionInfo* conn_info_;
		DeviceService* device_service_;
		MediaService* media_service_;
	};

	//helpers
	bool isMedia2Supported(const Services* services);
}