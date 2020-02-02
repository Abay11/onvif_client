#pragma once

#include "soapStub.h"
#include "wsseapi.h"
#include "httpda.h"

#include <functional>
#include <string>

namespace _onvif
{
	/***
	This class should be passed to the Services
	and contains GSoap context, credentials and device addresses
	Expected that will be used one instance for one device
	Cause if some data changed it could be easy applied to all services
	For example, if credential data were changed, there will no be problems
	***/
	enum AUTH_SCHEME
	{
		NO_AUTH = 0,
		WSSE_AUTH
	};

	class ConnectionInfo
	{
	public:
		ConnectionInfo(soap* soap,
			const std::string& address,
			int port,
			const std::string& login,
			const std::string& password)
			:soap_context_(soap),
			ip_(address),
			port_(port),
			login_(login),
			password_(password),
			auth_scheme_(NO_AUTH)
		{}

		~ConnectionInfo(){}

		~ConnectionInfo()
		{
		
		}

		void setCreds(const std::string& login, const std::string& pass)
		{
			login_ = login;
			password_ = pass;
		}

		soap* getSoap() { return soap_context_; }
		std::string getAddress() { return ip_; }
		int getPort() { return port_; }
		std::string getLogin() { return login_; }
		std::string getPass() { return password_; }
		http_da_info* get_http_da_info() { return &info; }
		AUTH_SCHEME auth_scheme() const { return auth_scheme_; }
		void set_auth_scheme(AUTH_SCHEME new_scheme) { auth_scheme_ = new_scheme; }

	private:
		soap* soap_context_;
		std::string ip_;
		short port_;
		std::string login_;
		std::string password_;
		AUTH_SCHEME auth_scheme_;
		
		//soap struct
		http_da_info info;
	};

	/***
		This method is intended to implement the logic of invocation
		GSoap classes' methods with authorization
		To use it pass to this function some GSoap method
		for convinience may be at first define lyamba and then pass it to the function.
		If returned result on client side is 401, it means that credentials are wrong.
		In this case a client should changed the credentials.
		
		Example:
		auto wrapper = [this](_tds__GetCapabilities* r1, _tds__GetCapabilitiesResponse& r2) {return deviceProxy->GetCapabilities(r1, r2); };
		GSoapRequestWrapper<_tds__GetCapabilities, _tds__GetCapabilitiesResponse>(wrapper, &request, response);

		As well may be used functional object like as returned one by std::bind:

		auto wrapReq = std::bind(static_cast<int(DeviceBindingProxy::*)(_tds__GetCapabilities*, _tds__GetCapabilitiesResponse&)>
		(&DeviceBindingProxy::GetCapabilities), *deviceProxy, _1, _2);
	***/
	template <typename R1, typename R2>
	int GSoapRequestWrapper(std::function<int(R1*, R2&)> f, R1* r1, R2& r2, ConnectionInfo* connInfo)
	{
		int res;
		switch (connInfo->auth_scheme())
		{
		case NO_AUTH:
			res = f(r1, r2);

			if (res == 401)
			{
				//try all further requests with auth
				connInfo->set_auth_scheme(WSSE_AUTH);
			}
			else
				break; //requst result not is 401, skip
		case WSSE_AUTH:
			soap_wsse_add_UsernameTokenDigest(connInfo->getSoap(), "Auth",
				connInfo->getLogin().c_str(), connInfo->getPass().c_str());
			res = f(r1, r2); //TODO: if res also 401 client should change credentials
			break;
		}

			/*
			http_da_save(connInfo->getSoap(),
				connInfo->get_http_da_info(),
				connInfo->getSoap()->authrealm,
				"aaaaa",
				connInfo->getPass().c_str());
				*/
			
			/*
			http_da_restore(connInfo->getSoap(), connInfo->get_http_da_info());
			http_da_release(connInfo->getSoap(), connInfo->get_http_da_info()); // release if auth is no longer needed
			*/

		return res;
	}
}