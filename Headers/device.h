#pragma once

#include "IDevice.h"

#include <string>

namespace _onvif
{
	class Device : public IDevice
	{
	public:
		Device(const std::string& endpoint, short port);

		//IDevice
		virtual void Init();

		virtual void StartLive();

		virtual void StopLive();

	private:
		std::string endpoint_;
		short port_;
	};

}