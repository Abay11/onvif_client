#pragma once

#include <string>
#include <memory>

namespace _onvif
{
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

	struct AudioEncoderConfiguration
	{
		AudioEncoding encoding;
		int bitrate;
		int sampleRate;
	};

	struct VideoAnalyticsConfiguration
	{
		//TODO
	};
	
	struct PTZConfiguration
	{
		std::string nodeToken;
	};

	struct MetadataConfiguration
	{
		//TODO	
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

	using Profiles = std::list<std::shared_ptr<Profile>>;

}