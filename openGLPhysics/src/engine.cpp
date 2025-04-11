#include "headers.h"
#include "engine.h"

namespace Program
{

	tools::Camera create_camera_ortho(tools::Window& window)
	{
		tools::CameraBundleOrthographic cameraBundleOrtho = {};
		cameraBundleOrtho.position = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraBundleOrtho.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundleOrtho.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraBundleOrtho.left = window.get_left_ortho();
		cameraBundleOrtho.right = window.get_right_ortho();
		cameraBundleOrtho.bottom = window.get_bottom_ortho();
		cameraBundleOrtho.top = window.get_top_ortho();
		cameraBundleOrtho.nearZ = 0.1f;
		cameraBundleOrtho.farZ = 1000.0f;
		cameraBundleOrtho.speed = 0.01f;
		cameraBundleOrtho.turnSpeed = 0.1f;

		tools::Camera camera(cameraBundleOrtho);

		camera.set_commands_to_window(window);

		auto keys = tools::KeyUsageRegistry::get_instance().a_to_z_keys_in_use();

		for (const auto& [key, mod] : keys)
		{
			std::function<bool()> func = [val = window.FindKeyComb(key), dt = window.get_delta_time_ref()]() -> bool
				{
					val->change_parameters(dt);
					return true;
				};

			window.SetFuncParamUpdaterKeys(key, std::move(func), mod);
		}

		window.SetMouseChangeUpdater([mouseMove = window.GetMouseMove(), &window, dt = window.get_delta_time_ref()]() -> bool
			{
				mouseMove->change_parameters(dt, window.GetMouseChangeXf(), window.GetMouseChangeYf());
				return true;
			}
		);

		return camera;
	}

	tools::Camera create_camera_persp(tools::Window& window)
	{

		tools::CameraBundlePerspective cameraBundlePersp = {};
		cameraBundlePersp.nearZ = 0.1f;
		cameraBundlePersp.farZ = 1000.0f;
		cameraBundlePersp.speed = 0.01f;
		cameraBundlePersp.turnSpeed = 0.1f;
		cameraBundlePersp.position = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraBundlePersp.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundlePersp.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraBundlePersp.fov = 45.0f;
		cameraBundlePersp.aspectRatio = window.get_aspect_ratio();

		tools::Camera camera(cameraBundlePersp);

		camera.set_commands_to_window(window);

		auto keys = tools::KeyUsageRegistry::get_instance().a_to_z_keys_in_use();

		for (const auto& [key, mod] : keys)
		{
			std::function<bool()> func = [val = window.FindKeyComb(key), &dt = window.get_delta_time_ref()]() -> bool
				{
					val->change_parameters(dt);
					std::cout << "Updating camera with  dt: " << dt << "\n";
					return true;
				};

			window.SetFuncParamUpdaterKeys(key, std::move(func), mod);
		}

		window.SetMouseChangeUpdater([mouseMove = window.GetMouseMove(), &window, &dt = window.get_delta_time_ref()]() -> bool
			{
				std::cout << "Updating camera with  dt: " << dt << "\n";
				std::cout << "Mouse Xf: " << window.GetMouseChangeXf() << " Mouse Yf: " << window.GetMouseChangeYf() << "\n";
				mouseMove->change_parameters(dt, window.GetMouseChangeXf(), window.GetMouseChangeYf());
				return true;
			}
		);


		return camera;
	}

	glInit::GLProgram create_program()
	{
		auto& config = Config::instance();

		glInit::GLProgram program;
		program.create_shaders_from_files(config.get_vert_shader_path(), config.get_frag_shader_path(), config.get_geom_shader_path());

		return program;
	}

	glUtil::Mesh create_demo_mesh()
	{

		std::vector<glType::Vertex> vertices = tools::create_cube_vertices(glm::vec3(1.0f, -1.0f, 0.0f), glm::vec3(1.0f, 0.5f, 0.5f), 1.0f);


		std::vector<glType::Index> indices = tools::create_cube_indices();

		vertices = tools::calculate_face_normals(vertices, indices);

		MeshBundle bundle;
		ArrayBufferLayout layout1;
		layout1.location = 0;
		layout1.stride = Stride::STRIDE_3D;
		layout1.type = StrideType::POS;

		ArrayBufferLayout layout2;
		layout2.location = 1;
		layout2.stride = Stride::STRIDE_3D;
		layout2.type = StrideType::COL;

		bundle.vertexCount = vertices.size();
		bundle.pVertices = vertices.data();
		bundle.indexCount = indices.size();
		bundle.pIndices = indices.data();
		bundle.fullStride = FullStride::STRIDE_6D;
		bundle.indexed = true;
		bundle.pLayout1 = &layout1;
		bundle.pLayout2 = &layout2;

		return glUtil::Mesh(bundle, true);
	}

	void clear_color()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	tools::Window create_window(int width, int height, const std::string& title)
	{
		tools::Window window(width, height, title);
		window.create_window(true, true);

		window.set_escape_button(tools::Keys::Esc);

		return window;
	}

	tools::DirectionalLight create_directional_light(const tools::Camera& cam, glInit::GLProgram& program, const glm::vec3& direction, const glm::vec3& color)
	{
		tools::DirectionalLightBundle dirLightBundle;
		dirLightBundle.direction = direction;
		dirLightBundle.color = color;


		tools::DirectionalLight dirLight(dirLightBundle, program.get_id(), true);

		dirLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));

		dirLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		return dirLight;
	}


	Engine::Engine()
	{
		_window.create_window(1000, 1000, "OpenGL", true, true);
		_window.set_escape_button(tools::Keys::Esc);

		_camera = Program::create_camera_persp(_window);

		_program = Program::create_program();

		_demoMesh = Program::create_demo_mesh();

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

		_demoMesh = Program::create_demo_mesh();

	}

	void Engine::run()
	{
		while (!_window.get_should_close())
		{
			_window.reset_delta_time();

			_window.poll_events();

			Program::clear_color();

			_program.use_shaders();

			_cameraUniform.bind();

			_cameraUniform.update_data(_camera.get_view(), "view");

			_demoMesh.render();

			_window.swap_buffers();

			_cameraUniform.unbind();
		}
	}

}