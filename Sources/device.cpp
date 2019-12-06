#include "..\Headers\device.h"

#include <iostream>

namespace _onvif
{
	Device::Device(const std::string& endpoint, short port)
		: endpoint_(endpoint),
		port_(port)
	{

	}

	void Device::Init()
	{
		std::cout << "Init" << std::endl;
	}

	void Device::StartLive()
	{
		std::cout << "Start" << std::endl;
	}

	void Device::StopLive()
	{
		std::cout << "Stop" << std::endl;
	}
}
