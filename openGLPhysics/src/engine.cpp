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

	tools::World create_world(glInit::GLProgram& program, tools::Window& window, bool debug)
	{
		return tools::World(program, window, debug);
	}


	glUtil::Mesh create_demo_mesh()
	{
		return tools::construct_default_mesh();
	}

	glUtil::Mesh create_demo_floor_mesh()
	{
		return tools::construct_default_floor_mesh();
	}

	void clear_color()
	{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	tools::Window create_window(int width, int height, const std::string& title, const std::optional<tools::CameraBundlePerspective>& camBundle)
	{
		using namespace units;

		if (!camBundle.has_value())
		{
			tools::CameraBundlePerspective defaultCamBundle;
			defaultCamBundle.nearZ = 0.1f;
			defaultCamBundle.farZ = 150.0f;
			defaultCamBundle.speed = 30.0_meters_per_sec;
			defaultCamBundle.turnSpeed = 30.0_meters_per_sec;
			defaultCamBundle.position = to_meters(glm::vec3(0.0f, 1.0f, 0.0f));
			defaultCamBundle.front = glm::vec3(0.0f, 0.0f, -1.0f);
			defaultCamBundle.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			defaultCamBundle.startPYR = glm::vec3(0.0f, -90.0f, 0.0f); 
			defaultCamBundle.fov = 45.0_degrees;
			defaultCamBundle.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			
			return tools::Window(width, height, title, true, defaultCamBundle, tools::CameraType::Quaternion);
		}
		return tools::Window(width, height, title, true, camBundle.value(), tools::CameraType::Quaternion);
	}

	physics::PhysicsBody create_demo_physics_body(const glUtil::Mesh& mesh)
	{
		physics::PhysicsBodyBundleAABB bundle;

		physics::MinMax minMax = physics::get_min_max_of_mesh(mesh);

		bundle.min = minMax.min;
		bundle.max = minMax.max;
		bundle.mass = 1.0f;
		bundle.gravityAffected = true;
		bundle.elasticity = 0.5f; 

		//bundle.rotation = glm::vec3(0.0f, 0.0f, 0.0f); 
		bundle.initalForce = units::to_newtons(physics::Force(glm::vec3(0.0f, 0.0f, 0.0f)));
		bundle.centerOfMass = physics::find_center_of_mass(mesh.get_verticies(), glUtil::FullStride::STRIDE_6D, glUtil::PosStride::STRIDE_3D);

		return physics::PhysicsBody(bundle);
	}

	physics::PhysicsBody create_demo_physics_body_floor(const glUtil::Mesh& mesh)
	{
		physics::PhysicsBodyBundleAABB bundle;
		
		physics::MinMax minMax = physics::get_min_max_of_mesh(mesh);

		bundle.min = minMax.min;
		bundle.max = minMax.max;
		bundle.mass = std::numeric_limits<float>::max();

		bundle.gravityAffected = false;
		bundle.elasticity = 0.8f; 
		bundle.initalForce = physics::Force(0.0f);

		return physics::PhysicsBody(bundle);
	}

/*
	Engine::Engine(int width, int height, bool orthoOrPerspective, bool debug)
	{

		_window = Program::create_window(width, height);

		_camera = (orthoOrPerspective ? Program::create_camera_persp(_window) : Program::create_camera_ortho(_window));

		_program = Program::create_program();

		_matrix.projection = _camera.get_projection();

		_matrix.view = _camera.get_view();

		_cameraUniform = Program::create_camera_uniform_buffer(_program, _matrix);

		//_demoMesh = Program::create_demo_mesh();

	}

	void Engine::run()
	{
		while (!_window.get_should_close())
		{
			_window.reset_delta_time();

			_window.poll_events();

			Program::clear_color();

			_program.bind();

			_cameraUniform.bind();

			_cameraUniform.update_data(_camera.get_view(), "view");

			_demoMesh.render();

			_window.swap_buffers();

			_cameraUniform.unbind();
		}
	}

	*/
}