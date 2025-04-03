#pragma once

#include "config.h"

class GLProgram
{
public:
    GLProgram();
    
    GLProgram(bool debug);

    void create_shaders_from_string(const char* vertex_code, const char* fragment_code);
    void create_shaders_from_files(const std::filesystem::path& vertex_location, const std::filesystem::path& fragment_location);

    std::string read_file(std::string_view file_location);

    unsigned int get_projection_location();
    unsigned int get_model_location();

    void add_uniform(std::string_view name);

    unsigned int get_uniform_loc(std::string_view name) const;

    void link_model_matrix(glm::mat4& model_matrix);
    void link_projection_matrix(glm::mat4& projection_matrix);

    void use_shaders();

    void clear_shaders();

    ~GLProgram();

private:
    unsigned int _shaderId;
    unsigned int _uniformProjection, _uniformModel, _uniformView;

    std::unordered_map<std::string_view, unsigned int> _uniforms;

	bool _debugMode = false; 

private:

    void compile_shader(std::string_view vertex_code, std::string_view fragment_code);
    void add_shader(unsigned int the_program, std::string_view shader_code, unsigned int shader_type);

    void linking_uniforms();
};