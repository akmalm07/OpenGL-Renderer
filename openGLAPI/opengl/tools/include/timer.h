#pragma once

#include "config.h"

#include <chrono>


namespace tools
{
	class Timer
	{

	public:

		Timer();

		Timer(bool start);

		double stop_time(bool debug);

		double reset(bool debug);

		double get_delta_time(bool debug);

		~Timer();

	private:

		std::chrono::high_resolution_clock::time_point _timeStarted;
		std::chrono::high_resolution_clock::time_point _timeEnded;

		bool _isStopped;

	};

}

