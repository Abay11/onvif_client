#pragma once

#include <list>
#include <string>

#include "types.h"

struct soap;
class MediaBindingProxy;

namespace _onvif
{
	class ConnectionInfo;

	class MediaService
	{
	public:
		/**
		A ConnectionInfo instance should be valid until a class object will be destroyed
		and should be destroyed by client.
		**/
		MediaService(ConnectionInfo* connInfo, const std::string& media_service_uri);
		~MediaService();

	public:
		Profiles get_profiles();
		ProfileSP get_profile(const std::string& token);
		VideoEncoderOptionsSP get_videoencoders_opts(const std::string& profile_token, const std::string& enc_token);
		VEncoders get_compatible_videoencoders(const std::string& token);
		VideoSources get_video_sources();
		VideoSources get_compatible_videosources(const std::string& profile_token);
		std::string get_stream_uri(const std::string& profileToken, StreamType type = Unicast, TransportProtocol transport = UDP);

		bool add_videoencoder_config(const std::string& /*profiletoken*/, const std::string& /*vetoken*/);

	private:
		ConnectionInfo* conn_info_;
		MediaBindingProxy* mediaProxy;

		std::string media_service_uri_;
	};
}