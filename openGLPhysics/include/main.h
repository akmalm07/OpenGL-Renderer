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
		using namespace glType;
		using namespace physics;

		Window window = Program::create_window(1000, 1000);

		Camera camera = Program::create_camera_persp(window);

		GLProgram program = Program::create_program();

		MoveibleMeshBundle bundle;
		std::vector<Vertex> vertices = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
			0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f
		};
		std::vector<Index> indices = { 0, 1, 2, 2, 3, 0 };
		bundle.vertexCount = vertices.size();
		bundle.pVertices = vertices.data();
		bundle.indexCount = indices.size();
		bundle.pIndices = indices.data();
		bundle.fullStride = FullStride::STRIDE_6D;
		bundle.pLayout1 = new ArrayBufferLayout{StrideType::POS, Stride::STRIDE_3D, 0};
		bundle.pLayout2 = new ArrayBufferLayout{StrideType::COL, Stride::STRIDE_3D, 1};
		bundle.startTime = false;

		std::shared_ptr<MoveibleMesh<MovementType::CONSTANT>> mesh = std::make_shared<MoveibleMesh<MovementType::CONSTANT>>(bundle, true);//Program::create_demo_mesh();

		World world = Program::create_world(program, true);

		//world.add_mesh(mesh);

		world.set_projection_matrix(camera.get_projection());
		world.set_view_matrix(camera.get_view());

		//program.link_projection_matrix(matrix.projection);
		//program.link_model_matrix(matrix.model);

		//glUtil::UniformBuffer uniformBuffer = Program::create_camera_uniform_buffer(matrix);
		world.bind_light();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		while (!window.get_should_close())
		{
			window.reset_delta_time();

			window.poll_events();

			program.use_shaders();

			program.link_projection_matrix(world.get_projection_matrix());
			program.link_model_matrix(world.get_model_matrix());

			world.set_view_matrix(camera.get_view());
			program.link_view_matrix(world.get_view_matrix());

			world.update_camera_pos(camera.get_position());
			//directionalLight.link_normal_mat();
			//uniformBuffer.bind();
			//uniformBuffer.update_data(camera.get_view(), 1);

			Program::clear_color();

			mesh->render();

			window.swap_buffers();

			//uniformBuffer.unbind();
		}
		world.unbind_light();

		std::cout << "Exiting application..." << std::endl;
	}
}
