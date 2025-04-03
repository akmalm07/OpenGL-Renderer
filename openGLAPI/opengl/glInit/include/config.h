#pragma once

#include "config.h"

class Config
{
public:

	std::filesystem::path get_frag_shader_path() const;
	
	std::filesystem::path get_vert_shader_path() const;

	static Config& instance();
private:

	Config();
	~Config();

	std::filesystem::path vert_shader_path = "shaders/shader.frag";
	std::filesystem::path frag_shader_path = "shaders/shader.vert";


};

