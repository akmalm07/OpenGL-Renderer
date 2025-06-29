#pragma once

#include "config.h"


namespace glInit
{
	enum class UnformType
	{
		FLOAT = 0,
		INT,
		UINT,
		BOOL,
		VEC2,
		VEC3,
		VEC4,
		MAT2,
		MAT3,
		MAT4
	};

	class GLProgram
	{
	public:
		GLProgram();
		GLProgram(bool debug);

		// Vertex/Fragment shader shader.geom
		void create_shaders_from_string(const char* vertex_code, const char* fragment_code, const char* geomatry_shader = "");
		void create_shaders_from_files(const std::filesystem::path& vertex_location, const std::filesystem::path& fragment_location, const std::filesystem::path& geomatry_shader = "");

		// Compute shader UNTESTED -- Very risky 
		void create_compute_shader_from_string(const char* compute_code);
		void create_compute_shader_from_file(const std::filesystem::path& compute_path);
		void dispatch(unsigned int x, unsigned int y, unsigned int z = 1);

		std::string read_file(std::string_view file_location);

		unsigned int get_projection_location();
		unsigned int get_model_location();

		template <class T>
		void update_uniforms(const std::vector<T>& data);

		template <class T>
		void update_uniform(std::string_view name, const T& data);

		unsigned int add_uniform(std::string_view name);
		unsigned int get_uniform_loc(std::string_view name) const;

		unsigned int get_id() const;

		void link_model_matrix(const glm::mat4& modelMatrix);
		void link_projection_matrix(const glm::mat4& projectionMatrix);
		void link_view_matrix(const glm::mat4& viewMatrix);

		void bind();
		void unbind();

		void clear_shaders();

		~GLProgram();

	private:
		unsigned int _shaderId = 0;
		unsigned int _uniformProjection = 0, _uniformModel = 0, _uniformView = 0;

		bool _isInit = false;

		struct Uniform
		{
			unsigned int location;
			UnformType type;
		};

		std::unordered_map<std::string_view, Uniform> _uniforms;
		bool _debugMode = false;

	private:
		void compile_shader(std::string_view vertex_code, std::string_view fragment_code, std::string_view geo_code = "");

		void add_shader(unsigned int theProgram, std::string_view shaderCode, unsigned int shaderType);

		void compile_compute_shader(std::string_view compute_code);
		void linking_uniforms();

		static void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id,
			GLenum severity, GLsizei length,
			const GLchar* message, const void* userParam);
	};


	template<class T>
	inline void GLProgram::update_uniforms(const std::vector<T>& data)
	{

		if (data.size() != _uniforms.size())
		{
			std::cerr << "Mismatch in uniform count and data size. Cannot update uniforms." << std::endl;
			return;
		}

		for (const auto& uniform : _uniforms)
		{
			if (uniform.second.location != 0)
			{
				switch (uniform.second.type)
				{
				case UnformType::FLOAT:
					glUniform1f(uniform.second.location, static_cast<float>(data));
					break;
				case UnformType::INT:
					glUniform1i(uniform.second.location, static_cast<int>(data));
					break;
				case UnformType::UINT:
					glUniform1ui(uniform.second.location, static_cast<unsigned int>(data));
					break;
				case UnformType::BOOL:
					glUniform1i(uniform.second.location, static_cast<bool>(data));
					break;
				case UnformType::VEC2:
					glUniform2fv(uniform.second.location, 1, glm::value_ptr(static_cast<glm::vec2>(data)));
					break;
				case UnformType::VEC3:
					glUniform3fv(uniform.second.location, 1, glm::value_ptr(static_cast<glm::vec3>(data)));
					break;
				case UnformType::VEC4:
					glUniform4fv(uniform.second.location, 1, glm::value_ptr(static_cast<glm::vec4>(data)));
					break;
				case UnformType::MAT2:
					glUniformMatrix2fv(uniform.second.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat2>(data)));
					break;
				case UnformType::MAT3:
					glUniformMatrix3fv(uniform.second.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat3>(data)));
					break;
				case UnformType::MAT4:
					glUniformMatrix4fv(uniform.second.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat4>(data)));
					break;
				default:
					break;
				}
			}
		}
	}

	template<class T>
	inline void GLProgram::update_uniform(std::string_view name, const T& data)
	{
		if (_uniforms.find(name) == _uniforms.end())
		{
			std::cerr << "Uniform '" << name << "' not found in shader program! Cannot update." << std::endl;
			return;
		}

		auto& uniform = _uniforms[name];

		if (uniform.location != 0)
		{
			switch (uniform.type)
			{
			case UnformType::FLOAT:
				glUniform1f(uniform.location, static_cast<float>(data));
				break;
			case UnformType::INT:
				glUniform1i(uniform.location, static_cast<int>(data));
				break;
			case UnformType::UINT:
				glUniform1ui(uniform.location, static_cast<unsigned int>(data));
				break;
			case UnformType::BOOL:
				glUniform1i(uniform.location, static_cast<bool>(data));
				break;
			case UnformType::VEC2:
				glUniform2fv(uniform.location, 1, glm::value_ptr(static_cast<glm::vec2>(data)));
				break;
			case UnformType::VEC3:
				glUniform3fv(uniform.location, 1, glm::value_ptr(static_cast<glm::vec3>(data)));
				break;
			case UnformType::VEC4:
				glUniform4fv(uniform.location, 1, glm::value_ptr(static_cast<glm::vec4>(data)));
				break;
			case UnformType::MAT2:
				glUniformMatrix2fv(uniform.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat2>(data)));
				break;
			case UnformType::MAT3:
				glUniformMatrix3fv(uniform.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat3>(data)));
				break;
			case UnformType::MAT4:
				glUniformMatrix4fv(uniform.location, 1, GL_FALSE, glm::value_ptr(static_cast<glm::mat4>(data)));
				break;
			default:
				break;
			}
		}
		else
		{
			std::cerr << "Warning: uniform '" << name << "' location is zero." << std::endl;
		}
	}
}
