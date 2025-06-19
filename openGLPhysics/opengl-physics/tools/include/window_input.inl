#pragma once 
#include "tools/include/window_input.h"
#include <GLFW/glfw3.h>


namespace tools 
{
	
	template<CallbackInputConcept InputStruct>
	view_ptr<InputStruct> cast_to_type(view_ptr<CallbackInput> incoming)
	{
		switch(incoming->get_type())
		{
		case InputType::AABButton:
			return static_cast<view_ptr<AABButtonInput>>(incoming);
		case InputType::Key:
			return static_cast<view_ptr<KeyCombInputOne>>(incoming);
		case InputType::KeyPoly:
			return static_cast<view_ptr<KeyCombInputPoly>>(incoming);
		case InputType::MouseButton:
			return static_cast<view_ptr<MouseButtonInput>>(incoming);
		case InputType::MouseMovement:
			return static_cast<view_ptr<MouseMoveInput>>(incoming);
		default:
			throw std::runtime_error("Unknown input type");

		}

	}
	

	template<typename ...Args>
	void CallbackDispatcher<Args...>::add_callback(std::function<void(Args...)> cb)
	{
		_callbacks.push_back(std::move(cb));
	}

	template<typename ...Args>
	void CallbackDispatcher<Args...>::invoke_all(Args ...args)
	{
		for (auto& callback : _callbacks)
		{
			callback(args...);
		}
	}




	template<CallbackInputConcept InputStruct, typename ...Args>
	inline InputEntry<InputStruct, Args...>::InputEntry(InputStruct i, std::function<void(Args...)>& cb, std::optional<std::function<void()>> updater)
		: input(std::move(i)), callback(std::move(cb)), updater(std::move(updater))
	{
	}

	template<CallbackInputConcept InputStruct, typename ...Args>
	bool InputEntry<InputStruct, Args...>::matches_impl(const CallbackInput& incoming) const
	{
		view_ptr<InputStruct> casted = cast_to_type<InputStruct>(incoming);

		if constexpr (std::is_same_v<InputStruct, KeyCombInputOne>) {
			return input.number == casted->number && input.action == casted->action && input.mod == casted->mod;
		}
		else if constexpr (std::is_same_v<InputStruct, KeyCombInputPoly>) {
			return input.numbers == casted->number && input.action == casted->action && input.mod == casted->mod;
		}
		else if constexpr (std::is_same_v<InputStruct, MouseButtonInput>) {
			return input.button == casted->button && input.action == casted->action;
		}
		else if constexpr (std::is_same_v<InputStruct, AABButtonInput>) {
			return input.name == casted->name && input.button == casted->button && input.action == casted->action;
		}
		else if constexpr (std::is_same_v<InputStruct, MouseMoveInput>) {
			return input.change == casted->change && input.button == casted->button;
		}

		return false;
	}

	/*
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
	*/
}