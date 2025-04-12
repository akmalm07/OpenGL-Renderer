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

		double stop_time(bool debug = false);

		void start_time();

		double current_time_ms() const;

		double current_time_s() const;

		double reset(bool debug = false);

		bool get_state() const;

		double get_delta_time(bool debug);

		~Timer();

	private:

		std::chrono::high_resolution_clock::time_point _timeStarted;
		std::chrono::high_resolution_clock::time_point _timeEnded;

		bool _state;

		bool _isStopped;

	};

}

