#include "media_service.h"
#include "SoapHelpers.h"
#include "types.h"

#include "soapStub.h"
#include "soapMediaBindingProxy.h"
#include "wsseapi.h"

#include <list>
#include <sstream>

extern SOAP_NMAC struct Namespace namespaces[];

//free-helpers functions
std::string encodingToString(_onvif::VideoEncoding e)
{
	using namespace _onvif;

	switch (e)
	{
	case VideoEncoding::H264: return "H264";
	case VideoEncoding::JPEG: return "JPEG";
	case VideoEncoding::MPEG: return "MPEG";
	}
}

std::string resolutionToString(int weight, int height)
{
	std::stringstream ss;
	ss << weight << "x" << height;
	return ss.str();
}

std::string h264ProfileToString(_onvif::H264Profile profile_code)
{
	using namespace _onvif;

	switch (profile_code)
	{
	case Baseline: return "Baseline";
	case Main: return "Main";
	case Extended: return "Extended";
	case High: return "High";
	}
}

_onvif::VideoEncoderConfigurationSP soapVEncoderToEncoder(const tt__VideoEncoderConfiguration* svec)
{
	using namespace _onvif;

	VideoEncoderConfigurationSP vencoder;
	if (svec)
	{
		vencoder = std::make_unique<VideoEncoderConfiguration>();

		vencoder->name = svec->Name;
		vencoder->token = svec->token;
		vencoder->useCount = svec->UseCount;
		//this convertion correct only if order of the encoder the same in both types
		vencoder->encoding = encodingToString(static_cast<_onvif::VideoEncoding>(svec->Encoding));

		vencoder->resolution = resolutionToString(svec->Resolution->Width,
			svec->Resolution->Height);
		vencoder->quality = svec->Quality;
		vencoder->framerate = svec->RateControl->FrameRateLimit;
		vencoder->encoding_interval = svec->RateControl->EncodingInterval;
		vencoder->bitrate = svec->RateControl->BitrateLimit;

		if (auto h264_props = svec->H264)
		{
			//this convertion is dangerous and the order of the enums should be exactly the same
			vencoder->codec_profile = h264ProfileToString(
				static_cast<_onvif::H264Profile>(h264_props->H264Profile));
			vencoder->gov_length = h264_props->GovLength;
		}

		/* NOTE: current implementation not use MPEG4 extentions
		if (auto mpeg4_props = gp->VideoEncoderConfiguration->MPEG4)
		{
			mpeg4_props->Mpeg4Profile;
		}
		*/

		if (auto ipv4 = svec->Multicast->Address->IPv4Address)
			vencoder->multicast_ip = *ipv4;
		else if (auto ipv6 = svec->Multicast->Address->IPv6Address)
			vencoder->multicast_ip = *ipv6;

		vencoder->multicast_port = svec->Multicast->Port;
		vencoder->multicast_ttl = svec->Multicast->TTL;
		vencoder->multicast_autostart = svec->Multicast->AutoStart;

		vencoder->session_timeout = svec->SessionTimeout;
	}

	return vencoder;
}

//help to copy data from gSoap type Profile
//to our type Profile
void soapProfileToProfile(const tt__Profile* gp, _onvif::Profile& p)
{
	p.Name = gp->Name;
	p.token = gp->token;
	p.fixed = gp->fixed;

	if (gp->VideoSourceConfiguration)
	{
		p.videoSource = new _onvif::VideoSourceConfiguration;
		
		p.videoSource->name = gp->VideoSourceConfiguration->Name;
		p.videoSource->token = gp->VideoSourceConfiguration->token;
		p.videoSource->useCount = gp->VideoSourceConfiguration->UseCount;

		if(auto* bounds = gp->VideoSourceConfiguration->Bounds)
			p.videoSource->bounds = resolutionToString(bounds->width, bounds->height);
	}

	p.videoEncoder = soapVEncoderToEncoder(gp->VideoEncoderConfiguration);

	if (gp->AudioSourceConfiguration)
	{
		p.audioSource = new _onvif::AudioSourceConfiguration;

		p.audioSource->name = gp->AudioEncoderConfiguration->Name;
		p.audioSource->token = gp->AudioEncoderConfiguration->token;
		p.audioSource->useCount = gp->AudioEncoderConfiguration->UseCount;
	}

	if (gp->AudioEncoderConfiguration)
	{
		p.audioEncoder = new _onvif::AudioEncoderConfiguration;

		p.audioEncoder->name = gp->AudioEncoderConfiguration->Name;
		p.audioEncoder->token = gp->AudioEncoderConfiguration->token;
		p.audioEncoder->useCount = gp->AudioEncoderConfiguration->UseCount;

		//the order of encoding should be the same as in the gSoap struct
		p.audioEncoder->encoding = static_cast<_onvif::AudioEncoding>(gp->AudioEncoderConfiguration->Encoding);
		p.audioEncoder->bitrate = gp->AudioEncoderConfiguration->Bitrate;
		p.audioEncoder->sampleRate = gp->AudioEncoderConfiguration->SampleRate;
	}

	if (gp->PTZConfiguration)
	{
		p.ptz = new _onvif::PTZConfiguration;

		p.ptz->name = gp->PTZConfiguration->Name;
		p.ptz->token = gp->PTZConfiguration->token;
		p.ptz->nodeToken = gp->PTZConfiguration->NodeToken;
		p.ptz->useCount = gp->PTZConfiguration->UseCount;
	}

	if (gp->MetadataConfiguration)
	{
		p.metadata = new _onvif::MetadataConfiguration;

		p.metadata->name = gp->MetadataConfiguration->Name;
		p.metadata->token = gp->MetadataConfiguration->token;
		p.metadata->useCount = gp->MetadataConfiguration->UseCount;
	}
}
//free-helpers functions

