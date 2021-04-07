#include "replay_factory.h"

#include "SoapHelpers.h"
#include "replay_control_impl.h"

namespace _onvif
{
	ReplayFactory::ReplayFactory(const std::shared_ptr<ConnectionInfo> connInfo)
		: connInfo_(connInfo)
	{
	}

	std::shared_ptr<IReplayControl> ReplayFactory::ReplayControl(const std::string& serviceUrl)
	{
		return std::make_shared<ReplayControlImpl>(serviceUrl, connInfo_);
	}
}
