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
		void register_callback(const InputStruct& input, std::function<void(Args...)> cb, std::string_view name, std::optional<std::function<void()>> updater = std::nullopt);

		template<CallbackInputConcept InputStruct, typename... Args>
		InputStruct& get_input(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct, typename... Args>
		void emit(const InputStruct& input, Args... args);

		template<CallbackInputConcept InputStruct>
		void emit_and_update(std::string_view name);


		template<CallbackInputConcept InputStruct, typename... Args>
		void emit(std::string_view name, Args... args);


		template<CallbackInputConcept InputStruct>
		void update_and_emit(const InputStruct& input);

		//const std::vector<std::unique_ptr<InputBase>>& list_entries_const_ref(InputType type) const;
		
		template<CallbackInputConcept InputStruct>
		const std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& list_entries_const_ref() const; // this returns only to the callbacks that have no parameters

	private:

		template<CallbackInputConcept InputStruct>
		std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& get_proper_list_ref();

		template<CallbackInputConcept InputStruct>
		const std::unordered_map<std::string, std::unique_ptr<InputEntry<InputStruct>>>& get_proper_list_ref() const;

	private:
		//std::unordered_map<InputType, std::vector<std::unique_ptr<InputBase>>> _registry;// ADD Naming system as well here
		
		std::unordered_map<std::string, std::unique_ptr<InputEntry<KeyCombInputOne>>> _keyInputs;
		std::unordered_map<std::string, std::unique_ptr<InputEntry<KeyCombInputPoly>>> _keyInputsPoly;
		std::unordered_map<std::string, std::unique_ptr<InputEntry<MouseButtonInput>>> _mouseButtonInputs;
		std::unordered_map<std::string, std::unique_ptr<InputEntry<AABButtonInput>>> _AABBInputs;
		std::unordered_map<std::string, std::unique_ptr<InputEntry<MouseMoveInput>>> _mouseMoveInputs;



		// Better Yet, make different constainers for all the different inputs

		// TEST: wether having sub sections is faster then not
	};

}

#include "tools/include/input_manager.inl"