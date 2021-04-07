#pragma once

#include <string>
#include <memory>

namespace _onvif
{
	class ConnectionInfo;

	class IReplayControl
	{
	public:
		IReplayControl(const std::string& serviceUrl,
			const std::shared_ptr<ConnectionInfo> connInfo)
			: replay_service_uri_(serviceUrl)
			, isConnected_(false)
			, connInfo_(connInfo)
		{}

		virtual ~IReplayControl() {}

		virtual void Connect() = 0;
		virtual void Disconnect() = 0;

		bool IsConnected() { return isConnected_; }

		virtual std::string ReplayUri() = 0;

	protected:
		const std::string replay_service_uri_;
		bool isConnected_;
		const std::shared_ptr<ConnectionInfo> connInfo_;
	};

	using IReplayControlSP = std::shared_ptr<IReplayControl>;
}