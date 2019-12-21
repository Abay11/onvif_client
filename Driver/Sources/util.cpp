#include "util.h"

std::string _util::onvifVersionToStr(int major, int minor)
{
	//TODO: correct function
	std::string result;

	result += std::to_string(major) + "." + std::to_string(minor);

	return result;
}
