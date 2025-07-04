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

		auto physBody = Program::create_demo_physics_body();

		auto floor = Program::create_demo_floor_mesh();



		auto ent = world.add_entity("Joe");
		
		auto entFlr = world.add_entity("Floor");

		mesh.add_component(ent, std::move(msh));
		
		mesh.add_component(entFlr, std::move(floor));

		phys.add_component(ent, std::move(physBody));

		world.bind_light();

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		while (!window.get_should_close())
		{
			program.bind();
			{
				Program::clear_color();

				world.update(program, window.get_delta_time());

				window.update();

			}
			program.unbind();
		}

		world.unbind_light();

		std::cout << "Exiting application..." << std::endl;
	}
}
