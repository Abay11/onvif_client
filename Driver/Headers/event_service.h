#pragma once

#include <boost\asio\io_context.hpp>

#include <string>
#include <thread>
#include <memory>

struct soap;
class EventBindingProxy;
class PullPointSubscriptionBindingProxy;


namespace _onvif
{
	class ConnectionInfo;

	class EventService
	{
	public:
		EventService(std::shared_ptr<ConnectionInfo> /*connInfo*/, const std::string& /*event_service_uri*/);
		~EventService();

		//Subcribe to events
		void start();
		//Unsubscribe
		void stop();

	private:
		std::shared_ptr<ConnectionInfo> conn_info_;
		soap* soap_;
		EventBindingProxy* service_proxy_;
		PullPointSubscriptionBindingProxy* pullpoint_proxy_;

		std::string service_uri_;
		
		std::thread* worker_thread_ = nullptr;

		using io_context_t = boost::asio::io_context;
		io_context_t* io_context_ = nullptr;
		using work_t = boost::asio::io_context::work;
		work_t* io_work_ = nullptr;
	};
}