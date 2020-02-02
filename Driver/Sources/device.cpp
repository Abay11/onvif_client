#include "..\Headers\device.h"

#include "SoapHelpers.h"
#include "device_service.h"
#include "media_service.h"

#include "soapStub.h"

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
		soap_register_plugin(soap_context_, http_da);

		login_ = login;
		pass_ = pass;

		conn_info_ = new ConnectionInfo(soap_context_, ip_, port_, login_, pass_);

		std::stringstream device_address;
		device_address << "http://" << ip_ << ":" << port_ << device_service_uri_;

		device_service_ = new DeviceService(conn_info_, device_address.str());

		capabilities_ = device_service_->get_capabilities();
		
		services_ = device_service_->get_service_addresses();

		device_info_ = device_service_->get_device_info();

		std::string addr = get_service_address(&services_, SERVICES::MEDIA_SERVICE);
		media_service_ = new MediaService(conn_info_, addr);

		fillONVIFGeneralInfo();
	}

	void Device::SetCreds(const std::string& login, const std::string& pass)
	{
		login_ = login;
		pass_ = pass;

		if (conn_info_)
		{
			conn_info_->setCreds(login_, pass_);
		}
	}

	void Device::StartLive()
	{
		std::cout << "Start" << std::endl;
	}

	void Device::StopLive()
	{
		std::cout << "Stop" << std::endl;
	}
	
	std::string Device::GetServiceAddress(SERVICES service)
	{
		std::stringstream address_stream;
		address_stream << "http://" << ip_ << ":" << port_ << get_service_address(&services_, service);

		return address_stream.str();
	}

	StringList Device::GetProfilesTokens() const
	{
		StringList tokens;

		Profiles profiles = media_service_->get_profiles();
		if (profiles)
		{
			for (const auto& t : *profiles)
				if(t) tokens.push_back(t->token);
		}

		return tokens;
	}

	Profiles Device::GetProfiles() const
	{
		return media_service_->get_profiles();
	}

	ProfileSP Device::GetProfile(const std::string& token) const
	{
		//at first fill current options the specified profile
		//then append some compatible options for different
		//device's components

		auto profile =  media_service_->get_profile(token);

		if (profile)
		{
			//compatible video sources
			if (profile->videoSource)
				profile->compatibleVideoSources = media_service_->get_compatible_videosources(token);

			//compatible video encoders
			if (profile->videoEncoder)
				profile->compatibleVideoEncoders = media_service_->get_compatible_videoencoders(token);

			//video encoder options
			if (profile->videoEncoder)
				profile->videoEncoderOptions = media_service_->get_videoencoders_opts(token, profile->videoEncoder->token);
		}

		return profile;
	}

	VideoSources Device::GetVideoSources() const
	{
		return media_service_->get_video_sources();
	}

	VideoSources Device::GetCompatibleVideoSources(const std::string& profile) const
	{
		return media_service_->get_compatible_videosources(profile);
	}

	VideoEncoderOptionsSP Device::GetVideoEncoderOptions(const std::string& profile, const std::string& encToken) const
	{
		return media_service_->get_videoencoders_opts(profile, encToken);
	}

	VEncoders Device::GetVideoEncoders(const std::string& profile) const
	{
		return media_service_->get_compatible_videoencoders(profile);
	}

	void Device::fillONVIFGeneralInfo()
	{
		if (!onvif_general_info_) onvif_general_info_ = std::make_shared<ONVIFGeneralInfo>();

		if(capabilities_->device_info_filled)
			onvif_general_info_->onvif_version = capabilities_->device_supported_versions.back(); // take the last one
		
		onvif_general_info_->isMedia2Supported = isMedia2Supported(&services_);
		onvif_general_info_->deviceServiceURI = device_service_uri_;
	}

	bool isMedia2Supported(const Services* services)
	{
		for (const auto s : *services)
		{
			if (s && s->ns.find("/ver20/media/") != std::string::npos) return true;
		}

		return false;
	}
}
