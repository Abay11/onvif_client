#pragma once

#include <memory>
#include <string>

namespace _onvif
{
	class IReplayControl;
	class ConnectionInfo;

	class ReplayFactory
	{
	public:
		std::shared_ptr<IReplayControl> ReplayControl(const std::string& replay_service_uri, ConnectionInfo* conn_info);
	};
	
}