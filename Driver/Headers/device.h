#pragma once

#include "IDevice.h"
#include "IAudioOutput.h"
//#include "IReplayControl.h"
#include "types.h"

#include <string>

struct soap;

class IReplayControl;
namespace _onvif
{
	class ConnectionInfo;
	class DeviceService;
	class MediaService;
	class EventService;

	class Device : public IDevice, public IAudioOutput
	{
	public:
		Device(const std::string& endpoint, short port);
		virtual ~Device() override;

		///IDevice
		virtual void Init(const std::string& login, const std::string& pass);
		virtual void SetCreds(const std::string& login, const std::string& pass);
		virtual void StartLive();
		virtual void StopLive();
		virtual std::string GetServiceAddress(SERVICES service);

		//Media
		virtual StringList GetProfilesTokens() const override;
		virtual Profiles GetProfiles() const override;
		virtual ProfileSP GetProfile(const std::string& token) const override;
		virtual VideoSources GetVideoSources() const override;
		virtual VideoSources GetCompatibleVideoSources(const std::string& profile) const override;
		virtual VideoEncoderOptionsSP GetVideoEncoderOptions(const std::string& profile, const std::string& encToken) const override;
		virtual VEncoders GetVideoEncoders(const std::string& profile) const override;
		virtual std::string GetStreamUri(const std::string& profileToken,
			StreamType type,
			TransportProtocol transport) const override;

		//Media applying settings
		virtual bool SetVideoEncoderSettings(const VideoEncoderConfiguration&) const override;
		virtual bool AddVideoEncoderConfig(const std::string& /*profile*/, const std::string& /*vetoken*/) const override;

		// Inherited via IDevice
		virtual void SubcribeEvents() const override;
		virtual void UnsubcribeEvents() const override;

		///IDevice

		/// Inherited via IAudioOutput
		virtual void Start() override;

		virtual void Stop() override;

		virtual void Send(void*, size_t) override;

		/// IAudioOutput end

	public:
		//some helpers
		void fillONVIFGeneralInfo(); //should be used in the Init(), to fill IDevice member params


	private:
		soap* soap_context_;
		ConnectionInfo* conn_info_ = nullptr;
		DeviceService* device_service_ = nullptr;
		MediaService* media_service_ = nullptr;
		EventService* event_service_ = nullptr;

	};

	//helpers
	bool isMedia2Supported(const Services* services);
}