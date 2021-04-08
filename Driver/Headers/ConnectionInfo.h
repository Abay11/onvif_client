#pragma once

#include <memory>
#include <string>

struct soap;

	/***
	This class should be passed to the Services
	and contains GSoap context, credentials and device addresses
	Expected that will be used one instance for one device
	Cause if some data changed it could be easy applied to all services
	For example, if credential data were changed, there will no be problems
	***/

namespace _onvif
{
	enum AUTH_SCHEME
	{
		NO_AUTH = 0,
		WSSE_AUTH
	};

	class ConnectionInfo
	{
	public:
		ConnectionInfo(std::shared_ptr<soap> soap,
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
		{
		}

		void setCreds(const std::string& login, const std::string& pass)
		{
			login_ = login;
			password_ = pass;
		}

		std::shared_ptr<soap> getSoap()
		{
			return soap_context_;
		}
		
		// be carefull, do not manipulate with the returned pointer
		soap* getSoapRaw()
		{
			return soap_context_.get();
		}

		std::string getAddress()
		{
			return ip_;
		}

		int getPort()
		{
			return port_;
		}

		std::string getLogin()
		{
			return login_;
		}

		std::string getPass()
		{
			return password_;
		}
		//http_da_info* get_http_da_info()
		//{
		//	return &info;
		//}
		AUTH_SCHEME auth_scheme() const
		{
			return auth_scheme_;
		}
		void set_auth_scheme(AUTH_SCHEME new_scheme)
		{
			auth_scheme_ = new_scheme;
		}

	private:
		std::shared_ptr<soap> soap_context_;
		std::string ip_;
		short port_;
		std::string login_;
		std::string password_;
		AUTH_SCHEME auth_scheme_;

		//http_da_info info;
	};
}
