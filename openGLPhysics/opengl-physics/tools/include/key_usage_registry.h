#pragma once


#include "config.h"

#include "tools\include\keys.h"

namespace tools
{
	class KeyUsageRegistry
	{
	public:
		std::vector<std::pair<Keys, Mods>> keys_in_use() const;
		
		std::vector<Keys> only_keys_in_use() const;
		
		std::vector<Mods> only_mods_in_use() const;

		std::vector<std::pair<Keys, Mods>> arrow_keys_in_use() const;

		size_t num_of_arrow_keys_in_use() const;
		
		std::vector<std::pair<Keys, Mods>> a_to_z_keys_in_use() const;
		
		size_t num_of_a_to_z_keys_in_use() const;

		std::vector<std::array<Keys, KEY_MAX>> only_keys_in_use_poly() const;
		
		std::vector<Mods> only_mods_in_use_poly() const;

		size_t num_of_keys_in_use() const;
		
		size_t num_of_keys_in_use_poly() const;

		static KeyUsageRegistry& get_instance();

		std::vector<std::pair<std::array<Keys, KEY_MAX>, Mods>> keys_in_use_poly() const;	

		void add_key(Keys key, std::optional<Mods> mod = std::nullopt);

		void add_key_poly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);

		void remove_key(Keys key, std::optional<Mods> mod = std::nullopt);

		void remove_key_poly(const std::array<Keys, KEY_MAX>& key, std::optional<Mods> mod = std::nullopt);

	private:
		KeyUsageRegistry();

		std::vector<std::pair<Keys, Mods>> _keys;

		std::vector<std::pair<std::array<Keys, KEY_MAX>, Mods>> _keysPoly;

	};
}