#pragma once

#include "config.h"

#include "tools/include/stride.h"
#include "tools/include/camera.h"
#include "tools/include/timer.h"
#include "tools/include/window.h"
#include "tools/include/directional_light.h"
#include "glInit/include/program.h"
#include "glUtil/include/mesh.h"
#include "glUtil/include/uniform_buffer.h"

namespace Program
{
	tools::Camera create_camera_ortho(tools::Window& window);

	tools::Camera create_camera_persp(tools::Window& window);

	glInit::GLProgram create_program();

	glUtil::Mesh create_demo_mesh();

	void clear_color();

	tools::Window create_window(int width = 800, int height = 600, const std::string& title = "OpenGL");

	tools::DirectionalLight create_directional_light(const tools::Camera& cam, glInit::GLProgram& program, 
		const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& color = glm::vec3(1.0f, 1.0f, 1.0f));

	template<class T>
	inline glUtil::UniformBuffer create_camera_uniform_buffer(const T& matrix);



	class Engine
	{
	public:
		Engine();

		Engine(int width, int height, bool orthoOrPerspective, bool debug);

		void run();
	private:
		tools::Window _window;	
		tools::Camera _camera;	
		glInit::GLProgram _program;
		glUtil::UniformBuffer _cameraUniform;
		glUtil::Mesh _demoMesh;

		std::vector<glUtil::Mesh> _meshes;

		std::vector<glUtil::UniformBuffer> _uniformBuffs;

		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		}_matrix;
	};

}


template<class T>
glUtil::UniformBuffer Program::create_camera_uniform_buffer(const T& matrix)
{
	glUtil::UniformBuffer uniformBuffer(0, matrix, sizeof(glm::mat4), 3, false);
	return uniformBuffer;
}