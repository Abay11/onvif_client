#include "..\Headers\device_service.h"

#include "DeviceBinding.nsmap"

#include <algorithm>

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
}
