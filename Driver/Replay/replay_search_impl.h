#pragma once

#include "IReplaySearch.h"

#include "SoapHelpers.h"

#include <string>
#include <memory>

class SearchBindingProxy;

namespace _onvif
{
	class ConnectionInfo;

	class ReplaySearchImpl : public SoapContextHolder, public IReplaySearch
	{
	public:
		ReplaySearchImpl(const std::string& url, std::shared_ptr<ConnectionInfo> connInfo);

		// Inherited via IReplaySearch
		virtual std::string Recordings() const override;
		
		// Inherited via IReplaySearch
		virtual std::string RecordingSummary() const override;

	private:
		std::shared_ptr<SearchBindingProxy> searchProxy_;
	};
}