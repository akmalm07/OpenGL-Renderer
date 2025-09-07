#pragma once

#include "config.h"

class Config
{
public:

	std::filesystem::path get_frag_shader_path() const;
	
	std::filesystem::path get_vert_shader_path() const;
	
	std::filesystem::path get_geom_shader_path() const;


	std::filesystem::path get_frag_shadow_shader_path() const;
	
	std::filesystem::path get_vert_shadow_shader_path() const;
	

	static Config& instance();
private:

	Config();
	~Config();

	std::filesystem::path vert_shader_path = "shaders/main_shaders/shader.vert";
	std::filesystem::path frag_shader_path = "shaders/main_shaders/shader.frag";
	std::filesystem::path geom_shader_path = "shaders/main_shaders/shader.geom";

	std::filesystem::path vert_shadow_shader_path = "shaders/shadow_pass.vert";
	std::filesystem::path frag_shadow_shader_path = "shaders/shadow_pass.frag";

};

