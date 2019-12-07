#pragma once

#include <string>
#include <memory>
#include <vector>
#include <list>

namespace _onvif
{
	struct Service
	{
		std::string ns;
		std::string xaddr;
		std::string version;
	};

	using ServiceSP = std::shared_ptr<Service>;
	using Services = std::vector<ServiceSP>;

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