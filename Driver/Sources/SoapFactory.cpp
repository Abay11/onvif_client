#include "..\Headers\SoapFactory.h"

#include "soapStub.h"
#include "wsseapi.h"
#include "httpda.h"

std::shared_ptr<soap> SoapFactory::instance()
{
	return std::shared_ptr<soap>();
		{
			soap* soapCtx = soap_new();
			soap_register_plugin(soapCtx, soap_wsse);
			soap_register_plugin(soapCtx, http_da);

			return std::shared_ptr<soap>(soapCtx,
				[](soap* s)
				{
					soap_destroy(s);
					soap_end(s);
					soap_free(s);
				});
		}
}
