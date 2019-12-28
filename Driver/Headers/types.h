#pragma once

#include <string>
#include <memory>
#include <vector>
#include <list>

namespace _onvif
{
	//namespaces
	extern const char* DEVICE_SERVICE_NS;
	extern const char* MEDIA_SERVICE_NS;
	extern const char* MEDIA2_SERVICE_NS;
	extern const char* EVENTS_SERVICE_NS;
	extern const char* PTZ_SERVICE_NS;
	extern const char* IMAGING_SERVICE_NS;
	extern const char* ANALYTICS_SERVICE_NS;
	extern const char* RECORDING_SERVICE_NS;
	extern const char* SEARCH_SERVICE_NS;
	extern const char* REPLAY_SERVICE_NS;
	extern const char* DEVICEIO_SERVICE_NS;

	struct DeviceInformation
	{
		std::string manufacturer;
		std::string model;
		std::string firmwareVersion;
		std::string serialNumber;
		std::string hardwareId;

		bool filled = false;
	};
	using DeviceInformationSP = std::shared_ptr<DeviceInformation>;

	struct Service
	{
		std::string ns;
		std::string xaddr;
		std::string version;
	};

	using ServiceSP = std::shared_ptr<Service>;
	using Services = std::vector<ServiceSP>;
	
	struct Capabilities
	{
		bool analytics_support = false;
		std::string analytics_xaddr;
		bool analytics_rule = false;
		bool analytics_module = false;

		//System
		bool device_info_filled = false;
		std::string device_xaddr;
		bool device_discovery_resolve = false;
		bool device_system_logging = false;
		std::vector<std::string> device_supported_versions;

		//EVENTS
		bool events_filled = false;
		bool events_wssubstription = false;
		bool events_wspullpoint = false;

		//IMAGING
		bool imaging_filled = false;
		std::string imaging_xaddr;

		//MEDIA
		bool media_filled = false;
		std::string media_xaddr;
		bool media_RTPMulticast = false;
		bool media_RTP_TCP = false;
		bool media_RTP_RTSP_TCP = false;

		//PTZ
		bool ptz_filled;
		std::string ptz_xaddr;

		//IO
		bool io_filled = false;
		std::string io_xaddr;
		int io_videoSources = 0;
		int io_videoOutputs = 0;
		int io_audioSources = 0;
		int io_audioOutputs = 0;
		int io_digitalInputs = 0;
		int io_relayOutputs = 0;

		//***Storage***
		//Recording
		bool recording_filled = false;
		std::string recording_xaddr;
		bool recording_from_profile = false;
		bool recording_from_receiver = false;
		//Search
		bool search_filled = false;
		std::string search_xaddr;
		bool search_metadata = false;
		//Replay
		bool replay_filled = false;
		std::string replay_xaddr;
	};

	using CapabilitiesSP = std::shared_ptr<Capabilities>;

	enum VideoEncoding
	{
		//value and sequence should be the same
		//as in gSoap
		JPEG = 0,
		MPEG,
		H264
	};

	enum AudioEncoding
	{
		G711,
		G726,
		AAC
	};

	enum StreamType
	{
		Unicast = 0,
		Multicase = 1
	};

	enum TransportProtocol
	{
		UDP = 0,
		TCP = 1,
		RTSP = 2,
		HTTP = 3
	};

	class BaseConfiguration
	{
	public:
		std::string name;
		std::string token;
		int useCount;
	};

	//Configuration types
	class VideoSourceConfiguration : public BaseConfiguration
	{
	public:
	};

	class VideoEncoderConfiguration : public BaseConfiguration
	{
	public:
		VideoEncoding encoding;
	};

	class AudioSourceConfiguration : public BaseConfiguration
	{
	public:
	};

	class AudioEncoderConfiguration : public BaseConfiguration
	{
	public:
		AudioEncoding encoding;
		int bitrate;
		int sampleRate;
	};

	class VideoAnalyticsConfiguration : public BaseConfiguration
	{
	public:
		//TODO
	};
	
	class PTZConfiguration : public BaseConfiguration
	{
	public:
		std::string nodeToken;
	};

	class MetadataConfiguration : public BaseConfiguration
	{
	public:
	};

	struct Profile
	{
		std::string Name;
		std::string token;
		bool* fixed;

		VideoSourceConfiguration* videoSource = nullptr;
		VideoEncoderConfiguration* videoEncoder = nullptr;
		AudioSourceConfiguration* audioSource = nullptr;
		AudioEncoderConfiguration* audioEncoder = nullptr;
		VideoAnalyticsConfiguration* videoAnalytics = nullptr;
		PTZConfiguration* ptz = nullptr;
		MetadataConfiguration* metadata = nullptr;

		~Profile()
		{
			delete videoSource;
			delete videoEncoder;
			delete audioSource;
			delete audioEncoder;
			delete videoAnalytics;
			delete ptz;
			delete metadata;
		}
	};

	using ProfileSP = std::shared_ptr<Profile>;
	using Profiles = std::list<ProfileSP>;
}