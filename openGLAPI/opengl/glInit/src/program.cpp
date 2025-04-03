#include "headers.h"
#include "glInit/include/program.h"

GLProgram::GLProgram() : _shaderId(0), _uniformProjection(0), _uniformModel(0), _uniformView(0) {}

GLProgram::GLProgram(bool debug) : GLProgram()
{
	_debugMode = debug;
}

void GLProgram::create_shaders_from_string(const char* vertex_code, const char* fragment_code)
{
	compile_shader(vertex_code, fragment_code);
}

void GLProgram::create_shaders_from_files(const std::filesystem::path& vertex_location, const std::filesystem::path& fragment_location)
{
	if (_debugMode)
	{
		std::cout << "Creating shaders from files:\n"
			<< "\tVertex Shader: " << vertex_location.string() << "\n"
			<< "\tFragment Shader: " << fragment_location.string() << "\n";
	}
	
	if (std::filesystem::exists(vertex_location) && std::filesystem::exists(fragment_location))
	{
		compile_shader(
			read_file(vertex_location.string()),
			read_file(fragment_location.string())
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

void GLProgram::add_uniform(std::string_view name)
{
	unsigned int location = glGetUniformLocation(_shaderId, name.data());
	if (location == -1)
	{
		std::cerr << "Warning: uniform '" << name << "' not found in shader program! Did you forget to add it?" << std::endl;
		return;
	}
	_uniforms[name] = location;
}

unsigned int GLProgram::get_uniform_loc(std::string_view name) const
{
	auto it = _uniforms.find(name);
	if (it != _uniforms.end())
	{
		return it->second;
	}
	else
	{
		std::cerr << "Uniform " << name << " not found!" << std::endl;
		return 0;
	}
	
}

void GLProgram::link_model_matrix(glm::mat4& model_matrix)
{
	glUniformMatrix4fv(_uniformModel, 1, GL_FALSE, glm::value_ptr(model_matrix));
}

void GLProgram::link_projection_matrix(glm::mat4& projection_matrix)
{
	glUniformMatrix4fv(_uniformProjection, 1, GL_FALSE, glm::value_ptr(projection_matrix));
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

void GLProgram::compile_shader(std::string_view vertex_code, std::string_view fragment_code)
{
	_shaderId = glCreateProgram();
	if (!_shaderId)
	{
		std::cerr << "Error creating shader program!" << std::endl;
		return;
	}

	add_shader(_shaderId, vertex_code, GL_VERTEX_SHADER);
	add_shader(_shaderId, fragment_code, GL_FRAGMENT_SHADER);

	int result = 0;
	char eLog[1024] = { 0 };

	glLinkProgram(_shaderId);
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

void GLProgram::add_shader(unsigned int the_program, std::string_view shader_code, unsigned int shader_type)
{
	unsigned int theShader = glCreateShader(shader_type);

	const char* theCode[1];
	theCode[0] = shader_code.data();

	int codeLength[1];
	codeLength[0] = shader_code.length();

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	int result = 0;
	char eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cerr << "Error compiling the " << shader_type << " shader: " << eLog << std::endl;
		return;
	}

	glAttachShader(the_program, theShader);
}

void GLProgram::linking_uniforms()
{
	_uniformProjection = glGetUniformLocation(_shaderId, "projection");
	_uniformModel = glGetUniformLocation(_shaderId, "model");
	_uniformView = glGetUniformLocation(_shaderId, "view");
}