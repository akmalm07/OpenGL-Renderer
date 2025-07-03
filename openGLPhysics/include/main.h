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

		GLProgram program = Program::create_program();

		World world = Program::create_world(program, window, true);


		auto& mesh = tools::ComponentRegistry<Mesh>::get_instance();
		
		auto& phys = tools::ComponentRegistry<PhysicsBody>::get_instance();

		
		auto msh = Program::create_demo_mesh();

		auto floor = Program::create_demo_floor_mesh();


		auto ent = world.add_entity("Joe");
		
		auto entFlr = world.add_entity("Floor");

		mesh.add_component(ent, msh);
		
		mesh.add_component(entFlr, floor);

		world.bind_light();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		while (!window.get_should_close())
		{
			program.bind();
			{
				Program::clear_color();

				world.update(program);

				window.update();

			}
			program.unbind();
		}

		world.unbind_light();

		std::cout << "Exiting application..." << std::endl;
	}
}


// EXTRA IMPLEMENTATIONS -- ONLY PARTIALLY TESTED
//void Run()
//{
//	using namespace tools;
//	using namespace glUtil;
//	using namespace glInit;
//	using namespace glType;
//	using namespace physics;
//
//	Window window = Program::create_window(1000, 1000);
//
//	QuaternionCamera camera = Program::create_quanternion_camera_persp(window);
//
//	GLProgram program = Program::create_program();
//
//	World world = Program::create_world(program, window, true);
//
//	auto mesh = Program::create_demo_volocity_moveible_mesh();
//
//	auto floor = Program::create_demo_floor_mesh();
//
//	world.add_mesh(mesh);
//	world.add_mesh(floor);
//
//	world.set_projection_matrix(camera.get_projection());
//	world.set_view_matrix(camera.get_view());
//
//	//program.link_projection_matrix(matrix.projection);
//	//program.link_model_matrix(matrix.model);
//
//	//glUtil::UniformBuffer uniformBuffer = Program::create_camera_uniform_buffer(matrix);
//	world.bind_light();
//
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//
//	while (!window.get_should_close())
//	{
//		window.reset_delta_time();
//
//		window.poll_events();
//
//		//world.run_shadow_pass();
//
//		program.bind();
//
//		program.link_projection_matrix(world.get_projection_matrix());
//		program.link_model_matrix(mesh->get_model_matrix());
//		//world.update_model_matrix(mesh->get_model_matrix());
//
//		mesh->update_position();
//
//		world.set_view_matrix(camera.get_view());
//		program.link_view_matrix(world.get_view_matrix());
//
//		world.update_camera_pos(camera.get_position());
//		//directionalLight.link_normal_mat();
//		//uniformBuffer.bind();
//		//uniformBuffer.update_data(camera.get_view(), 1);
//		//world.bind_shadow_tex();
//
//		Program::clear_color();
//
//		world.render_meshes();
//
//		//world.unbind_shadow_tex();
//		program.unbind();
//
//		window.swap_buffers();
//
//		//uniformBuffer.unbind();
//	}
//	world.unbind_light();
//
//	std::cout << "Exiting application..." << std::endl;
//}
