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
			_state = true;
			_timeStarted = std::chrono::steady_clock::now();
		}
		else
		{
			_state = false;
			_timeStarted = std::chrono::steady_clock::time_point();
		}

		_isStopped = false;
	}



	double Timer::stop_time(bool debug)
	{
		_state = true;
		_isStopped = true;

		_timeEnded = std::chrono::steady_clock::now();

		std::chrono::duration duration = _timeEnded - _timeStarted;

		double milliseconds = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());

		if (debug)
		{
			std::cout << "Timer stops at " << milliseconds << " ms \n";
		}

		return milliseconds;
	}

	void Timer::start_time()
	{
		_state = true;
		_isStopped = false;

		_timeStarted = std::chrono::steady_clock::now();
	}

	double Timer::current_time_ms() const 
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _timeStarted).count());
	}

	double Timer::current_time_s() const
	{
		return static_cast<double>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - _timeStarted).count());
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

	bool Timer::get_state() const
	{
		return _state;
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
		_state = false;

		if (!_isStopped)
		{
			stop_time(false);
		}
	}


} // namespace tools