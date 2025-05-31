#pragma once 
#include "tools\include\window_input.h"
#include <GLFW/glfw3.h>


namespace tools 
{
	template<typename ...Args>
	void CallbackDispatcher<Args...>::add_callback(Callback cb)
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
	InputEntry<InputStruct, Args...>::InputEntry(InputStruct i, Callback cb)
		: input(std::move(i)), callback(std::move(cb)) 
	{
	}


	template<CallbackInputConcept InputStruct, typename ...Args>
	bool InputEntry<InputStruct, Args...>::matches(const CallbackInput& incoming) const
	{
		const auto* casted = dynamic_cast<const InputStruct*>(&incoming);
		if (!casted) return false;

		if constexpr (std::is_same_v<InputStruct, KeyCombInputOne>) {
			return input.number == casted->number && input.action == casted->action && input.mod == casted->mod;
		}
		else if constexpr (std::is_same_v<InputStruct, KeyCombInputPoly>) {
			return input.number == casted->number && input.action == casted->action && input.mod == casted->mod;
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


	template<CallbackInputConcept InputStruct, typename ...Args>
	void InputManager::register_callback(const InputStruct& input, std::function<void(Args...)> cb)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;
		_registry[type].emplace_back(std::make_unique<InputEntry<InputStruct, Args...>>(input, std::move(cb)));
	}


	template<CallbackInputConcept InputStruct, typename ...Args>
	inline void InputManager::emit(const InputStruct& input, Args ...args)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;

		auto it = _registry.find(type);
		if (it == _registry.end()) return;

		for (auto& entry : it->second)
		{
			auto* typed = dynamic_cast<InputEntry<InputStruct, Args...>*>(entry.get());
			if (typed && typed->matches(input))
			{
				typed->callback(args...);
			}
		}
	}


	const std::vector<std::unique_ptr<InputEntryBase>>& tools::InputManager::list_entries(InputType type) const
	{
		auto it = _registry.find(type);
		if (it != _registry.end())
		{
			return it->second;
		}
		return {};
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