#include "..\Headers\device_service.h"
#include "util.h"

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

	DeviceService::DeviceInformation DeviceService::get_device_info()
	{
		DeviceInformation info;

		_tds__GetDeviceInformation request;
		_tds__GetDeviceInformationResponse response;
		if (!deviceProxy.GetDeviceInformation(&request, response))
		{
			info.filled = true;
			
			info.firmwareVersion = response.FirmwareVersion;
			info.hardwareId = response.HardwareId;
			info.manufacturer = response.Manufacturer;
			info.model = response.Model;
			info.serialNumber = response.SerialNumber;
		}

		return std::move(info);
	}

	DeviceService::Services DeviceService::get_service_addresses()
	{
		Services services;
		
		_tds__GetServices request;
		request.IncludeCapability = false;
		_tds__GetServicesResponse response;
		if (!deviceProxy.GetServices(&request, response))
		{
			for (auto s : response.Service)
			{
				if (s)
				{
					Service service;
					service.filled = true;
					service.ns = s->Namespace;
					service.xaddr = s->XAddr;
					service.version = _util::onvifVersionToStr(s->Version->Major, s->Version->Minor);
					services.push_back(service);
				}
			}
		}

		return std::move(services);
	}
}
