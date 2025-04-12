#include "headers.h"
#include "physics/include/universal_time.h"


namespace physics
{

	UniversalTime* UniversalTime::_instance = new UniversalTime();

	double UniversalTime::current_time_ms() const
	{
		return _timer.current_time_ms();
	}

	double UniversalTime::current_time_s() const
	{
		return _timer.current_time_s();
	}

	UniversalTime& UniversalTime::instance()
	{
		return *_instance;
	}

	UniversalTime::UniversalTime()
	{
		_timer.start_time();
	}

	UniversalTime::~UniversalTime()
	{
		_timer.stop_time();
	}

}