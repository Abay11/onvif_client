/* soapSubscriptionManagerBindingProxy.h
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

#ifndef soapSubscriptionManagerBindingProxy_H
#define soapSubscriptionManagerBindingProxy_H
#include "soapH.h"

    class SOAP_CMAC SubscriptionManagerBindingProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'SubscriptionManagerBindingProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in .\onvif.h, if any
        /// Construct a proxy with new managing context
        SubscriptionManagerBindingProxy();
        /// Copy constructor
        SubscriptionManagerBindingProxy(const SubscriptionManagerBindingProxy& rhs);
        /// Construct proxy given a shared managing context
        SubscriptionManagerBindingProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        SubscriptionManagerBindingProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        SubscriptionManagerBindingProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        SubscriptionManagerBindingProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        SubscriptionManagerBindingProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        SubscriptionManagerBindingProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~SubscriptionManagerBindingProxy();
        /// Initializer used by constructors
        virtual void SubscriptionManagerBindingProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual SubscriptionManagerBindingProxy *copy();
        /// Copy assignment
        SubscriptionManagerBindingProxy& operator=(const SubscriptionManagerBindingProxy&);
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
        /// Web service synchronous operation 'Renew' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int Renew(_wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse) { return this->Renew(NULL, NULL, wsnt__Renew, wsnt__RenewResponse); }
        /// Web service synchronous operation 'Renew' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int Renew(const char *soap_endpoint_url, const char *soap_action, _wsnt__Renew *wsnt__Renew, _wsnt__RenewResponse &wsnt__RenewResponse) { return this->send_Renew(soap_endpoint_url, soap_action, wsnt__Renew) || this->recv_Renew(wsnt__RenewResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_Renew' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_Renew(const char *soap_endpoint_url, const char *soap_action, _wsnt__Renew *wsnt__Renew);
        /// Web service asynchronous operation 'recv_Renew' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_Renew(_wsnt__RenewResponse &wsnt__RenewResponse);
        //
        /// Web service synchronous operation 'Unsubscribe' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int Unsubscribe(_wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse) { return this->Unsubscribe(NULL, NULL, wsnt__Unsubscribe, wsnt__UnsubscribeResponse); }
        /// Web service synchronous operation 'Unsubscribe' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int Unsubscribe(const char *soap_endpoint_url, const char *soap_action, _wsnt__Unsubscribe *wsnt__Unsubscribe, _wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse) { return this->send_Unsubscribe(soap_endpoint_url, soap_action, wsnt__Unsubscribe) || this->recv_Unsubscribe(wsnt__UnsubscribeResponse) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_Unsubscribe' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_Unsubscribe(const char *soap_endpoint_url, const char *soap_action, _wsnt__Unsubscribe *wsnt__Unsubscribe);
        /// Web service asynchronous operation 'recv_Unsubscribe' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_Unsubscribe(_wsnt__UnsubscribeResponse &wsnt__UnsubscribeResponse);
    };
#endif
