#pragma once
#if 0

#include "config.h"

#include "tools\include\keys.h"

#include "tools\include\window_input.h"

#include "tools\include\key_usage_registry.h"

#include <memory>
#include <unordered_map>
#include <vector>
#include <array>

namespace tools
{
	class KeyControl
	{
	public:

		KeyControl();

		std::vector<std::shared_ptr<KeyCombB>> FindKeyCombList(Keys key, std::optional<Mods> mod = std::nullopt);

		std::vector<std::shared_ptr<KeyCombB>> FindKeyCombPolyList(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);


		size_t NumOfKeysInList(Keys key, std::optional<Mods> mod = std::nullopt);

		size_t NumOfKeysInListPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);


		std::shared_ptr<KeyCombB> FindKeyComb(Keys key, std::optional<Mods> mod = std::nullopt);
		
		std::shared_ptr<KeyCombB> FindKeyComb(Action action, Keys key, std::optional<Mods> mod = std::nullopt);

		std::shared_ptr<KeyCombB> FindKeyCombPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);


		void DelKeyComb(Keys key, std::optional<Mods> mod = std::nullopt);

		void DelKeyCombPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);


		void SetFuncParamUpdaterKeys(Keys key, std::function<bool()> func, std::optional<Mods> mod = std::nullopt);

		void SetFuncParamUpdaterKeysPoly(const std::array<Keys, KEY_MAX>& key, std::function<bool()> func, std::optional<Mods> mod = std::nullopt);


		double GetKeyMoveX(Keys key, double val);
		
		double GetKeyMoveY(Keys key, double val);


		template<class F, class ... Args>
		void AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, F&& function, Args&&... args);

		template<glType::IsClass T, class ... Args>
		void AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, bool(T::* func)(Args...), std::tuple<Args...>&& args);

		template<class ... Args>
		void AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, std::function<bool(Args...)> func, Args&&... args);

		template<glType::IsClass T, class ... Args>
		void AddKeyComb(bool repeatAsWell, const KeyCombInputOne& input, bool(T::* func)(Args...), Args&&... args);



		template<class F, class ... Args>
		void AddKeyComb(const KeyCombInputPoly& input, F&& function, Args&&... args);

		template<glType::IsClass T, class ... Args>
		void AddKeyComb(const KeyCombInputPoly& input, bool(T::* func)(Args...), Args&&... args);

		template<glType::IsClass T, class ... Args>
		void AddKeyComb(const KeyCombInputPoly& input, bool(T::* func)(Args...), std::tuple<Args...>&& args);

		template<class ... Args>
		void AddKeyComb(const KeyCombInputPoly& input, std::function<bool(Args...)> func, Args&&... args);

		bool IsKeyPressed(Keys key, std::optional<Mods> mod = std::nullopt);
		
		bool IsKeyPressed(int key, std::optional<int> mod = std::nullopt);

	protected:

		std::array<std::unordered_map <std::pair<Keys, Mods>, std::shared_ptr<KeyCombB>>, SIZET(Action::Count)> _keyCombs;

		std::unordered_map <std::pair<std::array<Keys, KEY_MAX>, Mods>, std::shared_ptr<KeyCombB>> _keyCombsPoly;
	};

}

#include "key_control.inl"


#endif
