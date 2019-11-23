#include <iostream>

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "DeviceBinding.nsmap"

int main()
{
	soap* soap = soap_new();

	DeviceBindingProxy device(soap);
	device.soap_endpoint = ("http://192.168.43.196:8000/onvif/device_service"); //testing on ONVIF HAPPY TIME SERVER

	_tds__GetScopes request;
	_tds__GetScopesResponse response;
	if (!device.GetScopes(&request, response))
	{
		std::cout << "Received scopes:\n";
		for (auto it = response.Scopes.begin(); it != response.Scopes.end(); ++it)
			std::cout << (*it)->ScopeItem << "\n";
		
		std::cout << "\n";
	}
	else
		std::cout << "Received an error" << std::endl;

	device.destroy();

	//cleanup
	soap_destroy(soap);
	soap_end(soap);
	soap_free(soap);

	return 0;
}