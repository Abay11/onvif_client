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

	DeviceService::Capabilities* DeviceService::get_capabilities()
	{
		_tds__GetCapabilities request;
		request.Category.push_back(tt__CapabilityCategory__All);
		_tds__GetCapabilitiesResponse response;

		if (!deviceProxy.GetCapabilities(&request, response)
			&& response.Capabilities)
		{
			DeviceService::Capabilities* caps = new Capabilities;

			if (response.Capabilities->Analytics)
			{
				caps->analytics_support = true;
				caps->analytics_xaddr = response.Capabilities->Analytics->XAddr;
				caps->analytics_rule =
					response.Capabilities->Analytics->RuleSupport;
				caps->analytics_module =
					response.Capabilities->Analytics->AnalyticsModuleSupport;
			}

			if (response.Capabilities->Device)
			{
				caps->device_info_filled = true;
				caps->device_xaddr =
					response.Capabilities->Device->XAddr;

				if (response.Capabilities->Device->System)
				{
					caps->device_discovery_resolve =
						response.Capabilities->Device->System->DiscoveryResolve;

					caps->device_system_logging =
						response.Capabilities->Device->System->SystemLogging;

					for (auto v : response.Capabilities->Device->System->SupportedVersions)
					{
						if (!v) continue;

						caps->device_supported_versions.push_back
						(
							_util::onvifVersionToStr
							(
								v->Major,
								v->Minor
							)
						);
					}

				}

			}

			if (response.Capabilities->Events)
			{
				auto events = response.Capabilities->Events;

				caps->events_filled = true;
				
				caps->events_wssubstription = events->WSSubscriptionPolicySupport;

				caps->events_wspullpoint = events->WSPullPointSupport;
			}

			if (response.Capabilities->Imaging)
			{
				caps->imaging_filled = true;

				caps->imaging_xaddr = response.Capabilities->Imaging->XAddr;
			}

			if (response.Capabilities->Media)
			{
				auto media = response.Capabilities->Media;

				caps->media_filled = true;
				caps->media_xaddr = media->XAddr;

				auto scaps = media->StreamingCapabilities;
				if (scaps)
				{
					caps->media_RTPMulticast = scaps->RTPMulticast;
					caps->media_RTP_TCP = scaps->RTP_USCORETCP;
					caps->media_RTP_RTSP_TCP = scaps->RTP_USCORERTSP_USCORETCP;
				}
			}

			auto ptz = response.Capabilities->PTZ;
			if (ptz)
			{
				caps->ptz_filled = true;
				caps->ptz_xaddr = ptz->XAddr;
			}

			auto exts = response.Capabilities->Extension;
			if (exts)
			{
				auto io = exts->DeviceIO;
				if (io)
				{
					caps->io_filled = true;
					caps->io_videoSources = io->VideoSources;
					caps->io_videoOutputs = io->VideoOutputs;
					caps->io_audioSources = io->AudioSources;
					caps->io_audioOutputs = io->AudioOutputs;
				}

				auto device = response.Capabilities->Device;
				if (device)
				{
					if (device->IO)
					{
						caps->io_digitalInputs = device->IO->InputConnectors
							? *device->IO->InputConnectors : 0;
						
						caps->io_relayOutputs = device->IO->RelayOutputs
							? *device->IO->RelayOutputs : 0;
					}
					
				}
			
				auto rec = exts->Recording;
				if (rec)
				{

				}
			}

			return caps;
		}

		return nullptr;
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
