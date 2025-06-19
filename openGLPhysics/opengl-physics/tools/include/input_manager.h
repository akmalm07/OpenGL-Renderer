#pragma once


#include "config.h"

#include "tools/include/window_input.h"

namespace tools
{
	class InputManager
		// This class manages the registration and invocation of input callbacks.
		// It allows for registering callbacks for different input types and invoking them when the input is received.
	{
	public:

		template<CallbackInputConcept InputStruct, typename... Args>
		void register_callback(const InputStruct& input, std::function<void(Args...)> cb);


		template<CallbackInputConcept InputStruct, typename... Args>
		InputStruct& get_input(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct, typename... Args>
		void emit(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct>
		void auto_update(const InputStruct& input);


		const std::vector<std::unique_ptr<InputBase>>& list_entries(InputType type) const;

	private:
		std::unordered_map<InputType, std::vector<std::unique_ptr<InputBase>>> _registry;
	};

}

#include "tools/include/input_manager.inl"