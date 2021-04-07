#pragma once

#include <iostream>

namespace _onvif
{
	class IReplaySearch
	{
	public:
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

	private:
	};
}