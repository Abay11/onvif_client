#include "..\Headers\device_service.h"

#include "DeviceBinding.nsmap"

#include <algorithm>
#include <sstream>

namespace _onvif
{
	DeviceService::DeviceService(soap* soap, const char* endpoint)
		:soap_context(soap),
		deviceProxy(soap_context)
	{
		strcpy(endpoint_reference, endpoint);
		deviceProxy.soap_endpoint = endpoint_reference;
	}

	DeviceService::~DeviceService()
	{
		deviceProxy.destroy();
	}

	std::list<std::string> DeviceService::get_scopes()
	{
		std::list<std::string> scopes;

		_tds__GetScopes request;
		_tds__GetScopesResponse response;

		if (!deviceProxy.GetScopes(&request, response))
		{
			for (auto it : response.Scopes)
				scopes.push_back(it->ScopeItem);
		}

		return std::move(scopes);
	}
	std::string DeviceService::get_date_time()
	{
		_tds__GetSystemDateAndTime request;
		_tds__GetSystemDateAndTimeResponse response;
		if (!deviceProxy.GetSystemDateAndTime(&request, response)
			&& response.SystemDateAndTime
			&& response.SystemDateAndTime->UTCDateTime
			&& response.SystemDateAndTime->UTCDateTime->Date
			&& response.SystemDateAndTime->UTCDateTime->Time)
		{
			std::stringstream ss;
			ss << response.SystemDateAndTime->UTCDateTime->Date->Year << "/"
				<< response.SystemDateAndTime->UTCDateTime->Date->Month << "/"
				<< response.SystemDateAndTime->UTCDateTime->Date->Day << " "
				<< response.SystemDateAndTime->UTCDateTime->Time->Hour << ":"
				<< response.SystemDateAndTime->UTCDateTime->Time->Minute << ":"
				<< response.SystemDateAndTime->UTCDateTime->Time->Second;

			return ss.str();
		}

		return std::string();
	}
}
