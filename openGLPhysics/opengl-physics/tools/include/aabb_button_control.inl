#if 0

#pragma once


namespace tools
{


	template<glType::IsClass T, class ...Args>
	inline void AABButtonControl::AddAABButton(const AABButtonInput& input, bool(T::* func)(Args...), std::string_view str, std::tuple<Args...>&& args)
	{
		std::function<bool(Args...)> function = [this](Args&&... args)
			{
				return (this->*func)(std::forward<Args>(args)...);
			};
		_AABButtons[SIZET(input.action)].emplace(
			str, std::make_shared<AABButton<Args...>>(input, std::forward<std::function<bool(Args...)>>(function), std::forward<std::tuple<Args...>>(args))
		);
	}

	template<glType::IsClass T, class ...Args>
	inline void AABButtonControl::AddAABButton(const AABButtonInput& input, bool(T::* func)(Args...), std::string_view str, Args&&... args)
	{
		std::function<bool(Args...)> function = [this](Args&&... args)
			{
				return (this->*func)(std::forward<Args>(args)...);
			};
		_AABButtons[SIZET(input.action)].emplace(
			str, std::make_shared<AABButton<Args...>>(input, std::forward<std::function<bool(Args...)>>(function), std::forward<Args>(args)...)
		);
	}

	template<class F, class ... Args>
	void AABButtonControl::AddAABButton(const AABButtonInput& input, F&& function, std::string_view str, Args&&... args)
	{
		_AABButtons[SIZET(input.action)].emplace(
			str, std::make_shared<AABButton<Args...>>(input, std::forward<F>(function), std::forward<Args>(args)...)
		);
	}

	template<class ...Args>
	inline void AABButtonControl::AddAABButton(const AABButtonInput& input, std::function<bool(Args...)> func, std::string_view str, Args && ...args)
	{
		_AABButtons[SIZET(input.action)].emplace(
			str, std::make_shared<AABButton<Args...>>(input, std::forward<std::function<bool(Args...)>>(func), std::forward<Args>(args)...)
		);
	}

}
#endif