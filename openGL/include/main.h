#pragma once

#include "engine.h"
#include "config.h"

#include "tools/include/entity_registry.h"

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

		EntityRegistry reg;

		DirectionalLight dirLight;

		auto& mesh = tools::ComponentRegistry<Mesh>::get_instance();
		auto mshBody = Program::create_demo_mesh();

		auto floorBody = Program::create_demo_floor_mesh();

		auto ent = reg.register_entity("Joe");
		
		auto entFlr = reg.register_entity("Floor");

		mesh.add_component(ent, std::move(mshBody));
		
		mesh.add_component(entFlr, std::move(floorBody));

		dirLight.bind();

		while (!window.get_should_close())
		{
			program.bind();
			{
				Program::clear_color();

				// Add Updaters Here

				window.update();

			}
			program.unbind();
		}

		dirLight.unbind();

		std::cout << "Exiting application..." << std::endl;
	}
}
