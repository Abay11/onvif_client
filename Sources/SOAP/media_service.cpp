#include "media_service.h"

#include <list>

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

	std::list<std::string> MediaService::get_profiles()
	{
		std::list<std::string> tokens;

		_trt__GetProfiles request;
		_trt__GetProfilesResponse response;
		if (mediaProxy.GetProfiles(&request, response))
		{
			for (auto profile : response.Profiles)
			{
				if (profile)
				{
					tokens.push_back(profile->token);
				}
			}
		}

		return tokens;
	}
}
