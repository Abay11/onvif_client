#include "media_service.h"

#include <list>

//help to copy data from gSoap type Profile
//to our type Profile
void soapProfileToProfile(const tt__Profile& gp, _onvif::Profile& p)
{
	p.Name = gp.Name;
	p.token = gp.token;
	p.fixed = gp.fixed;

	if (gp.VideoSourceConfiguration)
	{
		p.videoSource = new _onvif::VideoSourceConfiguration;
		
		p.videoSource->name = gp.VideoSourceConfiguration->Name;
		p.videoSource->token = gp.VideoSourceConfiguration->token;
		p.videoSource->useCount = gp.VideoSourceConfiguration->UseCount;
	}

	if (gp.VideoEncoderConfiguration)
	{
		p.videoEncoder = new _onvif::VideoEncoderConfiguration;

		p.videoEncoder->name = gp.VideoEncoderConfiguration->Name;
		p.videoEncoder->token = gp.VideoEncoderConfiguration->token;
		p.videoEncoder->useCount = gp.VideoEncoderConfiguration->UseCount;
		//this convertion correct only if order of the encoder the same in both types
		p.videoEncoder->encoding = static_cast<_onvif::VideoEncoding>(gp.VideoEncoderConfiguration->Encoding);
	}

	if (gp.AudioSourceConfiguration)
	{
		p.audioSource = new _onvif::AudioSourceConfiguration;

		p.audioSource->name = gp.AudioEncoderConfiguration->Name;
		p.audioSource->token = gp.AudioEncoderConfiguration->token;
		p.audioSource->useCount = gp.AudioEncoderConfiguration->UseCount;
	}
}

namespace _onvif
{
	MediaService::MediaService(soap* soap, const char* endpoint)
		:soap_context(soap),
		mediaProxy(soap_context)
	{
		strcpy(endpoint_reference, endpoint);
		mediaProxy.soap_endpoint = endpoint_reference;
	}

	MediaService::~MediaService()
	{
		mediaProxy.destroy();
	}

	Profiles MediaService::get_profiles()
	{
		Profiles profiles;

		_trt__GetProfiles request;
		_trt__GetProfilesResponse response;
		if (!mediaProxy.GetProfiles(&request, response))
		{
			for (auto gprofile : response.Profiles)
			{
				if (gprofile)
				{
					auto profile = std::make_shared<Profile>();
					soapProfileToProfile(*gprofile, *profile);
					profiles.push_back(profile);
				}
			}
		}

		return profiles;
	}
}
