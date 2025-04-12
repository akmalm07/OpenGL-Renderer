#pragma once


#include "config.h"

#include "tools/include/timer.h"

namespace physics
{

	class UniversalTime
	{
	public:
		double current_time_ms() const;

		double current_time_s() const;

		static UniversalTime& instance();

	private:

		UniversalTime();

		~UniversalTime();

		static UniversalTime* _instance;

		tools::Timer _timer;
	};

}