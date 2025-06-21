#include "tools/include/input_manager.h"



namespace tools
{
	template<CallbackInputConcept InputStruct, typename ...Args>
	void InputManager::register_callback(const InputStruct& input, std::function<void(Args...)> cb, std::string_view name, std::optional<std::function<void()>> updater)
	{
		auto& properList = get_proper_list_ref<InputStruct>();
			
		properList.emplace(
			std::string(name),
			std::make_unique<InputEntryConcrete<InputStruct, Args...>>(input, std::move(cb), std::move(updater))
		);
	}

	template<CallbackInputConcept InputStruct, typename ...Args>
	inline InputStruct& InputManager::get_input(const InputStruct& input, Args ...args)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;
		auto& properList = get_proper_list_ref<InputStruct>();

		for (const auto& entry : properList->second)
		{
			if (entry->matches(input))
			{
				return entry->input;
			}
		}
		throw std::runtime_error("Input not found");
		return const_cast<InputStruct&>(input);
	}

	template<CallbackInputConcept InputStruct, typename ...Args>
	inline void InputManager::emit(const InputStruct& input, Args ...args)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;

		auto& properList = get_proper_list_ref<InputStruct>();

		for (auto& [name, entry] : properList)
		{
			view_ptr<InputEntryConcrete<InputStruct, Args...>> typed = static_cast<view_ptr<InputEntryConcrete<InputStruct, Args...>>>(entry.get());
			if (typed && typed->matches(input))
			{
				typed->callback(args...);
			}
		}

		/*
		auto it = _registry.find(type);
		if (it == _registry.end()) return;

		for (auto& entry : it->second)
		{
			view_ptr<InputEntry<InputStruct>> typed = static_cast<view_ptr<InputEntry<InputStruct, Args...>>>(entry.get());
			if (typed && typed->matches(input))
			{
				typed->callback(args...);
			}
		}
		*/
	}

	template<CallbackInputConcept InputStruct>
	inline void InputManager::emit_and_update(std::string_view name)
	{
		auto& properList = get_proper_list_ref<InputStruct>();

		auto it = properList.find(name);
		if (it == properList.end()) 
			return;
		else
			*it->emit_and_update();
	}

	
	template<CallbackInputConcept InputStruct, typename ...Args>
	inline void InputManager::emit(std::string_view name, Args ...args)
	{
		auto& properList = get_proper_list_ref<InputStruct>();
		auto it = properList.find(name);
		if (it == properList.end())
			return;
		else
			it->second->emit(args...);
	}


	template<CallbackInputConcept InputStruct>
	inline void InputManager::update_and_emit(const InputStruct& input)
	{
		auto& properList = get_proper_list_ref<InputStruct>();
		for (auto& [name, entry] : properList)
		{
			view_ptr<InputEntry<InputStruct>> typed = static_cast<view_ptr<InputEntry<InputStruct>>>(entry.get());
			if (typed && typed->matches(input))
			{
				typed->emit_and_update();
			}
		}
	}

	template<CallbackInputConcept InputStruct>
	inline const std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& InputManager::list_entries() const
	{
		const auto& properList = get_proper_list_ref<InputStruct>();

		return properList; 
	}

	template<CallbackInputConcept InputStruct>
	inline std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& InputManager::get_proper_list_ref()
	{
		if constexpr (std::same_as<KeyCombInputOne, InputStruct>)
		{
			return _keyInputs;
		}
		else if constexpr (std::same_as<KeyCombInputPoly, InputStruct>)
		{
			return _keyInputsPoly;
		}
		else if constexpr (std::same_as<MouseButtonInput, InputStruct>)
		{
			return _mouseButtonInputs;
		}
		else if constexpr (std::same_as<AABButtonInput, InputStruct>)
		{
			return _AABBInputs;
		}
		else if constexpr (std::same_as<MouseMoveInput, InputStruct>)
		{
			return _mouseMoveInputs;
		}
		else
		{
			throw std::runtime_error("Unknown input type");
		}
	}
	

	template<CallbackInputConcept InputStruct>
	inline const std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& InputManager::get_proper_list_ref()
	{
		if constexpr (std::same_as<KeyCombInputOne, InputStruct>)
		{
			return _keyInputs;
		}
		else if constexpr (std::same_as<KeyCombInputPoly, InputStruct>)
		{
			return _keyInputsPoly;
		}
		else if constexpr (std::same_as<MouseButtonInput, InputStruct>)
		{
			return _mouseButtonInputs;
		}
		else if constexpr (std::same_as<AABButtonInput, InputStruct>)
		{
			return _AABBInputs;
		}
		else if constexpr (std::same_as<MouseMoveInput, InputStruct>)
		{
			return _mouseMoveInputs;
		}
		else
		{
			throw std::runtime_error("Unknown input type");
		}
	}

}