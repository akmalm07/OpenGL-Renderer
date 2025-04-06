#include "headers.h"
#include "tools\include\key_control.h"

namespace tools
{


	KeyControl::KeyControl() = default;


	void KeyControl::DelKeyComb(Keys name, std::optional<Mods> mod)
	{
		for (auto& item : _keyCombs)
		{
			auto it = item.find(std::make_pair(name, mod.value_or(Mods::None)));
			if (it == item.end())
			{
				throw std::runtime_error("Entered invalid window name ( Window.cpp : DelWindowButton )");
				return;
			}
			item.erase(it);
		}
	}

	void KeyControl::DelKeyCombPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{

		auto it = _keyCombsPoly.find(std::make_pair(key, mod.value_or(Mods::None)));
		if (it == _keyCombsPoly.end())
		{
			throw std::runtime_error("Entered invalid window name ( Window.cpp : DelWindowButton )");
			return;
		}
		_keyCombsPoly.erase(it);
	}


	std::vector<std::shared_ptr<KeyCombB>> KeyControl::FindKeyCombPolyList(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{
		std::vector<std::shared_ptr<KeyCombB>> thekeys;
		thekeys.reserve(2);

		for (auto& keys : _keyCombsPoly)
		{
			auto& [ky, val] = keys;

			if (ky.first == key && ky.second == mod.value_or(Mods::None))
			{
				thekeys.push_back(val);
			}

		}
		return thekeys;
	}


	std::vector<std::shared_ptr<KeyCombB>> KeyControl::FindKeyCombList(Keys key, std::optional<Mods> mod)
	{
		std::vector<std::shared_ptr<KeyCombB>> thekeys;
		thekeys.reserve(2);

		for (auto& keys : _keyCombs)
		{
			for (auto& [ky, val] : keys)
			{
				if (ky.first == key && ky.second == mod.value_or(Mods::None))
				{
					thekeys.push_back(val);
				}
			}
		}
		return thekeys;
	}



	size_t KeyControl::NumOfKeysInList(Keys key, std::optional<Mods> mod)
	{
		size_t value = 0;

		for (const auto& thekey : _keyCombs)
		{
			auto it = thekey.find(std::make_pair(key, mod.value_or(Mods::None)));

			if (it != thekey.end())
			{
				value++;
			}
			
		}
		return value;
	}


	size_t KeyControl::NumOfKeysInListPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{
		size_t value = 0;
		for (auto& keys : _keyCombsPoly)
		{
			auto& [ky, val] = keys;
			if (ky.first == key && ky.second == mod.value_or(Mods::None))
			{
				value++;
			}
		}
		return value;
	}


	std::shared_ptr<KeyCombB> KeyControl::FindKeyComb(Keys key, std::optional<Mods> mod)
	{
		for (auto& keys : _keyCombs)
		{
			auto it = keys.find(std::make_pair(key, mod.value_or(Mods::None)));

			if (it != keys.end())
			{
				return it->second;
			}
		}
		return nullptr;

	}

	std::shared_ptr<KeyCombB> KeyControl::FindKeyComb(Action action, Keys key, std::optional<Mods> mod)
	{
		return _keyCombs[SIZET(action)][std::make_pair(key, mod.value_or(Mods::None))];
	}


	std::shared_ptr<KeyCombB> KeyControl::FindKeyCombPoly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{
		for (auto& keys : _keyCombsPoly)
		{
			auto& [ky, val] = keys;
			if (ky.first == key && ky.second == mod.value_or(Mods::None))
			{
				return val;
			}
		}
		return nullptr;
	}


	void KeyControl::SetFuncParamUpdaterKeys(Keys key, std::function<bool()> func, std::optional<Mods> mod)
	{
		bool ran = false;
		for (auto& keys : _keyCombs)
		{
			auto it = keys.find(std::make_pair(key, mod.value_or(Mods::None)));
			if (it != keys.end())
			{
				it->second->set_updater(std::move(func));
				ran = true;
				break;
			}
		}
		if (!ran)
		{
			throw std::runtime_error("No such item exists!");
		}
	}


	void KeyControl::SetFuncParamUpdaterKeysPoly(const std::array<Keys, KEY_MAX>& key, std::function<bool()> func, std::optional<Mods> mod)
	{
		bool ran = false;

		for (auto& keys : _keyCombsPoly)
		{
			auto& [ky, val] = keys;
			if (ky.first == key && ky.second == mod.value_or(Mods::None))
			{
				val->set_updater(std::move(func));
				ran = true;
				break;
			}
		}

		if (ran)
		{
			throw std::runtime_error("No such item exists!");
		}
	}

	double KeyControl::GetKeyMoveX(Keys key, double val)
	{
		switch (key)
		{
		case Keys::A:
		case Keys::Left:
			return -1 * val;
		case Keys::D:
		case Keys::Right:
			return val;
		default:
			return 0.0;
		}
	}

	double KeyControl::GetKeyMoveY(Keys key, double val)
	{
		switch (key)
		{
		case Keys::W:
		case Keys::Up:
			return val;
		case Keys::S:
		case Keys::Down:
			return -1 * val;
		default:
			return 0.0;
		}
	}

	bool KeyControl::IsKeyPressed(Keys key, std::optional<Mods> mod)
	{
		auto modVal = mod.value_or(Mods::None); //
		return FindKeyComb(key, modVal)->is_pressed(toInt(key), toInt(modVal));
	}

	bool KeyControl::IsKeyPressed(int key, std::optional<int> mod)
	{
		auto modVal = mod.value_or(0);
		return FindKeyComb(KEYS(key), MODS(modVal))->is_pressed(toInt(key), toInt(mod.value_or(0)));
	}


}
