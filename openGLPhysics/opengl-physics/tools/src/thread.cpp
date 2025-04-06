#include "headers.h"

#include "tools\include\thread.h"



namespace tools
{

	void ConditionalVariuble::wait_for_condition(std::mutex& mutex)
	{
		std::unique_lock<std::mutex> lock(mutex);
		_condition.wait(lock);
	}

	void ConditionalVariuble::wait_for_condition(std::mutex& mutex, bool& value)
	{
		std::unique_lock<std::mutex> lock(mutex);
		_condition.wait(lock, [&value] { return value; });
	}

	void ConditionalVariuble::notify_one()
	{
		_condition.notify_one();
	}

	void ConditionalVariuble::notify_all() 
	{
		_condition.notify_all(); 
	}
   


	void Mutex::open_mutex()
	{
		_mutex.unlock();
	}

	void Mutex::close_mutex()
	{
		_mutex.unlock();
	}
}