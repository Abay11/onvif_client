#include "replay_search_impl.h"

#include "ConnectionInfo.h"
#include "SoapHelpers.h"

#include "soapSearchBindingProxy.h"

namespace _onvif
{
	ReplaySearchImpl::ReplaySearchImpl(const std::string& url, std::shared_ptr<ConnectionInfo> connInfo)
		: IReplaySearch(url, connInfo)
	{
		searchProxy_ = std::shared_ptr<SearchBindingProxy>(new SearchBindingProxy(soap_),
			[](SearchBindingProxy* searchProxy)
			{
				searchProxy->destroy();
			});

		searchProxy_->soap_endpoint = url_.c_str();
	}

	std::string ReplaySearchImpl::Recordings() const
	{
		//searchProxy_->GetRe
		return {};
	}

	std::string ReplaySearchImpl::RecordingSummary() const
	{
		_tse__GetRecordingSummary T1;
		_tse__GetRecordingSummaryResponse T2;
		auto r = searchProxy_->GetRecordingSummary(&T1, T2);
		
		if (r != SOAP_OK)
			throw std::runtime_error("GetRecordingSummary");

		freeSoapUsedMemory();

		return std::string();
	}
}
