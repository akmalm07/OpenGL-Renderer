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

		
		auto mshBody = Program::create_demo_mesh();

		auto floorBody = Program::create_demo_floor_mesh();

		auto mshPhysBody = Program::create_demo_physics_body(mshBody);
		
		auto flrPhysBody = Program::create_demo_physics_body_floor(floorBody);


		auto ent = world.add_entity("Joe");
		
		auto entFlr = world.add_entity("Floor");

		mesh.add_component(ent, std::move(mshBody));
		
		mesh.add_component(entFlr, std::move(floorBody));

		phys.add_component(ent, std::move(mshPhysBody));

		phys.add_component(entFlr, std::move(flrPhysBody));

		world.bind_light();

		//phys.get_component(ent).add_volocity(glm::vec3(0.3, 0, 0));

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		while (!window.get_should_close())
		{
			program.bind();
			{
				Program::clear_color();

				world.update(program, window.get_delta_time_sec());

				window.update();

			}
			program.unbind();
		}

		world.unbind_light();

		std::cout << "Exiting application..." << std::endl;
	}
}
