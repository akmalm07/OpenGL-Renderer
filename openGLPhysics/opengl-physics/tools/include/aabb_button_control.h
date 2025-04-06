#pragma once


#include "config.h"

#include "tools\include\keys.h"

#include "tools\include\window_input.h"

namespace tools
{
	class AABButtonControl
	{
	protected:
		static constexpr size_t KEY_CONST = 1024;
	public:

		AABButtonControl();

		std::shared_ptr<AABButtonB> FindAABButton(std::string_view name);

		void DelAABButton(std::string_view name);

		bool IsKeyActive(Keys key, Action act) const;

		void AddFuncParamUpdaterButton(std::string_view name, std::function<bool()> function);

		void ChangeFuncParamUpdaterButton(std::string_view name, std::function<bool()> function);


		template<class F, class ... Args>
		void AddAABButton(const AABButtonInput& input, F&& function, std::string_view str, Args&&... args);

		template<glType::IsClass T, class ... Args>
		void AddAABButton(const AABButtonInput& input, bool(T::* func)(Args...), std::string_view str, std::tuple<Args...>&& args);

		template<class ... Args>
		void AddAABButton(const AABButtonInput& input, std::function<bool(Args...)> func, std::string_view str, Args&&... args);

		template<glType::IsClass T, class ...Args>
		void AddAABButton(const AABButtonInput& input, bool(T::* func)(Args...), std::string_view str, Args&&... args);


		inline const std::array<bool, KEY_CONST>& GetKeysConstRef() { return _keys; }

		inline std::array<bool, KEY_CONST> GetKeys() { return _keys; }

	protected:

		std::array<bool, KEY_CONST> _keys{ false };

		std::array <std::unordered_map <std::string_view, std::shared_ptr<AABButtonB>>, SIZET(Action::Count)> _AABButtons;

	protected:
		void SetKey(unsigned int key, bool val);

	};

}

#include "aabb_button_control.inl"