#include "headers.h"
#include "engine.h"

namespace Program
{


	glInit::GLProgram create_program()
	{
		auto& config = Config::instance();

		glInit::GLProgram program(
			config.get_vert_shader_path(), config.get_frag_shader_path(), config.get_geom_shader_path());

		return program;
	}

	glUtil::Mesh create_demo_mesh()
	{
		return tools::construct_default_mesh();
	}

	glUtil::Mesh create_demo_floor_mesh()
	{
		return tools::construct_default_floor_mesh();
	}

	tools::World create_world(glInit::GLProgram& program, tools::Window& window)
	{
		return tools::World(program, window);
	}

	tools::DirectionalLight create_directional_light(unsigned int programID)
	{

		return tools::DirectionalLight();
	}

	void clear_color()
	{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	tools::Window create_window(int width, int height, const std::string& title, const std::optional<tools::CameraBundlePerspective>& camBundle)
	{
		if (!camBundle.has_value())
		{
			tools::CameraBundlePerspective defaultCamBundle;
			defaultCamBundle.nearZ = 0.1f;
			defaultCamBundle.farZ = 150.0f;
			defaultCamBundle.speed = 30.0;
			defaultCamBundle.turnSpeed = 30.0;
			defaultCamBundle.position = glm::vec3(0.0f, 1.0f, 0.0f);
			defaultCamBundle.front = glm::vec3(0.0f, 0.0f, -1.0f);
			defaultCamBundle.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			defaultCamBundle.startPYR = glm::vec3(0.0f, -90.0f, 0.0f); 
			defaultCamBundle.fov = 45.0;
			defaultCamBundle.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			
			return tools::Window(width, height, title, true, defaultCamBundle, tools::CameraType::Quaternion);
		}
		return tools::Window(width, height, title, true, camBundle.value(), tools::CameraType::Quaternion);
	}

}