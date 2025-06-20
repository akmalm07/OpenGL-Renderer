#pragma once

#include "config.h"

#include "tools\include\keys.h"
#include "tools\include\window_input_bundles.h"
#include "tools\include\thread.h"


namespace tools {

	template<CallbackInputConcept InputStruct>
	view_ptr<InputStruct> cast_to_type(view_ptr<CallbackInput> incoming);
	
	template<typename T> struct InputTypeResolver;

	template<> struct InputTypeResolver<KeyCombInputOne> {
		static constexpr InputType value = InputType::Key;
	};
	template<> struct InputTypeResolver<KeyCombInputPoly> {
		static constexpr InputType value = InputType::KeyPoly;
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

		void add_callback(std::function<void(Args...)> cb);

		void invoke_all(Args... args);

	private:
		std::vector<std::function<void(Args...)>> _callbacks;
	};


	struct InputBase
	{
		virtual bool matches(const CallbackInput& incoming) = 0;
		virtual ~InputBase() = default;
	};

	template<typename Child>
	struct InputEntryBase : InputBase
		// This is a CRTP base class for input entries, allowing for static polymorphism.
		// It ensures that the derived class implements the matches_impl method.
	{
		virtual ~InputEntryBase() = default;
		virtual bool matches_impl(const CallbackInput& input) const = 0;

		bool matches(const CallbackInput& incoming) const final
		{
			return static_cast<const Child*>(this)->matches_impl(incoming);
		}
	};


	template<CallbackInputConcept InputStruct, typename... Args>
	struct InputEntry : InputEntryBase<InputEntry<InputStruct, Args...>>
	{
	public:
		InputStruct input;
		std::function<void(Args...)> callback;
		std::optional<std::function<void()>> updater;

		InputEntry(InputStruct i, std::function<void(Args...)>& cb, std::optional<std::function<void()>> updater = std::nullopt);
		
		void emit_and_update() const;

		bool matches_impl(const CallbackInput& incoming) const override;
	};



} // namespace tools


#include "tools\include\window_input.inl"