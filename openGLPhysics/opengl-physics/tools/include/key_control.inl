#pragma once

#if 0
namespace tools
{
	template<class F, class ... Args>
	void KeyControl::AddKeyComb(const KeyCombInputPoly& input, F&& function, Args&&... args)
	{
		_keyCombsPoly.emplace(
			std::pair(input.number, input.mod),
			std::make_shared<KeyComb<Args...>>(input, std::forward<F>(function), std::forward<Args>(args)...)
		);

		KeyUsageRegistry::get_instance().add_key_poly(input.number, input.mod);
	}

	template<glType::IsClass T, class ...Args>
	inline void KeyControl::AddKeyComb(const KeyCombInputPoly& input, bool(T::* func)(Args...), std::tuple<Args...>&& args)
	{
		std::function<bool(Args...)> function = [this](Args&&... args)
			{
				return (this->*func)(std::forward<Args>(args)...);
			};
		_keyCombsPoly.emplace(
			std::pair(input.number, input.mod),
			std::make_shared<KeyComb<Args...>>(input, std::forward<std::function<bool(Args...)>>(function), std::forward<std::tuple<Args...>>(args))
		);

		KeyUsageRegistry::get_instance().add_key_poly(input.number, input.mod);
	}

	template<glType::IsClass T, class ...Args>
	inline void KeyControl::AddKeyComb(const KeyCombInputPoly& input, bool(T::* func)(Args...), Args&&... args)
	{
		std::function<bool(Args...)> function = [this](Args&&... args)
			{
				return (this->*func)(std::forward<Args>(args)...);
			};
		_keyCombsPoly.emplace(
			std::pair(input.number, input.mod),
			std::make_shared<KeyComb<Args...>>(input, std::forward<std::function<bool(Args...)>>(function), std::forward<Args>(args)...)
		);

		KeyUsageRegistry::get_instance().add_key_poly(input.number, input.mod);
	}

	template<class ...Args>
	inline void KeyControl::AddKeyComb(const KeyCombInputPoly& input, std::function<bool(Args...)> func, Args && ...args)
	{
		_keyCombsPoly.emplace(
			std::pair(input.number, input.mod),
			std::make_shared<KeyComb<Args...>>(input, std::forward<std::function<bool(Args...)>>(func), std::forward<Args>(args)...)
		);

		KeyUsageRegistry::get_instance().add_key_poly(input.number, input.mod);

	}


	template<class F, class ... Args>
	void KeyControl::AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, F&& function, Args&&... args)
	{
		std::shared_ptr<KeyCombB> sharedPtr = std::make_shared<KeyComb<Args...>>
			(input, std::forward<F>(function), std::forward<Args>(args)...);

		if (repeatAsWell)
		{
			_keyCombs[SIZET(Action::Repeat)].emplace(
				std::pair(input.number, input.mod), sharedPtr);
		}

		_keyCombs[SIZET(input.action)].emplace(
			std::pair(input.number, input.mod), sharedPtr);

		KeyUsageRegistry::get_instance().add_key(input.number, input.mod);

	}

	template<glType::IsClass T, class ...Args>
	inline void KeyControl::AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, bool(T::* func)(Args...), std::tuple<Args...>&& args)
	{
		std::function<bool(Args...)> function = [this](Args&&... args)
			{
				return (this->*func)(std::forward<Args>(args)...);
			};

		std::shared_ptr<KeyCombB> sharedPtr = std::make_shared<KeyComb<Args...>>
			(input, std::forward<std::function<bool(Args...)>>(function), std::forward<std::tuple<Args...>>(args));
		if (repeatAsWell)
		{
			_keyCombs[SIZET(Action::Repeat)].emplace(
				std::pair(input.number, input.mod), sharedPtr);
		}

		_keyCombs[SIZET(input.action)].emplace(
			std::pair(input.number, input.mod), sharedPtr);

		KeyUsageRegistry::get_instance().add_key(input.number, input.mod);
	}

	template<glType::IsClass T, class ...Args>
	inline void KeyControl::AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, bool(T::* func)(Args...), Args&&... args)
	{
		std::shared_ptr<KeyCombB> sharedPtr = std::make_shared<KeyComb<Args...>>
			(input, std::forward<bool(T::*)(Args...)>(func), std::forward<Args>(args)...);
		if (repeatAsWell)
		{
			_keyCombs[SIZET(Action::Repeat)].emplace(
				std::pair(input.number, input.mod), sharedPtr);
		}

		_keyCombs[SIZET(input.action)].emplace(
			std::pair(input.number, input.mod), sharedPtr);

		KeyUsageRegistry::get_instance().add_key(input.number, input.mod);
	}

	template<class ...Args>
	inline void KeyControl::AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, std::function<bool(Args...)> func, Args && ...args)
	{
		std::shared_ptr<KeyCombB> sharedPtr = std::make_shared<KeyComb<Args...>>
			(input, std::forward<std::function<bool(Args...)>>(func), std::forward<Args>(args)...);

		if (repeatAsWell)
		{
			_keyCombs[SIZET(Action::Repeat)].emplace(
				std::pair(input.number, input.mod), sharedPtr);
		}

		_keyCombs[SIZET(input.action)].emplace(
			std::pair(input.number, input.mod), sharedPtr);


		KeyUsageRegistry::get_instance().add_key(input.number, input.mod);

	}

}

#endif