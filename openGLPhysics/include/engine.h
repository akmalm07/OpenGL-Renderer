#pragma once

#include "config.h"

#include "tools/include/stride.h"
#include "tools/include/camera.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/timer.h"
#include "tools/include/window.h"
#include "tools/include/directional_light.h"
#include "glInit/include/program.h"
#include "physics/include/world.h"
#include "tools/include/component_registry.h"
#include "tools/include/entity_registry.h"
#include "physics/include/physics_body.h"




namespace Program
{

	glInit::GLProgram create_program();

	tools::World create_world(glInit::GLProgram& program, tools::Window& window, bool debug = false);

	glUtil::Mesh create_demo_mesh();

	glUtil::Mesh create_demo_floor_mesh();

	void clear_color();

	tools::Window create_window(int width = 800, int height = 600, const std::string& title = "OpenGL", const std::optional<tools::CameraBundlePerspective>& camBundle = std::nullopt);

	physics::PhysicsBody create_demo_physics_body(const glUtil::Mesh& mesh);
	
	physics::PhysicsBody create_demo_physics_body_floor(const glUtil::Mesh& mesh);



}




using namespace units;
using namespace glType;
using namespace glUtil;
using namespace tools;
using namespace physics;
using namespace Program;