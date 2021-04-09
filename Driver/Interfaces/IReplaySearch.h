#pragma once

#include <iostream>

namespace _onvif
{
	class ConnectionInfo;
	
	class IReplaySearch
	{
	public:
		IReplaySearch(const std::string& url, std::shared_ptr<ConnectionInfo> connInfo)
			:
			url_(url)
			,connInfo_(connInfo)
		{
		}

		virtual ~IReplaySearch() {};

		// result in JSON
		/*
		[
			{
				"RecordingToken":"value",
				"Track":
				[
					{
					"TrackToken":"value",
					"TrackType":"value",
					"DataFrom":"value",
					"DataTo":"value"
					},
					...
				],
				"RecordingStatus":"value"
			},
			...
		]
		*/
		virtual std::string Recordings() const = 0;
		
		
		/*
		{
			DataFrom:,
			DataUntil:,
			NumberRecordings:
		}
		*/
		virtual std::string RecordingSummary() const = 0;

	protected:
		const std::string url_;
		std::shared_ptr<ConnectionInfo> connInfo_;
	};
}