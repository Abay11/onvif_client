#pragma once

#include <string>
#include <memory>

namespace _onvif
{
	class ConnectionInfo;

	class IReplayControl
	{
	public:
		IReplayControl(const std::string& replay_service_uri, ConnectionInfo* conn_info)
			: replay_service_uri_(replay_service_uri)
			, isConnected_(false)
			, conn_info_(conn_info)
		{}

		virtual ~IReplayControl() {}

		virtual void Connect() = 0;
		virtual void Disconnect() = 0;

		bool IsConnected() { return isConnected_; }

		virtual std::string ReplayUri() = 0;

	protected:
		const std::string replay_service_uri_;
		bool isConnected_;
		ConnectionInfo* conn_info_ = nullptr;
	};

	using IReplayControlSP = std::shared_ptr<IReplayControl>;
}