namespace _onvif
{
	MediaService::MediaService(ConnectionInfo* connInfo, const std::string& media_service_uri)
		:conn_info_(connInfo),
		mediaProxy(new MediaBindingProxy(connInfo->getSoap())),
		media_service_uri_(media_service_uri)
	{
		mediaProxy->soap_endpoint = media_service_uri_.c_str();
	}

	MediaService::~MediaService()
	{
		mediaProxy->destroy();
	}

	Profiles MediaService::get_profiles()
	{
		Profiles profiles;

		_trt__GetProfiles request;
		_trt__GetProfilesResponse response;

		auto wrapper = [this](_trt__GetProfiles* r1, _trt__GetProfilesResponse& r2) {return mediaProxy->GetProfiles(r1, r2); };
		int res = GSoapRequestWrapper<_trt__GetProfiles, _trt__GetProfilesResponse>(wrapper, &request, response, conn_info_);

		if (res == SOAP_OK)
		{
			for (auto gprofile : response.Profiles)
			{
				if (gprofile)
				{
					auto profile = std::make_shared<Profile>();
					soapProfileToProfile(gprofile, *profile);
					profiles.push_back(profile);
				}
			}
		}
		
		return profiles;
	}

	ProfileSP MediaService::get_profile(const std::string& token) {
		ProfileSP profile = std::make_shared<Profile>();

		_trt__GetProfile request;
		request.ProfileToken = token;
		_trt__GetProfileResponse response;
		if (!mediaProxy->GetProfile(&request, response))
		{
			if (response.Profile)
			{
				soapProfileToProfile(response.Profile, *profile);
			}
		}

		return profile;
	}

	VideoEncoderOptionsSP MediaService::get_videoencoders_opts(const std::string& profile_token, const std::string& enc_token)
	{
		VideoEncoderOptionsSP options;

		using T1 = _trt__GetVideoEncoderConfigurationOptions;
		using T2 = _trt__GetVideoEncoderConfigurationOptionsResponse;

		T1 request;
		//request.ProfileToken = token;
		T2 response;
		auto wrapper = [this](T1* r1, T2& r2) {return mediaProxy->GetVideoEncoderConfigurationOptions(r1, r2); };
		int res = GSoapRequestWrapper<T1, T2>(wrapper, &request, response, conn_info_);

		auto* opts = response.Options;
		if (res == SOAP_OK && opts)
		{
			options = std::make_shared<VideoEncoderOptions>();

			if (opts->QualityRange)
			{
				options->QualityMax = opts->QualityRange->Max;
				options->QualityMin = opts->QualityRange->Min;
			}

			if (auto* j = opts->JPEG)
			{
				auto& jpeg = options->JPEGOptions;
				jpeg.isInit = true;

				for (const auto* r : j->ResolutionsAvailable)
					if (r) jpeg.Resolutions.push_back(resolutionToString(r->Width, r->Height));

				if (const auto* f = j->FrameRateRange)
				{
					jpeg.FrameRateMax = f->Max;
					jpeg.FrameRateMax = f->Min;
				}

				if (const auto* e = j->EncodingIntervalRange)
				{
					jpeg.EncodingIntervalMax = e->Max;
					jpeg.EncodingIntervalMin = e->Min;
				}
			}

			/*
			if (const auto* m = opts->MPEG4)
			{
				auto& mpeg = options->MPEGOptions;
				mpeg.isInit = true;

				for (const auto* res : m->ResolutionsAvailable)
					if (res) mpeg.Resolutions.push_back(resolutionToString(res->Width, res->Height));

				if (const auto* gov = m->GovLengthRange)
				{
					mpeg.GovLengthMax = gov->Max;
					mpeg.GovLengthMin = gov->Min;
				}

				if (const auto* fr = m->FrameRateRange)
				{
					mpeg.FrameRateMax = fr->Max;
					mpeg.FrameRateMin = fr->Min;
				}

				if (const auto* ei = m->EncodingIntervalRange)
				{
					mpeg.EncodingIntervalMax = ei->Max;
					mpeg.EncodingIntervalMin = ei->Min;
				}

				// SKIP MPEG Profiles// for (const auto& prof : m->Mpeg4ProfilesSupported){}
			}
			*/

			if (const auto* h = opts->H264)
			{
				auto& h264 = options->H264Options;
				h264.isInit = true;

				for (const auto* res : h->ResolutionsAvailable)
					if (res) h264.Resolutions.push_back(resolutionToString(res->Width, res->Height));

				if (const auto* gov = h->GovLengthRange)
				{
					h264.GovLengthMax = gov->Max;
					h264.GovLengthMin = gov->Min;
				}

				if (const auto* fr = h->FrameRateRange)
				{
					h264.FrameRateMax = fr->Max;
					h264.FrameRateMin = fr->Min;
				}

				if (const auto* ei = h->EncodingIntervalRange)
				{
					h264.EncodingIntervalMax = ei->Max;
					h264.EncodingIntervalMin = ei->Min;
				}

				for (const auto & pr : h->H264ProfilesSupported)
					h264.Profiles.push_back(h264ProfileToString(static_cast<_onvif::H264Profile>(pr)));
			}
		}

		return options;
	}

