#pragma once

#include "soapStub.h"
#include "soapMediaBindingProxy.h"
#include "wsseapi.h"

#include <list>
#include <string>

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	class MediaService
	{
	public:
		MediaService(soap* soap, const char* endpoint);
		~MediaService();

	public:
		std::list<std::string> get_profiles();

	private:
		soap* soap_context;
		MediaBindingProxy mediaProxy;

		char endpoint_reference[128];
	};
}