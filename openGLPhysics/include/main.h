#pragma once

#include "engine.h"
#include "config.h"


namespace Renderer
{
	void Run()
	{
		using namespace tools;
		using namespace glUtil;
		using namespace glInit;

		Window window = Program::create_window(1000, 1000);

		Camera camera = Program::create_camera_persp(window);

		GLProgram program = Program::create_program();

		Mesh mesh = Program::create_demo_mesh();

		DirectionalLight directionalLight = Program::create_directional_light(camera, program);

		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		}matrix;

		matrix.projection = camera.get_projection();
		matrix.view = camera.get_view();


		//glUtil::UniformBuffer uniformBuffer = Program::create_camera_uniform_buffer(matrix);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		directionalLight.bind();
		while (!window.get_should_close())
		{
			window.reset_delta_time();

			window.poll_events();

			program.use_shaders();

			program.link_projection_matrix(matrix.projection); 
			program.link_model_matrix(matrix.model); 
		
			matrix.view = camera.get_view(); 
			program.link_view_matrix(matrix.view); 

			directionalLight.link_camera_pos(camera.get_position()); 

			//uniformBuffer.bind();
			//uniformBuffer.update_data(camera.get_view(), 1);

			Program::clear_color();

			mesh.render();

			window.swap_buffers();

			//uniformBuffer.unbind();
		}
		directionalLight.unbind();

		std::cout << "Exiting application..." << std::endl;
	}
}
