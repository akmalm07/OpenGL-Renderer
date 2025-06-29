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

		window.set_disable_cursor(true);
		window.set_escape_button(Keys::Esc, Action::Press);
		window.set_movement_callbacks();

		GLProgram program = Program::create_program();

		World world = Program::create_world(program, window, true);

	//	tools::ComponentRegistry<Mesh>& meshes = tools::ComponentRegistry<Mesh>::get_instance();
		//tools::ComponentRegistry<PhysicsBodyBase>& phys = tools::ComponentRegistry<PhysicsBodyBase>::get_instance();

		EntityRegistry reg;

		Entity entity = reg.register_entity("Joe");

		//PhysicsBodyBase physicsBody;
		//phys.add_component(entity, physicsBody);


		auto msh = Program::create_demo_mesh();

		//meshes.add_component(entity, msh);

		//auto mesh = Program::create_demo_volocity_moveible_mesh();
		//
		//auto floor = Program::create_demo_floor_mesh();

	/*	world.add_mesh(mesh);
		world.add_mesh(floor);*/

		world.update_mv_matrices();

		//world.bind_light();

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		while (!window.get_should_close())
		{
			window.update();
			
			program.bind();

			//program.link_model_matrix( // ERR!
			//	//meshes.get_component(entity).get_model_matrix()
			//	meshes.get_component(entity).get_model_matrix()
			//);


			//meshes.get_component(entity).render(); // ERR!

			msh.render();


			//PRINT_MAT4("Model Matrix: ", meshes.get_component(entity).get_model_matrix())

			world.update_mv_matrices_and_link(program);

			GLenum err;
			while ((err = glGetError()) != GL_NO_ERROR) {
				std::cerr << "OpenGL error: " << std::hex << err << std::endl;
			}

			Program::clear_color();

			// phys.get_component(entity).update(0.016);

			//world.render_meshes(program);

			program.unbind();

		}

		//world.unbind_light();

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
