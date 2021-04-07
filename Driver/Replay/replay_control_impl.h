#pragma once

#include "IReplayControl.h"


namespace _onvif
{
	class ReplayControlImpl : public IReplayControl
	{
	public:
		ReplayControlImpl(const std::string& serviceUrl,
			const std::shared_ptr<ConnectionInfo> connInfo) 
			: IReplayControl(serviceUrl, connInfo)
		{
		}

		// Inherited via IReplayControl
		virtual void Connect() override;
		virtual void Disconnect() override;
		virtual std::string ReplayUri() override;
	};
}