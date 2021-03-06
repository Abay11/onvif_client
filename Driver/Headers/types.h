#pragma once

#include <string>
#include <memory>
#include <vector>
#include <list>

namespace _onvif
{
	using StringList = std::vector<std::string>;

	//namespaces
	enum SERVICES
	{
		DEVICE_SERVICE,
		MEDIA_SERVICE,
		MEDIA2_SERVICE,
		EVENTS_SERVICE,
		PTZ_SERVICE,
		IMAGING_SERVICE,
		ANALYTICS_SERVICE,
		RECORDING_SERVICE,
		SEARCH_SERVICE,
		REPLAY_SERVICE,
		DEVICEIO_SERVICE
	};

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

	struct ONVIFGeneralInfo
	{
		std::string onvif_version;
		bool isMedia2Supported = false;
		std::string deviceServiceURI;
	};
	using ONVIFGeneralInfoSP = std::shared_ptr<ONVIFGeneralInfo>;

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

	/*struct VideoSourceConfiguration
	{
		StringList tokens;
		std::string bounds;
	};
	using VideoSourceSP = std::shared_ptr<VideoSourceConfiguration>;
	using VideoSources = std::list<VideoSourceSP>;
	*/

	enum VideoEncoding
	{
		//value and sequence should be the same
		//as in gSoap
		JPEG = 0,
		MPEG,
		H264
	};

	enum H264Profile
	{
		Baseline = 0,
		Main = 1,
		Extended = 2,
		High = 3
	};

	enum MPEGProfile
	{
		SP = 0,
		ASP = 1
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
		std::string source_token; //also here are @token from base class - is the config token themself
		std::string bounds;
	};
	using VSrcConfigSP = std::shared_ptr<VideoSourceConfiguration>;
	using VideoSources = std::shared_ptr<std::vector<VSrcConfigSP>>;

	class VideoEncoderConfiguration : public BaseConfiguration
	{
	public:
		std::string encoding;
		std::string resolution;
		float quality;
		int framerate;
		int encoding_interval;
		int bitrate;
		std::string codec_profile;
		int gov_length;
		/*
			TODO: additional MPEG4 configurations
		*/
		std::string multicast_ip;
		int multicast_port;
		int multicast_ttl;
		bool multicast_autostart;
		std::string session_timeout;
		//bool guaranteed_framerate; not used by current realisation
	};
	using VEncoderConfigSP = std::shared_ptr<VideoEncoderConfiguration>;
	using VEncoders = std::shared_ptr<std::vector<VEncoderConfigSP>>;

	//Simple uses for JPEG codec,
	//other codes have GOV length, etc.
	struct SimpleCodecOptions
	{
		bool isInit = false;

		std::vector<std::string> Resolutions;

		int FrameRateMin = 0;
		int FrameRateMax = 0;

		int EncodingIntervalMin = 0;
		int EncodingIntervalMax = 0;

		int BitrateRangeMin = 0;
		int BitrateRangeMax = 0;
	};

	//Extended SimpleCodecOptions for H264, MPEG etc
	struct ComplexCodecOptions : public SimpleCodecOptions
	{
		int GovLengthMin = 0;
		int GovLengthMax = 0;

		//I think this one can be used for both h264 and mpeg profiles
		std::vector<std::string> Profiles;
	};

	class VideoEncoderOptions
	{
	public:
		int QualityMin = 0;
		int QualityMax = 0;
		
		std::vector<std::string> Resolutions;

		SimpleCodecOptions JPEGOptions;
		ComplexCodecOptions MPEGOptions;
		ComplexCodecOptions H264Options;
	};
	using VideoEncoderOptionsSP = std::shared_ptr<VideoEncoderOptions>;

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

		VSrcConfigSP videoSource;
		VideoSources compatibleVideoSources;
		VEncoderConfigSP videoEncoder;
		VEncoders compatibleVideoEncoders;
		VideoEncoderOptionsSP videoEncoderOptions;
		AudioSourceConfiguration* audioSource = nullptr;
		AudioEncoderConfiguration* audioEncoder = nullptr;
		VideoAnalyticsConfiguration* videoAnalytics = nullptr;
		PTZConfiguration* ptz = nullptr;
		MetadataConfiguration* metadata = nullptr;

		~Profile()
		{
			delete audioSource;
			delete audioEncoder;
			delete videoAnalytics;
			delete ptz;
			delete metadata;
		}
	};

	using ProfileSP = std::shared_ptr<Profile>;
	using Profiles = std::shared_ptr<std::vector<ProfileSP>>;
}