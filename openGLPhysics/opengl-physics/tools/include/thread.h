#pragma once


#include "config.h"

#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>


namespace tools
{

	template <bool HaveMutex, bool HaveConditional, class ...Args>
	class ThreadT
	{
	public:
		ThreadT() = default;

		ThreadT(std::function<bool(Args...)>func, Args&&... args, std::string_view name, bool debug);

		void initalize(std::function<bool(Args...)> func, Args&&... args, std::string_view name, bool debug);

		ThreadT(const ThreadT& other) = delete;
		ThreadT(ThreadT&& other) = delete;

		ThreadT& operator=(const ThreadT&) = delete;
		ThreadT& operator=(ThreadT&&) = delete;

		void start(bool independent);
		void join();

		void wait_for_condition(bool& value);
		bool is_joinable() const;

		void run_independent();

		~ThreadT() = default;
	private:
		std::function<bool(Args...)> _function;
		std::thread _thread;
		std::tuple<Args...> _args;
		std::string_view _name;
		bool _debug;
	};


	struct ConditionalVariuble
	{
	public:
		void wait_for_condition(std::mutex& mutex);

		void wait_for_condition(std::mutex& mutex, bool& value);

		void notify_one();

		void notify_all();

	private:
		std::condition_variable _condition;
	};

	struct Mutex
	{
	public:
		void open_mutex();
		void close_mutex();
	private:
		std::mutex _mutex;
	};


	template <class ...Args>
	class ThreadT <true, true, Args...> : public Mutex, private ConditionalVariuble, public ThreadT <false, false, Args...>
	{
	public:
		void notify_condition()
		{
			return ConditionalVariuble::notify_condition();
		}

		void wait_for_condition()
		{
			return ConditionalVariuble::wait_for_condition(_mutex);
		}

		void wait_for_condition(bool& value)
		{
			return ConditionalVariuble::wait_for_condition(_mutex, value);
		}
	};


	template <class ...Args>
	class ThreadT <false, true, Args...> : public ConditionalVariuble, public ThreadT <false, false, Args...>
	{
	};



	template <class ...Args>
	class ThreadT <true, false, Args...> : private Mutex, public ThreadT <false, false, Args...>
	{
	};





	template <bool HaveMutex, bool HaveConditional, class ...Args>
	inline void ThreadT<HaveMutex, HaveConditional, Args...>::run_independent()
	{
		_thread.detach();
		if (_debug)
		{
			std::cout << "ThreadT: " << _name << " with id: " << _thread.get_id() << " is running independently!" << std::endl;
		}
	}

	template <bool HaveMutex, bool HaveConditional, class ...Args>
	inline void ThreadT<HaveMutex, HaveConditional, Args...>::join()
	{
		if (_thread.joinable())
		{
			if (_debug)
			{
				std::cout << "ThreadT " << _name << " is joining!" << std::endl;
			}
			_thread.join();
		}
		else
		{
			throw std::runtime_error("ThreadT is not joinable!");
		}
	}



	template<bool HaveMutex, bool HaveConditional, class ...Args>
	inline bool ThreadT<HaveMutex, HaveConditional, Args...>::is_joinable() const
	{
		return _thread.joinable();
	}


	template<bool HaveMutex, bool HaveConditional, class ...Args>
	inline ThreadT<HaveMutex, HaveConditional, Args...>::ThreadT(const std::function<bool(Args...)> func, Args && ...args, std::string_view name, bool debug)
	{
		initalize(func, std::forward<Args>(args)..., name, debug);
	}

	template <bool HaveMutex, bool HaveConditional, class ...Args>
	inline void ThreadT<HaveMutex, HaveConditional, Args...>::initalize(std::function<bool(Args...)> func, Args&&... args, std::string_view name, bool debug)
	{
		_function = std::move(func);
		_args = std::tuple<Args...>(std::forward<Args>(args)...);
		_name = name;
		_debug = debug;
	}

	template<bool HaveMutex, bool HaveConditional, class ...Args>
	inline void ThreadT<HaveMutex, HaveConditional, Args...>::start(bool independent)
	{
		if (_debug)
		{
			std::cout << "ThreadT " << _name << " is starting!" << std::endl;
		}

		_thread = std::thread(
			[this]() -> void
			{
				std::apply(_function, _args);
			}
		);

		if (independent)
		{
			run_independent();
		}
	}
} // namespace tools

namespace vkType
{
	using Thread = tools::ThreadT<true, true>;
} // namespace vkType