#pragma once

#include <list>
#include <string>

#include "soapStub.h"
#include "soapMediaBindingProxy.h"
#include "wsseapi.h"

#include "types.h"

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	class MediaService
	{
	public:
		MediaService(soap* soap, const char* endpoint);
		~MediaService();

	public:
		Profiles get_profiles();
		ProfileSP get_profile(const std::string& token);

	private:
		soap* soap_context;
		MediaBindingProxy mediaProxy;

		char endpoint_reference[128];
	};
}