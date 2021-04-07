#pragma once

#include <memory>

struct soap;

class SoapFactory
{
public:
	std::shared_ptr<soap> instance();
};

