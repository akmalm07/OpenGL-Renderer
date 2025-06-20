#include "tools/include/input_manager.h"



namespace tools
{
	template<CallbackInputConcept InputStruct, typename ...Args>
	void InputManager::register_callback(const InputStruct& input, std::function<void(Args...)> cb)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;
		_registry[type].emplace_back(std::make_unique<InputEntry<InputStruct, Args...>>(input, std::move(cb)));
	}


	template<CallbackInputConcept InputStruct, typename ...Args>
	inline InputStruct& InputManager::get_input(const InputStruct& input, Args ...args)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;
		auto it = _registry.find(type);
		if (it == _registry.end()) return;
		for (auto& entry : it->second)
		{
			if (entry->matches(input))
			{
				auto* typed = dynamic_cast<view_ptr<InputEntry<InputStruct, Args...>>>(entry.get());
				if (typed)
				{
					return typed->input;
				}
			}
		}
		throw std::runtime_error("Input not found");
		return const_cast<InputStruct&>(input);
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

	template<CallbackInputConcept InputStruct>
	inline void InputManager::update_and_emit(const InputStruct& input)
	{
		constexpr InputType type = InputTypeResolver<InputStruct>::value;
		auto it = _registry.find(type);
		if (it == _registry.end()) return;
		for (auto& entry : it->second)
		{
			view_ptr<InputEntry<InputStruct>> typed = static_cast<view_ptr<InputEntry<InputStruct>>>(entry.get());
			if (typed && typed->matches(input))
			{
				if (typed->updater.has_value())
				{
					typed->updater.value()();
				}
				typed->callback();

			}
		}
	}

	template<CallbackInputConcept InputStruct>
	inline std::vector<view_ptr<InputEntry<InputStruct>>> InputManager::list_entries_values() const
	{
		auto type = InputTypeResolver<InputStruct>::value;
		auto it = _registry.find(type);
		if (it != _registry.end())
		{
			std::vector<view_ptr<InputEntry<InputStruct>>> result;
			for (const auto& entry : it->second)
			{
				view_ptr<InputEntry<InputStruct>> typed = static_cast<view_ptr<InputEntry<InputStruct>>>(entry.get());
				if (typed)
				{
					result.push_back(typed);
				}
			}
			return result;
		}
		return {};
	}


	const std::vector<std::unique_ptr<InputBase>>& tools::InputManager::list_entries(InputType type) const
	{
		auto it = _registry.find(type);
		if (it != _registry.end())
		{
			return it->second;
		}
		return {};
	}
}