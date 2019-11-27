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
			struct Analytics
			{
				std::string XAddr;
				bool rule = false;
				bool analyticsModule = false;
			};

			struct Device
			{
				//System
				std::string XAddr;
				bool discoveryResolve = false;
				bool systemLogging = false;
				std::string supportedONVIFversion;
			};

			struct Events
			{
				std::string XAddr;
				bool WSSubscription = false;
				bool WSPullpoint = false;
			};

			struct Imaging
			{
				std::string XAddr;
			};

			struct Media
			{
				std::string XAddr;

				//streaming caps
				bool RTPMulticast = false;
				bool RTP_TCP = false;
				bool RTP_RTSP_TCP = false;
			};

			struct PTZ
			{
				std::string XAddr;
			};

			struct DeviceIO
			{
				std::string XAddr;
				int videoSources = 0;
				int videoOutputs = 0;
				int audioSources = 0;
				int audioOutputs = 0;
				int digitalInputs = 0;
				int relayOutputs = 0;
			};

			struct Storage
			{
				std::string recordingXAddr;
				bool profileRecording = false;
				bool receiverRecording;

				std::string searchXAddr;
				bool metadataSearch;

				std::string replayXAddr;
			};
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