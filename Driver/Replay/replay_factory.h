#pragma once

#include <memory>
#include <string>

namespace _onvif
{
	class ConnectionInfo;
	class IReplayControl;
	class IReplaySearch;

	class ReplayFactory
	{
	public:
		ReplayFactory(const std::shared_ptr<ConnectionInfo> connInfo);

		std::shared_ptr<IReplayControl> ReplayControl(const std::string& serviceUrl);

		std::shared_ptr<IReplaySearch> ReplaySearch(const std::string& serviceUrl);

	private:
		const std::shared_ptr<ConnectionInfo> connInfo_;
	};
	
}