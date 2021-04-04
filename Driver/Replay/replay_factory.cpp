#include "replay_factory.h"

#include "SoapHelpers.h"
#include "replay_control_impl.h"

namespace _onvif
{
	std::shared_ptr<IReplayControl> ReplayFactory::ReplayControl(const std::string& replay_service_uri, ConnectionInfo* conn_info)
	{
		return std::make_shared<ReplayControlImpl>(replay_service_uri, conn_info);
	}
}
