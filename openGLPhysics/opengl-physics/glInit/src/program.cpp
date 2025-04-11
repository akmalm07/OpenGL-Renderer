#include "headers.h"
#include "glInit/include/program.h"


namespace glInit
{
	GLProgram::GLProgram() : _shaderId(0), _uniformProjection(0), _uniformModel(0), _uniformView(0) {}

	GLProgram::GLProgram(bool debug) : GLProgram()
	{
		_debugMode = debug;
	}

	void GLProgram::create_shaders_from_string(const char* vertexCode, const char* fragmentCode, const char* geomatry_shader)
	{
		compile_shader(vertexCode, fragmentCode);
	}

	void GLProgram::create_compute_shader_from_string(const char* compute_code)
	{
		compile_compute_shader(compute_code);
	}

	void GLProgram::create_compute_shader_from_file(const std::filesystem::path& compute_path)
	{
		std::string code = read_file(compute_path.string());
		compile_compute_shader(code);
	}

	void GLProgram::compile_compute_shader(std::string_view compute_code)
	{
		if (!_isInit)
		{
			_shaderId = glCreateProgram();
			_isInit = true;
		}

		unsigned int computeShader = glCreateShader(GL_COMPUTE_SHADER);

		const char* code = compute_code.data();
		glShaderSource(computeShader, 1, &code, nullptr);
		glCompileShader(computeShader);

		int success;
		glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetShaderInfoLog(computeShader, 512, nullptr, infoLog);
			std::cerr << "ERROR: Compute Shader Compilation Failed\n" << infoLog << std::endl;
		}

		glAttachShader(_shaderId, computeShader);
		glLinkProgram(_shaderId);
		//glUseProgram(_shaderId);

		glGetProgramiv(_shaderId, GL_LINK_STATUS, &success);
		if (!success)
		{
			char infoLog[512];
			glGetProgramInfoLog(_shaderId, 512, nullptr, infoLog);
			std::cerr << "ERROR: Shader Program Linking Failed\n" << infoLog << std::endl;
		}

		glDeleteShader(computeShader);
	}

	void GLProgram::dispatch(unsigned int x, unsigned int y, unsigned int z)
	{
		use_shaders();
		glDispatchCompute(x, y, z);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	}

	void GLProgram::create_shaders_from_files(const std::filesystem::path& vertex_location, const std::filesystem::path& fragment_location, const std::filesystem::path& geomatry_location)
	{
		if (_debugMode)
		{
			std::cout << "Creating shaders from files:\n"
				<< "\tVertex Shader: " << vertex_location.string() << "\n"
				<< "\tFragment Shader: " << fragment_location.string() << "\n"
				<< "\Geometry Shader: " << geomatry_location.string() << "\n";
		}

		if (std::filesystem::exists(vertex_location) && std::filesystem::exists(fragment_location))
		{
			compile_shader(
				read_file(vertex_location.string()),
				read_file(fragment_location.string()), (std::filesystem::exists(geomatry_location) ? read_file(geomatry_location.string()) : "")
			);
		}
	}

	std::string GLProgram::read_file(std::string_view file_location)
	{
		std::ifstream file(file_location.data());
		std::stringstream buffer;
		buffer << file.rdbuf();

		if (!file)
		{
			std::cerr << "Error reading file: " << file_location << std::endl;
			return "";
		}

		if (_debugMode)
		{
			std::cout << "Read file: " << file_location << "\n"
				<< "Content:\n"
				<< buffer.str() << "\n";
		}

		return buffer.str();
	}

	unsigned int GLProgram::get_projection_location()
	{
		return _uniformProjection;
	}

	unsigned int GLProgram::get_model_location()
	{
		return _uniformModel;
	}

	unsigned int GLProgram::add_uniform(std::string_view name)
	{
		//glLinkProgram(_shaderId);
		//glUseProgram(_shaderId);
		unsigned int location = glGetUniformLocation(_shaderId, name.data());
		if (location == -1)
		{
			std::cerr << "Warning: uniform '" << name << "' not found in shader program! Did you forget to add it?" << std::endl; // EROR HERE, PORGRAM CANNO FIND THE ITEM
			return 0;
		}
		_uniforms[name].location = location;
		return location;
	}

	unsigned int GLProgram::get_uniform_loc(std::string_view name) const
	{
		auto it = _uniforms.find(name);
		if (it != _uniforms.end())
		{
			return it->second.location;
		}
		else
		{
			std::cerr << "Uniform " << name << " not found!" << std::endl;
			return 0;
		}

	}

	unsigned int GLProgram::get_id() const
	{
		return _shaderId;
	}


	void GLProgram::link_model_matrix(const glm::mat4& modelMatrix)
	{
		glUniformMatrix4fv(_uniformModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}

	void GLProgram::link_projection_matrix(const glm::mat4& projectionMatrix)
	{
		glUniformMatrix4fv(_uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}

	void GLProgram::link_view_matrix(const glm::mat4& viewMatrix)
	{
		glUniformMatrix4fv(_uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	}

	void GLProgram::use_shaders()
	{
		glUseProgram(_shaderId);
	}

	void GLProgram::clear_shaders()
	{
		if (_shaderId != 0)
		{
			glDeleteProgram(_shaderId);
			_shaderId = 0;
		}
		if (_debugMode)
		{
			std::cout << "Clearing shader program with ID: " << _shaderId << std::endl;
		}

		_uniformProjection = 0;
		_uniformModel = 0;
		_uniformView = 0;
		_uniforms.clear();
	}

	GLProgram::~GLProgram()
	{
		clear_shaders();
	}

	void GLProgram::compile_shader(std::string_view vertex_code, std::string_view fragment_code, std::string_view geo_code)
	{
		if (!_isInit)
		{
			_shaderId = glCreateProgram();
			_isInit = true;
		}

		if (!_shaderId)
		{
			std::cerr << "Error creating shader program!" << std::endl;
			return;
		}

		add_shader(_shaderId, vertex_code, GL_VERTEX_SHADER);
		add_shader(_shaderId, fragment_code, GL_FRAGMENT_SHADER);
		if (geo_code != "")
		{
			add_shader(_shaderId, geo_code, GL_GEOMETRY_SHADER);
		}

		int result = 0;
		char eLog[1024] = { 0 };

		glLinkProgram(_shaderId);
		//glUseProgram(_shaderId);

		glGetProgramiv(_shaderId, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
			std::cerr << "Error linking program: " << eLog << std::endl;
			return;
		}

		glValidateProgram(_shaderId);
		glGetProgramiv(_shaderId, GL_VALIDATE_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(_shaderId, sizeof(eLog), NULL, eLog);
			std::cerr << "Error validating program: " << eLog << std::endl;
			return;
		}

		linking_uniforms();
	}

	void GLProgram::add_shader(unsigned int theProgram, std::string_view shaderCode, unsigned int shaderType)
	{
		unsigned int theShader = glCreateShader(shaderType);

		const char* theCode[1];
		theCode[0] = shaderCode.data();

		int codeLength[1];
		codeLength[0] = shaderCode.length();

		glShaderSource(theShader, 1, theCode, codeLength);
		glCompileShader(theShader);

		int result = 0;
		char eLog[1024] = { 0 };

		glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
			std::cerr << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
			return;
		}

		glAttachShader(theProgram, theShader);
	}

	void GLProgram::linking_uniforms()
	{
		_uniformProjection = glGetUniformLocation(_shaderId, "uProjection");
		_uniformModel = glGetUniformLocation(_shaderId, "uModel");
		_uniformView = glGetUniformLocation(_shaderId, "uView");
	}
}