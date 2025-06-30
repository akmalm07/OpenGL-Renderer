#include "headers.h"
#include "engine.h"

namespace Program
{
/*

		tools::CameraBundlePerspective cameraBundlePersp = {};
		cameraBundlePersp.nearZ = 0.1f;
		cameraBundlePersp.farZ = 1000.0f;
		cameraBundlePersp.speed = 0.05f;
		cameraBundlePersp.turnSpeed = 0.05f;
		cameraBundlePersp.position = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraBundlePersp.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundlePersp.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraBundlePersp.fov = 45.0f;
		cameraBundlePersp.aspectRatio = window.get_aspect_ratio();

		tools::QuaternionCamera camera(cameraBundlePersp);

		camera.set_commands_to_window(window);

		tools::CameraBundlePerspective cameraBundlePersp = {};
		cameraBundlePersp.nearZ = 0.1f;
		cameraBundlePersp.farZ = 1000.0f;
		cameraBundlePersp.speed = 0.1f;
		cameraBundlePersp.turnSpeed = 0.1f;
		cameraBundlePersp.position = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraBundlePersp.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundlePersp.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraBundlePersp.fov = 45.0f;
		cameraBundlePersp.aspectRatio = window.get_aspect_ratio();

*/

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

	//std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Constant>> create_demo_volocity_moveible_mesh()
	//{

	//	std::vector<glType::Vertex> vertices = tools::create_cube_vertices(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);

	//	std::vector<glType::Index> indices = tools::create_cube_indices();

	//	//vertices = tools::calculate_face_normals(vertices, indices);

	//	physics::MoveibleMeshBundle bundle; // Change -- CRITICAL
	//	ArrayBufferLayout layout1;
	//	layout1.location = 0;
	//	layout1.stride = Stride::STRIDE_3D;
	//	layout1.type = StrideType::POS;

	//	ArrayBufferLayout layout2;
	//	layout2.location = 1;
	//	layout2.stride = Stride::STRIDE_3D;
	//	layout2.type = StrideType::COL;

	//	bundle.vertexCount = vertices.size();
	//	bundle.pVertices = vertices.data();
	//	bundle.indexCount = indices.size();
	//	bundle.pIndices = indices.data();
	//	bundle.fullStride = FullStride::STRIDE_6D;
	//	bundle.indexed = true;
	//	bundle.pLayout1 = &layout1;
	//	bundle.pLayout2 = &layout2;
	//	bundle.rateOfChange = glm::vec3(0.0f, 0.0f, 0.0f);
	//	bundle.gravityAffected = true;

	//	return std::make_shared<physics::MoveibleMesh<glType::MovementType::Constant>>(bundle, true);
	//}
	//

	//std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Kenimatic>> create_demo_acceleration_moveible_mesh()
	//{

	//	std::vector<glType::Vertex> vertices = tools::create_cube_vertices(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);

	//	std::vector<glType::Index> indices = tools::create_cube_indices();

	//	//vertices = tools::calculate_face_normals(vertices, indices);

	//	physics::MoveibleMeshBundle bundle; // Change -- CRITICAL
	//	ArrayBufferLayout layout1;
	//	layout1.location = 0;
	//	layout1.stride = Stride::STRIDE_3D;
	//	layout1.type = StrideType::POS;

	//	ArrayBufferLayout layout2;
	//	layout2.location = 1;
	//	layout2.stride = Stride::STRIDE_3D;
	//	layout2.type = StrideType::COL;

	//	bundle.vertexCount = vertices.size();
	//	bundle.pVertices = vertices.data();
	//	bundle.indexCount = indices.size();
	//	bundle.pIndices = indices.data();
	//	bundle.fullStride = FullStride::STRIDE_6D;
	//	bundle.indexed = true;
	//	bundle.pLayout1 = &layout1;
	//	bundle.pLayout2 = &layout2;
	//	bundle.rateOfChange = glm::vec3(0.0f, 0.0f, 0.0f);
	//	bundle.gravityAffected = true;

	//	return std::make_shared<physics::MoveibleMesh<glType::MovementType::Kenimatic>>(bundle, true);
	//}

	//glUtil::Mesh create_demo_mesh()
	//{
	//std::vector<glType::Vertex> vertices = tools::create_cube_vertices(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), 2.0f);
	//
	//std::vector<glType::Index> indices = tools::create_cube_indices();

