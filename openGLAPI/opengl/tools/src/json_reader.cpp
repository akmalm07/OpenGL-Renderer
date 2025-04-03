#include "headers.h"
#include "tools\include\json_reader.h"

#include <nlohmann\json.hpp>
#include <fstream>

JsonReader::JsonReader() = default;

JsonReader::JsonReader(const std::filesystem::path& json)
{
	try
	{
		_file.open(json, std::ios::in);
		if (!_file.is_open())
		{
			throw std::ios_base::failure("Failed to open file: " + json.string());
		}

		_path = json;

		_file >> _json;
	}
	catch (const std::ios_base::failure& e)
	{
		std::cerr << e.what() << std::endl;
	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::cerr << "Failed to parse JSON file: " << e.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "An error occurred: " << e.what() << std::endl;
	}
}


JsonReader::JsonReader(const std::string& raw)
{
	try
	{
		_json = nlohmann::json::parse(raw);
	}
	catch (const nlohmann::json::parse_error& e)
	{
		std::cerr << "Failed to parse JSON file: " << e.what() << std::endl;
	}
}


nlohmann::json::iterator JsonReader::begin()
{
	return _json.begin();
}

nlohmann::json::iterator JsonReader::end()
{
	return _json.end();
}


JsonReader::~JsonReader()
{
	_file.close(); 
}