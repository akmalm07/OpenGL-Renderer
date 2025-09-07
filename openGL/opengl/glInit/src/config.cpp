#include "headers.h"
#include "glInit/include/config.h"


Config::Config() = default;

Config::~Config() = default;

std::filesystem::path Config::get_frag_shader_path() const
{
	return frag_shader_path;
}

std::filesystem::path Config::get_vert_shader_path() const
{
	return vert_shader_path;
}

std::filesystem::path Config::get_geom_shader_path() const
{
	return geom_shader_path;
}

std::filesystem::path Config::get_vert_shadow_shader_path() const
{
	return vert_shadow_shader_path;
}

std::filesystem::path Config::get_frag_shadow_shader_path() const
{
	return frag_shadow_shader_path;
}

Config& Config::instance()
{
	static Config instance;

	return instance;

}


