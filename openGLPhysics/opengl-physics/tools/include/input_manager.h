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

		template<CallbackInputConcept InputStruct, typename... Args> // must provide the updater if intended to use callbacks for window, otherwise, optional
		void register_callback(const InputStruct& input, std::function<void(Args...)> cb);


		template<CallbackInputConcept InputStruct, typename... Args>
		InputStruct& get_input(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct, typename... Args>
		void emit(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct>
		void update_and_emit(const InputStruct& input);

		const std::vector<std::unique_ptr<InputBase>>& list_entries(InputType type) const;
		
		template<CallbackInputConcept InputStruct>
		std::vector<view_ptr<InputEntry<InputStruct>>> list_entries_values() const; // this returns only to the callbacks that have no parameters

	private:
		std::unordered_map<InputType, std::vector<std::unique_ptr<InputBase>>> _registry;
		// TEST: wether having sub sections is faster then not
	};

}

#include "tools/include/input_manager.inl"