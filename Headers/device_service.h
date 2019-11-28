#pragma once

#include <list>

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"

extern SOAP_NMAC struct Namespace namespaces[];

namespace _onvif
{
	class DeviceService
	{
	public:
		/**
		A soap context should be valid until a class object will destroy
		**/
		DeviceService(soap* soap, const char* endpoint);
		~DeviceService();

		struct Capabilities;
		Capabilities* get_capabilities();

		std::string get_date_time();
		struct DeviceInformation;
		DeviceInformation get_device_info();

		struct Service;
		using Services = std::vector<Service>;
		Services get_service_addresses();

		std::list<std::string> get_scopes();

	public:
		struct Capabilities
		{

			bool analytics_support = false;
			std::string analytics_xaddr;
			bool analytics_rule = false;
			bool analytics_module = false;

			//System
			bool device_info_filled = false;
			std::string device_xaddr;
			bool device_discovery_resolve = false;
			bool device_system_logging = false;
			std::vector<std::string> device_supported_versions;

			//EVENTS
			bool events_filled = false;
			bool events_wssubstription = false;
			bool events_wspullpoint = false;

			//IMAGING
			bool imaging_filled = false;
			std::string imaging_xaddr;

			//MEDIA
			bool media_filled = false;
			std::string media_xaddr;
			bool media_RTPMulticast = false;
			bool media_RTP_TCP = false;
			bool media_RTP_RTSP_TCP = false;

			//PTZ
			bool ptz_filled;
			std::string ptz_xaddr;

			//IO
			bool io_filled = false;
			std::string io_xaddr;
			int io_videoSources = 0;
			int io_videoOutputs = 0;
			int io_audioSources = 0;
			int io_audioOutputs = 0;
			int io_digitalInputs = 0;
			int io_relayOutputs = 0;

			//Storage
			bool storage_filled = true;
			std::string storage_recording_xaddr;
			bool storage_profile_recording = false;
			bool storage_receiverRecording = false;
			std::string storage_search_xaddr;
			bool storage_metadata_search = false;
			std::string storage_replay_xaddr;
		};

		struct DeviceInformation
		{
			std::string manufacturer;
			std::string model;
			std::string firmwareVersion;
			std::string serialNumber;
			std::string hardwareId;

			bool filled = false;
		};

		struct Service
		{
			std::string ns;
			std::string xaddr;

			//TODO: Filling version not implemented yet
			std::string version;

			bool filled = false;
		};

	private:
		soap* soap_context;
		DeviceBindingProxy deviceProxy;

		char endpoint_reference[128];
	};
}