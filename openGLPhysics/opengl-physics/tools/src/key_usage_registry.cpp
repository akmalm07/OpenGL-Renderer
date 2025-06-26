#include "headers.h"
#include "tools\include\key_usage_registry.h"



namespace tools
{

	std::vector<std::pair<Keys, Mods>> KeyUsageRegistry::keys_in_use() const
	{
		return _keys;
	}

	std::vector<Keys> KeyUsageRegistry::only_keys_in_use() const
	{
		std::vector<Keys> keys;
		keys.reserve(_keys.size());
		for (const auto& [key, val] : _keys)
		{
			keys.push_back(key);
		}
		return keys;
	}

	std::vector<Mods> KeyUsageRegistry::only_mods_in_use() const
	{
		std::vector<Mods> mods;
		mods.reserve(_keys.size());
		for (const auto& key : _keys)
		{
			mods.push_back(key.second);
		}
		return mods;
	}

	std::vector<std::pair<Keys, Mods>> KeyUsageRegistry::arrow_keys_in_use() const
	{
		std::vector<std::pair<Keys, Mods>> keys;
		keys.reserve(_keys.size());
		for (const auto& key : _keys)
		{
			if (key.first == Keys::Up || key.first == Keys::Down || key.first == Keys::Right || key.first == Keys::Left)
			{
				keys.push_back(key);
			}
		}
		return keys;
	}

	size_t KeyUsageRegistry::num_of_arrow_keys_in_use() const
	{
		size_t total = 0;
		for (const auto& key : _keys)
		{
			if (key.first == Keys::Up || key.first == Keys::Down || key.first == Keys::Right || key.first == Keys::Left)
			{
				total++;
			}
		}
		return total;
	}

	std::vector<std::pair<Keys, Mods>> KeyUsageRegistry::a_to_z_keys_in_use() const
	{
		std::vector<std::pair<Keys, Mods>> keys;
		keys.reserve(_keys.size());
		for (const auto& key : _keys)
		{
			if (INT(key.first) >= INT(Keys::A) && INT(key.first) <= INT(Keys::Z))
			{
				keys.push_back(key);
			}
		}
		return keys;
	}

	size_t KeyUsageRegistry::num_of_a_to_z_keys_in_use() const
	{
		size_t total = 0;
		for (const auto& key : _keys)
		{
			if (INT(key.first) >= INT(Keys::A) && INT(key.first) <= INT(Keys::Z))
			{
				total++;
			}
		}
		return total;
	}

	std::vector<std::array<Keys, KEY_MAX>> KeyUsageRegistry::only_keys_in_use_poly() const
	{
		std::vector<std::array<Keys, KEY_MAX>> keys;
		keys.reserve(_keysPoly.size());
		for (const auto& [key, val] : _keysPoly)
		{
			keys.push_back(key);
		}
		return keys;
	}

	std::vector<Mods> KeyUsageRegistry::only_mods_in_use_poly() const
	{
		std::vector<Mods> mods;
		mods.reserve(_keysPoly.size());
		for (const auto& key : _keysPoly)
		{
			mods.push_back(key.second);
		}
		return mods;
	}

	size_t KeyUsageRegistry::num_of_keys_in_use() const
	{
		return _keys.size();
	}

	size_t KeyUsageRegistry::num_of_keys_in_use_poly() const
	{
		return _keysPoly.size();
	}

	std::vector<std::pair<std::array<Keys, KEY_MAX>, Mods>> KeyUsageRegistry::keys_in_use_poly() const
	{
		return _keysPoly;
	}

	void KeyUsageRegistry::add_key(Keys key, std::optional<Mods> mod)
	{
		_keys.push_back(std::make_pair(key, mod.value_or(Mods::None)));
	}

	void KeyUsageRegistry::add_key_poly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{
		_keysPoly.push_back(std::make_pair(key, mod.value_or(Mods::None)));
	}

	void KeyUsageRegistry::remove_key(Keys key, std::optional<Mods> mod)
	{
		_keys.erase(std::remove(_keys.begin(), _keys.end(), std::make_pair(key, mod.value_or(Mods::None))), _keys.end());
	}

	void KeyUsageRegistry::remove_key_poly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod)
	{
		_keysPoly.erase(std::remove(_keysPoly.begin(), _keysPoly.end(), std::make_pair(key, mod.value_or(Mods::None))), _keysPoly.end());
	}

	KeyUsageRegistry& KeyUsageRegistry::get_instance()
	{
		static KeyUsageRegistry instance;
		return instance;
	}


	KeyUsageRegistry::KeyUsageRegistry() = default;
}
