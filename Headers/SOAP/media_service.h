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
		MediaService(soap* soap, const std::string& service_addr);
		~MediaService();

	public:
		Profiles get_profiles();
		ProfileSP get_profile(const std::string& token);
		std::string get_stream_uri(const std::string& profileToken, StreamType type = Unicast, TransportProtocol transport = UDP);

	private:
		soap* soap_context;
		MediaBindingProxy mediaProxy;

		std::string service_addr_;
	};
}