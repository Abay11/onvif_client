#include "replay_search_impl.h"

namespace _onvif
{
	ReplaySearchImpl::ReplaySearchImpl(const std::string& url, std::shared_ptr<ConnectionInfo> connInfo)
		: replayServiceUrl_(url)
		, conn_info_(connInfo)
	{
	}

	std::string ReplaySearchImpl::Recordings() const
	{
		return std::string();
	}
}