	VideoEncoderConfigs MediaService::get_compatible_videoencoders(const std::string& token)
	{
		VideoEncoderConfigs encoders;

		using T1 = _trt__GetCompatibleVideoEncoderConfigurations;
		using T2 = _trt__GetCompatibleVideoEncoderConfigurationsResponse;

		T1 request;
		request.ProfileToken = token;
		T2 response;
		auto wrapper = [this](T1* r1, T2& r2) {return mediaProxy->GetCompatibleVideoEncoderConfigurations(r1, r2); };
		int res = GSoapRequestWrapper<T1, T2>(wrapper, &request, response, conn_info_);
		if (!res)
		{
			for (auto soapEncoder : response.Configurations)
			{
				auto encoder = soapVEncoderToEncoder(soapEncoder);
				if (encoder) encoders.push_back(encoder);
			}
		}

		return encoders;
	}

	VideoSources MediaService::get_video_sources()
	{
		//NOTE: current realisation uses only tokens

		_trt__GetVideoSources request;
		_trt__GetVideoSourcesResponse response;
		auto wrapper = [this](_trt__GetVideoSources* r1, _trt__GetVideoSourcesResponse& r2) {return mediaProxy->GetVideoSources(r1, r2); };
		int res = GSoapRequestWrapper<_trt__GetVideoSources, _trt__GetVideoSourcesResponse>(wrapper, &request, response, conn_info_);

		VideoSources video_sources;
		StringList sources_list;
		if (!res)
		{
			for (auto* vs : response.VideoSources)
			{
				if (vs)
				{
					auto video_source_sp = std::make_shared<VideoSourceConfiguration>();
					video_source_sp->source_token = vs->token;
					if(auto* res = vs->Resolution)
						video_source_sp->bounds = resolutionToString(res->Width, res->Height);

					video_sources.push_back(video_source_sp);
				}
			}
		}

		return video_sources;
	}

	VideoSources MediaService::get_compatible_videosources(const std::string& profile_token)
	{
		using T1 = _trt__GetCompatibleVideoSourceConfigurations;
		using T2 = _trt__GetCompatibleVideoSourceConfigurationsResponse;
		T1 request;
		request.ProfileToken = profile_token;
		T2 response;
		auto wrapper = [this](T1* r1, T2& r2) {return mediaProxy->GetCompatibleVideoSourceConfigurations(r1, r2); };
		int res = GSoapRequestWrapper<T1, T2>(wrapper, &request, response, conn_info_);

		VideoSources video_sources;
		if (!res)
		{
			for (const auto* vs : response.Configurations)
			{
				if (vs)
				{
					auto video_source_sp = std::make_shared<VideoSourceConfiguration>();

					video_source_sp->token = vs->token;
					video_source_sp->name = vs->Name;
					video_source_sp->source_token = vs->SourceToken;
					video_source_sp->bounds = resolutionToString(vs->Bounds->width,
						vs->Bounds->height);

					video_sources.push_back(video_source_sp);
				}
			}
		}

		return video_sources;
	}
	
	std::string MediaService::get_stream_uri(const std::string& profileToken, StreamType type, TransportProtocol transport)
	{
		std::string result_uri;

		_trt__GetStreamUri request;
		request.ProfileToken = profileToken;
		request.StreamSetup = soap_new_tt__StreamSetup(conn_info_->getSoap());
		request.StreamSetup->Stream = static_cast<tt__StreamType>(type);
		request.StreamSetup->Transport = soap_new_tt__Transport(conn_info_->getSoap());
		request.StreamSetup->Transport->Protocol = static_cast<tt__TransportProtocol>(transport);
		_trt__GetStreamUriResponse response;
		if (!mediaProxy->GetStreamUri(&request, response) && response.MediaUri)
		{
			result_uri = response.MediaUri->Uri;
		}

		return result_uri;
	}
}
