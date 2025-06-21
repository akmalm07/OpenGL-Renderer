#pragma once 
#include "tools/include/window_input.h"
#include <GLFW/glfw3.h>


namespace tools 
{
	
	template<CallbackInputConcept InputStruct>
	view_ptr<InputStruct> cast_to_type(view_ptr<CallbackInput> incoming)
	{
		if constexpr (std::same_as<InputStruct, AABButtonInput>)
			return static_cast<view_ptr<AABButtonInput>>(incoming);
		else if constexpr (std::same_as<InputStruct, KeyCombInputOne>)
			return static_cast<view_ptr<KeyCombInputOne>>(incoming);
		else if constexpr (std::same_as<InputStruct, KeyCombInputPoly>)
			return static_cast<view_ptr<KeyCombInputPoly>>(incoming);
		else if constexpr (std::same_as<InputStruct, MouseButtonInput>)
			return static_cast<view_ptr<MouseButtonInput>>(incoming);
		else if constexpr (std::same_as<InputStruct, MouseMoveInput>)
			return static_cast<view_ptr<MouseMoveInput>>(incoming);
		else
			throw std::runtime_error("Unknown input type");

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
	inline InputEntryConcrete<InputStruct, Args...>::InputEntryConcrete(InputStruct i, std::function<void(Args...)> cb, std::string_view name, std::optional<std::function<void(std::function<void(Args...)>)>> updater)
		: InputEntry<InputStruct>(std::move(i), name), updater(std::move(updater)), callback(std::move(cb))
	{
	}

	template<CallbackInputConcept InputStruct, typename ...Args>
	inline void InputEntryConcrete<InputStruct, Args...>::emit_and_update() const
	{
		if (updater)
		{
			updater.value()(callback);
		}
	}

	template<CallbackInputConcept InputStruct, typename ...Args>
	inline std::string_view InputEntryConcrete<InputStruct, Args...>::get_name() const
	{
		return name;
	}


	template<CallbackInputConcept InputStruct>
	inline InputEntry<InputStruct>::InputEntry(InputStruct i, std::string_view name)
		: input(std::move(i)), name(name)
	{
	}


	template<CallbackInputConcept InputStruct, typename ...Args>
	bool InputEntryConcrete<InputStruct, Args...>::matches_impl(const CallbackInput& incoming) const
	{
		view_ptr<CallbackInput> ptr = &incoming;
		view_ptr<InputStruct> casted = cast_to_type<InputStruct>(ptr);

		if constexpr (std::same_as<InputStruct, KeyCombInputOne>) {
			return input.number == casted->number && input.action == casted->action && input.mod == casted->mod;
		}
		else if constexpr (std::same_as<InputStruct, KeyCombInputPoly>) {
			return input.numbers == casted->number && input.action == casted->action && input.mod == casted->mod;
		}
		else if constexpr (std::same_as<InputStruct, MouseButtonInput>) {
			return input.button == casted->button && input.action == casted->action;
		}
		else if constexpr (std::same_as<InputStruct, AABButtonInput>) {
			return input.name == casted->name && input.button == casted->button && input.action == casted->action;
		}
		else if constexpr (std::same_as<InputStruct, MouseMoveInput>) {
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