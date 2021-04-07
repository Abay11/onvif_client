#include "event_service.h"
#include "SoapHelpers.h"

#include "soapStub.h"
#include "soapEventBindingProxy.h"
#include "soapPullPointSubscriptionBindingProxy.h"

#include <iostream>
#include <string>

#include <boost\asio\io_context.hpp>

#define log(line) std::cout << line << std::endl

namespace _onvif
{
	EventService::EventService(std::shared_ptr<ConnectionInfo> connInfo, const std::string& service_uri)
		:
		conn_info_(connInfo),
		soap_(soap_new()),
		service_proxy_(new EventBindingProxy(soap_)),
		pullpoint_proxy_(new PullPointSubscriptionBindingProxy(soap_)),
		service_uri_(service_uri),
		io_context_(new io_context_t{}),
		io_work_(new work_t(*io_context_))
	{
		service_proxy_->soap_endpoint = service_uri_.c_str();
		pullpoint_proxy_->soap_endpoint = service_uri_.c_str();
	}

	template <typename Pointer>
	void safe_delete(Pointer* p)
	{
		if (p)
		{
			delete p;
			p = nullptr;
		}
	}

	EventService::~EventService()
	{
	
		std::cout << "~EventService" << std::endl;
		try {
			worker_thread_->join();
			safe_delete(worker_thread_);
		}catch(std::exception){}

		safe_delete(io_context_);
		//delete service_proxy_;
		//delete pullpoint_proxy_;
	}

	void EventService::start()
	{
		log("Posting event pulling..");

		io_context_->post([this]() {
			log("subscribe to events");
			_tev__CreatePullPointSubscription request;
			_tev__CreatePullPointSubscriptionResponse response;

			auto res = service_proxy_->CreatePullPointSubscription(&request, response);
			if (res != SOAP_OK)
			{
				throw std::runtime_error(std::string("CreatePullPointSubscription: ") + conn_info_->getSoap()->fault->faultstring);
			}

			log("PullPoint Subscription created. Termination time: " + std::to_string(response.wsnt__TerminationTime));

			log("Pulling messages");
			_tev__PullMessages pull_request;
			pull_request.Timeout = "PT60S";
			pull_request.MessageLimit = 30;

			_tev__PullMessagesResponse pull_response;
			res = pullpoint_proxy_->PullMessages(&pull_request, pull_response);
			if (res != SOAP_OK)
			{
				log("PullMessages not OK");
				return;
			}

			log("PullMessage finished successfully");

		});

		if (!worker_thread_)
		{
			worker_thread_ = new std::thread([this]() {
				io_context_->run();
				});

			log("Worker thread is running..");
		}
		else
			log("Worker thread is already running..");

	}

	void EventService::stop()
	{
		log("unsubscribing events");

		//soap_closesock(soap_);
		//safe_delete(pullpoint_proxy_);
		pullpoint_proxy_->destroy();
		//soap_destroy(soap_);
		//soap_end(soap_);

		try
		{
			if (io_work_)
			{
				safe_delete(io_work_);
			}
		}
		catch (const  std::exception&)
		{
		}

		log("unsubscribed events");
	}
}