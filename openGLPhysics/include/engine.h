#pragma once

#include "config.h"

#include "tools/include/stride.h"
#include "tools/include/camera.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/timer.h"
#include "tools/include/window.h"
#include "tools/include/directional_light.h"
#include "glInit/include/program.h"
#include "physics/include/meshes.h"
#include "glUtil/include/uniform_buffer.h"
#include "physics/include/world.h"



namespace Program
{

	glInit::GLProgram create_program();

	physics::World create_world(glInit::GLProgram& program, tools::Window& window, bool debug = false);

	std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Constant>> create_demo_volocity_moveible_mesh();
	
	std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Kenimatic>> create_demo_acceleration_moveible_mesh();

	std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Constant>> create_demo_floor_mesh();

	glUtil::Mesh create_demo_mesh();

	void clear_color();

	tools::Window create_window(int width = 800, int height = 600, const std::string& title = "OpenGL", const std::optional<tools::CameraBundlePerspective>& camBundle = std::nullopt);

	template<class T>
	inline glUtil::UniformBuffer create_camera_uniform_buffer(const glInit::GLProgram& program, const T& matrix);

	/*

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
glUtil::UniformBuffer Program::create_camera_uniform_buffer(const glInit::GLProgram& program, const T& matrix)
{
	glUtil::UniformBuffer uniformBuffer;
	uniformBuffer.init(program.get_id(), "CameraData", 0, false);

	uniformBuffer.update_data(matrix.model, "model");
	uniformBuffer.update_data(matrix.view, "view");
	uniformBuffer.update_data(matrix.projection, "projection");

	return uniformBuffer;
*/
}