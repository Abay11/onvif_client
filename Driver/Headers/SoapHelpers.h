#pragma once

#include "soapStub.h"
#include "wsseapi.h"
#include "httpda.h"

#include <functional>
#include <string>
#include <memory>

namespace _onvif
{
	class ConnectionInfo;

	class SoapContextHolder
	{
	public:
		SoapContextHolder();

		virtual ~SoapContextHolder()
		{
			delete soap_;
		}

	protected:
		void freeSoapUsedMemory() const;

	protected:
		soap* soap_;
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
	int GSoapRequestWrapper(std::function<int(R1*, R2&)> f, R1* r1, R2& r2, std::shared_ptr<ConnectionInfo> connInfo)
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
			soap_wsse_add_UsernameTokenDigest(connInfo->getSoapRaw(), "Auth",
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