#pragma once

#include "types.h"

#include <string>

namespace _onvif
{
	class IDevice
	{
	public:
		IDevice(const std::string& endpoint, short port)
			: ip_(endpoint)
			, port_(port)
		{}

		virtual ~IDevice() {}

		//should be implemented by child
		virtual void Init(const std::string& login, const std::string& pass) = 0;

		virtual void SetAddressInfo(const std::string& ip, short port) { ip_ = ip; port_ = port; };
		
		//should be used before Init() if the URI is specific
		virtual void SetDeviceServiceURI(const std::string& deviceServiceURI) { device_service_uri_ = deviceServiceURI; };

		//no realisation cause may be complited additional actions
		//used to update Creds data
		virtual void SetCreds(const std::string& login, const std::string& pass) = 0;

		virtual void StartLive() = 0;

		virtual void StopLive() = 0;

		std::string getIP() { return ip_; }
		short getPort() { return port_; };

		//should return a full address like http://ip:port//uri
		virtual std::string GetServiceAddress(SERVICES service) = 0;
		
		const CapabilitiesSP getCapabilities() const { return capabilities_; }
        const DeviceInformationSP getDeviceInfo() const { return device_info_; }
        const ONVIFGeneralInfoSP getONVIFGeneralInfo() const { return onvif_general_info_; }

		///////////////////////Media/////////////////////////
		virtual StringList GetProfilesTokens() const = 0;
		virtual Profiles GetProfiles() const = 0;
		virtual ProfileSP GetProfile(const std::string& token) const = 0;
		virtual VideoSources GetVideoSources() const = 0;
		virtual VideoSources GetCompatibleVideoSources(const std::string& profile) const = 0;
		virtual VideoEncoderOptionsSP GetVideoEncoderOptions(const std::string& profile, const std::string& encToken) const = 0;
		virtual VEncoders GetVideoEncoders(const std::string& profile) const = 0;
		virtual std::string GetStreamUri(const std::string& profileToken,
			StreamType type = StreamType::Unicast,
			TransportProtocol transport = TransportProtocol::UDP) const = 0;

		//APPLYING NEW SETTINGS
		//Following methods return true if settings applied successfully, otherwise false

		//Sending new settings
		virtual bool SetVideoEncoderSettings(const VideoEncoderConfiguration& /*new_configs*/) const = 0;
		virtual bool AddVideoEncoderConfig(const std::string& /*profile*/, const std::string& /*vetoken*/) const = 0;
		///////////////////////Media/////////////////////////


		//Event service
		virtual void SubcribeEvents() const = 0;
		virtual void UnsubcribeEvents() const = 0;
		
	protected:		
		Services services_;
		DeviceInformationSP device_info_;
		CapabilitiesSP capabilities_;
		ONVIFGeneralInfoSP onvif_general_info_;

		std::string ip_;
		short port_;
		std::string device_service_uri_ = "/onvif/device_service";

		std::string login_;
		std::string pass_;
	};
}