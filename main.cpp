#include <iostream>

#include "device_service.h"

int main()
{
	soap* soap = soap_new();

	soap_register_plugin(soap, soap_wsse);

	soap_wsse_add_UsernameTokenDigest(soap, "Auth", "admin", "admin");

	const char* HAPPY_TIME_SERVER = "http://192.168.43.196:8000/onvif/device_service";

	_onvif::DeviceService device_s(soap, HAPPY_TIME_SERVER);

	auto info = device_s.get_device_info();

	//

	//cleanup
	soap_destroy(soap);
	soap_end(soap);
	soap_free(soap);

	return 0;
}