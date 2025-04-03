#pragma once

#include "config.h"
#include <nlohmann/json.hpp>
#include <fstream>



class JsonReader
{
private:
	std::ifstream _file;
	nlohmann::json _json;
	std::filesystem::path _path;
public:
	JsonReader();
	JsonReader(const std::filesystem::path& json);

	JsonReader(const std::string& raw);

	nlohmann::json::iterator begin();

	nlohmann::json::iterator end();

	nlohmann::json operator[](std::string_view key) const
	{
		return _json[key.data()];
	}

	template<class T = nlohmann::json>
	T get(std::string_view key) const;

	~JsonReader();

};

namespace vkType
{
	using Json = nlohmann::json;
}

template<class T>
inline T JsonReader::get(std::string_view key) const
{
	if (_json.contains(key))
	{
		try
		{
			return _json.at(key).get<T>();
		}
		catch (const nlohmann::json::type_error& e)
		{
			std::cerr << "Type error for key \"" << key << "\": " << e.what() << std::endl;
			throw;
		}
	}
	else
	{
		std::cerr << "Key \"" << key << "\" does not exist for json file: " << _path << std::endl;
		throw std::runtime_error("Item does not exist");
	}
}
