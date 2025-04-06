#include "headers.h"

#include "tools\include\timer.h"

namespace tools
{

	Timer::Timer()
	{
		_timeStarted = std::chrono::steady_clock::now();

		_isStopped = false;
	}

	Timer::Timer(bool start)
	{
		if (start)
		{
			_timeStarted = std::chrono::steady_clock::now();
		}
		else
		{
			_timeStarted = std::chrono::steady_clock::time_point();
		}

		_isStopped = false;
	}



	double Timer::stop_time(bool debug)
	{
		_isStopped = true;

		_timeEnded = std::chrono::steady_clock::now();

		std::chrono::duration duration = _timeEnded - _timeStarted;

		double milliseconds = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());//

		if (debug)
		{
			std::cout << "Timer stops at " << milliseconds << " ms \n";
		}

		return milliseconds;
	}

	double Timer::reset(bool debug)
	{
		if (_isStopped)
		{
			if (debug)
			{
				std::cout << "Timer is stopped\n";
			}
			return 0.0f;
		}
		_timeEnded = std::chrono::steady_clock::now();

		std::chrono::duration duration = _timeEnded - _timeStarted;

		_timeStarted = std::chrono::steady_clock::now();

		double milliseconds = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

		if (debug)
		{
			std::cout << milliseconds << " ms \n";
		}

		return milliseconds;
	}

	double Timer::get_delta_time(bool debug)
	{
		double time = reset(debug);
		if (time < 0.01)
		{
			time = 0.01;
		}
		return time;
	}


	Timer::~Timer()
	{
		if (!_isStopped)
		{
			stop_time(false);
		}
	}


} // namespace tools