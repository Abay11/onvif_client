#pragma once

#include "IReplayControl.h"


namespace _onvif
{
	class ReplayControlImpl : public IReplayControl
	{
	public:
		ReplayControlImpl(const std::string& replay_service_uri, ConnectionInfo* conn_info) 
			: IReplayControl(replay_service_uri, conn_info)
		{
		}

	private:

		// Inherited via IReplayControl
		virtual void Connect() override;
		virtual void Disconnect() override;
		virtual std::string ReplayUri() override;
	};
}