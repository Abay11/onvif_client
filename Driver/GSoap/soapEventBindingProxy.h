/* soapEventBindingProxy.h
   Generated by gSOAP 2.8.95 for .\onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapEventBindingProxy_H
#define soapEventBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC EventBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'EventBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in .\onvif.h, if any
        /// Construct a proxy with new managing context
        EventBindingProxy();
        /// Copy constructor
        EventBindingProxy(const EventBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        EventBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        EventBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        EventBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        EventBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        EventBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        EventBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~EventBindingProxy();
        /// Initializer used by constructors
        virtual void EventBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual EventBindingProxy *copy();
        /// Copy assignment
        EventBindingProxy& operator=(const EventBindingProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Add SOAP Header to message
        virtual void soap_header(char *wsa5__MessageID, struct wsa5__RelatesToType *wsa5__RelatesTo, struct wsa5__EndpointReferenceType *wsa5__From, struct wsa5__EndpointReferenceType *wsa5__ReplyTo, struct wsa5__EndpointReferenceType *wsa5__FaultTo, char *wsa5__To, char *wsa5__Action, struct chan__ChannelInstanceType *chan__ChannelInstance, struct wsdd__AppSequenceType *wsdd__AppSequence, struct _wsse__Security *wsse__Security);
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'GetServiceCapabilities' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(_tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse &tev__GetServiceCapabilitiesResponse) { return this->GetServiceCapabilities(NULL, NULL, tev__GetServiceCapabilities, tev__GetServiceCapabilitiesResponse); }
        /// Web service synchronous operation 'GetServiceCapabilities' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tev__GetServiceCapabilities *tev__GetServiceCapabilities, _tev__GetServiceCapabilitiesResponse &tev__GetServiceCapabilitiesResponse) { return this->send_GetServiceCapabilities(soap_endpoint_url, soap_action, tev__GetServiceCapabilities) || this->recv_GetServiceCapabilities(tev__GetServiceCapabilitiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetServiceCapabilities' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetServiceCapabilities(const char *soap_endpoint_url, const char *soap_action, _tev__GetServiceCapabilities *tev__GetServiceCapabilities);
        /// Web service asynchronous operation 'recv_GetServiceCapabilities' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetServiceCapabilities(_tev__GetServiceCapabilitiesResponse &tev__GetServiceCapabilitiesResponse);
        //
        /// Web service synchronous operation 'CreatePullPointSubscription' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int CreatePullPointSubscription(_tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse &tev__CreatePullPointSubscriptionResponse) { return this->CreatePullPointSubscription(NULL, NULL, tev__CreatePullPointSubscription, tev__CreatePullPointSubscriptionResponse); }
        /// Web service synchronous operation 'CreatePullPointSubscription' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int CreatePullPointSubscription(const char *soap_endpoint_url, const char *soap_action, _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, _tev__CreatePullPointSubscriptionResponse &tev__CreatePullPointSubscriptionResponse) { return this->send_CreatePullPointSubscription(soap_endpoint_url, soap_action, tev__CreatePullPointSubscription) || this->recv_CreatePullPointSubscription(tev__CreatePullPointSubscriptionResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_CreatePullPointSubscription' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_CreatePullPointSubscription(const char *soap_endpoint_url, const char *soap_action, _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription);
        /// Web service asynchronous operation 'recv_CreatePullPointSubscription' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_CreatePullPointSubscription(_tev__CreatePullPointSubscriptionResponse &tev__CreatePullPointSubscriptionResponse);
        //
        /// Web service synchronous operation 'GetEventProperties' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventProperties(_tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse &tev__GetEventPropertiesResponse) { return this->GetEventProperties(NULL, NULL, tev__GetEventProperties, tev__GetEventPropertiesResponse); }
        /// Web service synchronous operation 'GetEventProperties' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventProperties(const char *soap_endpoint_url, const char *soap_action, _tev__GetEventProperties *tev__GetEventProperties, _tev__GetEventPropertiesResponse &tev__GetEventPropertiesResponse) { return this->send_GetEventProperties(soap_endpoint_url, soap_action, tev__GetEventProperties) || this->recv_GetEventProperties(tev__GetEventPropertiesResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetEventProperties' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetEventProperties(const char *soap_endpoint_url, const char *soap_action, _tev__GetEventProperties *tev__GetEventProperties);
        /// Web service asynchronous operation 'recv_GetEventProperties' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetEventProperties(_tev__GetEventPropertiesResponse &tev__GetEventPropertiesResponse);
        //
        /// Web service synchronous operation 'AddEventBroker' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int AddEventBroker(_tev__AddEventBroker *tev__AddEventBroker, _tev__AddEventBrokerResponse &tev__AddEventBrokerResponse) { return this->AddEventBroker(NULL, NULL, tev__AddEventBroker, tev__AddEventBrokerResponse); }
        /// Web service synchronous operation 'AddEventBroker' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int AddEventBroker(const char *soap_endpoint_url, const char *soap_action, _tev__AddEventBroker *tev__AddEventBroker, _tev__AddEventBrokerResponse &tev__AddEventBrokerResponse) { return this->send_AddEventBroker(soap_endpoint_url, soap_action, tev__AddEventBroker) || this->recv_AddEventBroker(tev__AddEventBrokerResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_AddEventBroker' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_AddEventBroker(const char *soap_endpoint_url, const char *soap_action, _tev__AddEventBroker *tev__AddEventBroker);
        /// Web service asynchronous operation 'recv_AddEventBroker' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_AddEventBroker(_tev__AddEventBrokerResponse &tev__AddEventBrokerResponse);
        //
        /// Web service synchronous operation 'DeleteEventBroker' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteEventBroker(_tev__DeleteEventBroker *tev__DeleteEventBroker, _tev__DeleteEventBrokerResponse &tev__DeleteEventBrokerResponse) { return this->DeleteEventBroker(NULL, NULL, tev__DeleteEventBroker, tev__DeleteEventBrokerResponse); }
        /// Web service synchronous operation 'DeleteEventBroker' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int DeleteEventBroker(const char *soap_endpoint_url, const char *soap_action, _tev__DeleteEventBroker *tev__DeleteEventBroker, _tev__DeleteEventBrokerResponse &tev__DeleteEventBrokerResponse) { return this->send_DeleteEventBroker(soap_endpoint_url, soap_action, tev__DeleteEventBroker) || this->recv_DeleteEventBroker(tev__DeleteEventBrokerResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_DeleteEventBroker' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_DeleteEventBroker(const char *soap_endpoint_url, const char *soap_action, _tev__DeleteEventBroker *tev__DeleteEventBroker);
        /// Web service asynchronous operation 'recv_DeleteEventBroker' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_DeleteEventBroker(_tev__DeleteEventBrokerResponse &tev__DeleteEventBrokerResponse);
        //
        /// Web service synchronous operation 'GetEventBrokers' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventBrokers(_tev__GetEventBrokers *tev__GetEventBrokers, _tev__GetEventBrokersResponse &tev__GetEventBrokersResponse) { return this->GetEventBrokers(NULL, NULL, tev__GetEventBrokers, tev__GetEventBrokersResponse); }
        /// Web service synchronous operation 'GetEventBrokers' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetEventBrokers(const char *soap_endpoint_url, const char *soap_action, _tev__GetEventBrokers *tev__GetEventBrokers, _tev__GetEventBrokersResponse &tev__GetEventBrokersResponse) { return this->send_GetEventBrokers(soap_endpoint_url, soap_action, tev__GetEventBrokers) || this->recv_GetEventBrokers(tev__GetEventBrokersResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetEventBrokers' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetEventBrokers(const char *soap_endpoint_url, const char *soap_action, _tev__GetEventBrokers *tev__GetEventBrokers);
        /// Web service asynchronous operation 'recv_GetEventBrokers' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetEventBrokers(_tev__GetEventBrokersResponse &tev__GetEventBrokersResponse);
    };
#endif
