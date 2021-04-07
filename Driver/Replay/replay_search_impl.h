#pragma once

#include "IReplaySearch.h"

#include <string>
#include <memory>

class ILogger;

namespace _onvif
{

	class ConnectionInfo;

	class ReplaySearchImpl : public IReplaySearch
	{
		ReplaySearchImpl(const std::string& url, std::shared_ptr<ConnectionInfo> connInfo);

		// Inherited via IReplaySearch
		virtual std::string Recordings() const override;

	private:
		const std::string replayServiceUrl_;
		std::shared_ptr<ConnectionInfo> conn_info_;
	};
}