	//	glUtil::MeshBundle bundle;
	//	ArrayBufferLayout layout1;
	//	layout1.location = 0;
	//	layout1.stride = Stride::STRIDE_3D;
	//	layout1.type = StrideType::POS;

	//	ArrayBufferLayout layout2;
	//	layout2.location = 1;
	//	layout2.stride = Stride::STRIDE_3D;
	//	layout2.type = StrideType::COL;

	//	bundle.vertexCount = vertices.size();
	//	bundle.pVertices = vertices.data();
	//	bundle.indexCount = indices.size();
	//	bundle.pIndices = indices.data();
	//	bundle.fullStride = FullStride::STRIDE_6D;
	//	bundle.indexed = true;
	//	bundle.pLayout1 = &layout1;
	//	bundle.pLayout2 = &layout2;

	//	return glUtil::Mesh(bundle, true);
	//}

	//std::shared_ptr<physics::MoveibleMesh<glType::MovementType::Constant>> create_demo_floor_mesh()
	//{

	//	std::vector<glType::Vertex> vertices = tools::create_floor_vertices(glm::vec3(1.0f), glm::vec3(0.0f, -3.0, 0.0f), 30.0f);
	//	
	//	std::vector<glType::Index> indices = tools::create_floor_indices();

	//	physics::MoveibleMeshBundle bundle;
	//	ArrayBufferLayout layout1;
	//	layout1.location = 0;
	//	layout1.stride = Stride::STRIDE_3D;
	//	layout1.type = StrideType::POS;

	//	ArrayBufferLayout layout2;
	//	layout2.location = 1;
	//	layout2.stride = Stride::STRIDE_3D;
	//	layout2.type = StrideType::COL;

	//	bundle.vertexCount = vertices.size();
	//	bundle.pVertices = vertices.data();
	//	bundle.indexCount = indices.size();
	//	bundle.pIndices = indices.data();
	//	bundle.fullStride = FullStride::STRIDE_6D;
	//	bundle.indexed = true;
	//	bundle.pLayout1 = &layout1;
	//	bundle.pLayout2 = &layout2;
	//	bundle.rateOfChange = glm::vec3(0.0f, 0.0f, 0.0f);
	//	bundle.gravityAffected = true;

	//	return std::make_shared<physics::MoveibleMesh<glType::MovementType::Constant>>(bundle, true);
	//}
//
//glUtil::Mesh create_demo_mesh()
//{
//	return tools::contruct_default_mesh();
//}

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
		if (!camBundle.has_value())
		{
			tools::CameraBundlePerspective defaultCamBundle;
			defaultCamBundle.nearZ = 0.1f;
			defaultCamBundle.farZ = 1000.0f;
			defaultCamBundle.speed = 0.03f;
			defaultCamBundle.turnSpeed = 0.03f;
			defaultCamBundle.position = glm::vec3(0.0f, 0.0f, 0.0f);
			defaultCamBundle.front = glm::vec3(0.0f, 0.0f, -1.0f);
			defaultCamBundle.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			defaultCamBundle.fov = 45.0f;
			defaultCamBundle.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
			
			return tools::Window(width, height, title, true, defaultCamBundle, tools::CameraType::Classic);
		}
		return tools::Window(width, height, title, true, camBundle.value(), tools::CameraType::Quaternion);
	}


	/*

	tools::DirectionalLight create_directional_light(const tools::Camera& cam, glInit::GLProgram& program, const glm::vec3& direction, const glm::vec3& color)
	{
		tools::DirectionalLightBundle dirLightBundle;
		dirLightBundle.direction = direction;
		dirLightBundle.color = color;


		tools::DirectionalLight dirLight(dirLightBundle, program.get_id(), true);

		dirLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));

		//dirLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		return dirLight;
	}


	Engine::Engine()
	{
		_window.create_window(1000, 1000, "OpenGL", true, true);
		_window.set_escape_button(tools::Keys::Esc);

		_camera = Program::create_camera_persp(_window);

		_program = Program::create_program();

		//_demoMesh = Program::create_demo_mesh();

		_matrix.projection = _camera.get_projection();

		_matrix.view = _camera.get_view();

		std::cout << "Matrix: " << sizeof(_matrix) << "\n";

		_cameraUniform = Program::create_camera_uniform_buffer(_program, _matrix);

	}

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