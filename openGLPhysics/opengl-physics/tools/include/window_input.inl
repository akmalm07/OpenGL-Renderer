#pragma once 
#include "tools\include\window_input.h"
#include <GLFW/glfw3.h>


namespace tools 
{
	template<class ...Args>
	void KeyInputBase::change_parameter(Args && ...args)
	{
		_invoke->change_parameters(std::forward<Args>(args)...);
	}
	
	template<class ...Args>
	bool MouseButton<Args...>::execute() const
	{
		if (this->_updater)
		{
			this->_updater();
		}
		this->_result = std::apply(_func, _args);
		return this->_result;
	}

	template<class ...Args>
	void MouseButton<Args...>::change_parameter(Args && ...args)
	{
		_args = std::tuple<Args...>(std::forward<Args>(args)...);
	}


	template<class ...Args>
	bool AABButton<Args...>::execute() const
	{
		if (this->_updater)
		{
			this->_updater();
		}
		this->_result = std::apply(_func, _args);
		return this->_result;
	}

	template<class ...Args>
	void AABButton<Args...>::change_parameter(Args && ...args)
	{
		_args = std::tuple<Args...>(std::forward<Args>(args)...);
	}



	template<class ...Args>
	bool KeyComb<Args...>::execute() const
	{
		if (this->_updater)
		{
			this->_updater();
		}
		this->_result = std::apply(_func, _args);
		return this->_result;
	}

	template<class ...Args>
	void KeyComb<Args...>::change_parameter(Args && ...args)
	{
		_args = std::tuple<Args...>(std::forward<Args>(args)...);
	}


	template<class ...Args>
	bool MouseMovement<Args...>::execute() const
	{
		if (this->_updater)
		{
			this->_updater();
		}
		this->_result = std::apply(_func, _args);
		return this->_result;
	}

	template<class ...Args>
	void MouseMovement<Args...>::change_parameter(Args && ...args)
	{
		_args = std::tuple<Args...>(std::forward<Args>(args)...);
	}

}