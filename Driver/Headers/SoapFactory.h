#pragma once

#include <memory>

struct soap;

namespace _onvif
{
	class SoapFactory
	{
	public:
		std::shared_ptr<soap> instance();
	};
}

