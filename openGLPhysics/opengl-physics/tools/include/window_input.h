#pragma once

#include "headers.h"

#include "tools\include\keys.h"
#include "tools\include\window_input_bundles.h"
#include "tools\include\thread.h"


namespace tools {


	template<typename T> struct InputTypeResolver;

	template<> struct InputTypeResolver<KeyCombInputOne> {
		static constexpr InputType value = InputType::Key;
	};
	template<> struct InputTypeResolver<KeyCombInputPoly> {
		static constexpr InputType value = InputType::Key;
	};
	template<> struct InputTypeResolver<MouseButtonInput> {
		static constexpr InputType value = InputType::MouseButton;
	};
	template<> struct InputTypeResolver<AABButtonInput> {
		static constexpr InputType value = InputType::AABButton;
	};
	template<> struct InputTypeResolver<MouseMoveInput> {
		static constexpr InputType value = InputType::MouseMovement;
	};



	template<typename... Args>
	class CallbackDispatcher
	{
	public:
		using Callback = std::function<void(Args...)>;

		void add_callback(Callback cb);

		void invoke_all(Args... args);

	private:
		std::vector<Callback> _callbacks;
	};



	struct InputEntryBase
	{
		virtual ~InputEntryBase() = default;
		virtual bool matches(const CallbackInput& input) const = 0;
	};


	template<CallbackInputConcept InputStruct, typename... Args>
	struct InputEntry : InputEntryBase
	{
		InputStruct input;
		using Callback = std::function<void(Args...)>;
		Callback callback;

		InputEntry(InputStruct i, Callback cb);

		bool matches(const CallbackInput& incoming) const override;
	};



	class InputManager // FIX: ADD AN AUTOMATIC UPDATER INTO THIS SYSTEM SO THAT THE CALLBASCKS CAN BE EASILY MANAGED : SELF DUE TASK (No AI) FOR InputManager Class 
	{
	public:

		template<CallbackInputConcept InputStruct, typename... Args>
		void register_callback(const InputStruct& input, std::function<void(Args...)> cb);


		template<CallbackInputConcept InputStruct, typename... Args>
		void emit(const InputStruct& input, Args... args);


		const std::vector<std::unique_ptr<InputEntryBase>>& list_entries(InputType type) const;

	private:
		std::unordered_map<InputType, std::vector<std::unique_ptr<InputEntryBase>>> _registry;
	};



} // namespace tools


#include "tools\include\window_input.inl"