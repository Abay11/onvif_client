#include "..\Headers\device_service.h"

#include "ConnectionInfo.h"
#include "SoapHelpers.h"
#include "util.h"

#include "DeviceBinding.nsmap"
#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "wsseapi.h"

#include <algorithm>
#include <sstream>

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	DeviceService::DeviceService(std::shared_ptr<ConnectionInfo> connInfo, const std::string& device_service_uri)
		:conn_info_(connInfo),
		deviceProxy(new DeviceBindingProxy(conn_info_->getSoapRaw())),
		device_service_uri_(device_service_uri)
	{
		
		deviceProxy->soap_endpoint = device_service_uri_.c_str();
	}

	DeviceService::~DeviceService()
	{
		deviceProxy->destroy();
	}

	CapabilitiesSP DeviceService::get_capabilities()
	{
		CapabilitiesSP caps;

		_tds__GetCapabilities request;
		request.Category.push_back(tt__CapabilityCategory__All);
		_tds__GetCapabilitiesResponse response;

		auto res = deviceProxy->GetCapabilities(&request, response);

		if (res != SOAP_OK)
			throw std::runtime_error("GetCapabilities failed");

		if (response.Capabilities)
		{
			caps = std::make_shared<Capabilities>();

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
					caps->io_xaddr = io->XAddr;
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
					caps->recording_filled = true;
					caps->recording_xaddr = rec->XAddr;
					caps->recording_from_profile = rec->MediaProfileSource;
					caps->recording_from_receiver = rec->ReceiverSource;
				}

				auto search = exts->Search;
				if (search)
				{
					caps->search_filled = true;
					caps->search_xaddr = search->XAddr;
					caps->search_metadata = search->MetadataSearch;
				}

				auto replay = exts->Replay;
				if (replay)
				{
					caps->replay_filled = true;
					caps->replay_xaddr = replay->XAddr;
				}
			} //response.Capabilities->Extension
		}

		return caps;
	}

	std::list<std::string> DeviceService::get_scopes()
	{
		std::list<std::string> scopes;

		_tds__GetScopes request;
		_tds__GetScopesResponse response;

		if (!deviceProxy->GetScopes(&request, response))
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
		if (!deviceProxy->GetSystemDateAndTime(&request, response)
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

	DeviceInformationSP DeviceService::get_device_info()
	{
		DeviceInformationSP info = std::make_shared<DeviceInformation>();

		_tds__GetDeviceInformation request;
		_tds__GetDeviceInformationResponse response;

		auto wrapper = [this](_tds__GetDeviceInformation* r1, _tds__GetDeviceInformationResponse& r2) {return deviceProxy->GetDeviceInformation(r1, r2); };
		int res = GSoapRequestWrapper<_tds__GetDeviceInformation, _tds__GetDeviceInformationResponse>(wrapper, &request, response, conn_info_);

		if (!res)
		{
			info->filled = true;
			
			info->firmwareVersion = response.FirmwareVersion;
			info->hardwareId = response.HardwareId;
			info->manufacturer = response.Manufacturer;
			info->model = response.Model;
			info->serialNumber = response.SerialNumber;
		}

		return info;
	}

	Services DeviceService::get_service_addresses()
	{
		Services services;
		
		_tds__GetServices request;
		request.IncludeCapability = false;
		_tds__GetServicesResponse response;
		if (!deviceProxy->GetServices(&request, response))
		{
			for (auto s : response.Service)
			{
				if (s)
				{
					ServiceSP service = std::make_shared<Service>();
					service->ns = s->Namespace;
					service->xaddr = s->XAddr;
					service->version = _util::onvifVersionToStr(s->Version->Major, s->Version->Minor);
					services.push_back(service);
				}
			}
		}

		return std::move(services);
	}	

	std::string get_service_address(const Services* services, SERVICES service)
	{
		const char* DEVICE_SERVICE_NS = "http://www.onvif.org/ver10/device/wsdl";
		const char* MEDIA_SERVICE_NS = "http://www.onvif.org/ver10/media/wsdl";
		const char* MEDIA2_SERVICE_NS = "http://www.onvif.org/ver20/media/wsdl";
		const char* EVENTS_SERVICE_NS = "http://www.onvif.org/ver10/events/wsdl";
		const char* PTZ_SERVICE_NS = "http://www.onvif.org/ver20/ptz/wsdl";
		const char* IMAGING_SERVICE_NS = "http://www.onvif.org/ver20/imaging/wsdl";
		const char* ANALYTICS_SERVICE_NS = "http://www.onvif.org/ver20/analytics/wsdl";
		const char* RECORDING_SERVICE_NS = "http://www.onvif.org/ver10/recording/wsdl";
		const char* SEARCH_SERVICE_NS = "http://www.onvif.org/ver10/search/wsdl";
		const char* REPLAY_SERVICE_NS = "http://www.onvif.org/ver10/replay/wsdl";
		const char* DEVICEIO_SERVICE_NS = "http://www.onvif.org/ver10/deviceIO/wsdl";
		
		//TODO: add converting other services
		//here we search for passed service string representation
		const char* requested_service_ns;
		switch (service)
		{
		case SERVICES::MEDIA_SERVICE:
			requested_service_ns = MEDIA_SERVICE_NS;
			break;
		case SERVICES::EVENTS_SERVICE:
			requested_service_ns = EVENTS_SERVICE_NS;
			break;
		//case SERVICES::others...
		}

		for (const auto& s : *services)
		{
			if (s->ns == requested_service_ns)
				return s->xaddr;
		}

		return std::string();
	}
}
