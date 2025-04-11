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


	std::vector<float> create_sphere_vertices(float radius, int num_segments, int num_rings) {
		std::vector<float> vertices;
		for (int y = 0; y <= num_rings; ++y) {
			float v = (float)y / num_rings;
			float phi = glm::pi<float>() * v;


			for (int x = 0; x <= num_segments; ++x) {
				float u = (float)x / num_segments;
				float theta = 2.0f * glm::pi<float>() * u;


				float xPos = radius * cos(theta) * sin(phi);
				float yPos = radius * cos(phi);
				float zPos = radius * sin(theta) * sin(phi);


				// Position
				vertices.push_back(xPos);
				vertices.push_back(yPos);
				vertices.push_back(zPos);


				// Normal (for a sphere, the normal is the normalized position vector)
				vertices.push_back(xPos);
				vertices.push_back(yPos);
				vertices.push_back(zPos);


				vertices.push_back(1.0f);
				vertices.push_back(1.0f);
				vertices.push_back(0.0f);
			}
		}
		return vertices;
	}


	std::vector<unsigned int> create_sphere_indices(int num_segments, int num_rings) {
		std::vector<unsigned int> indices;
		for (int y = 0; y < num_rings; ++y) {
			for (int x = 0; x < num_segments; ++x) {
				unsigned int first = (y * (num_segments + 1)) + x;
				unsigned int second = first + num_segments + 1;


				indices.push_back(first);
				indices.push_back(second);
				indices.push_back(first + 1);


				indices.push_back(second);
				indices.push_back(second + 1);
				indices.push_back(first + 1);
			}
		}
		return indices;
	}


	std::vector<float> create_circle_vertices(float radius, int num_segments) {
		std::vector<float> vertices;
		vertices.push_back(0.0f); // Center x
		vertices.push_back(0.0f); // Center y
		vertices.push_back(0.0f); // Z coordinate (0 for 2D)


		// Color for center (example: red)
		vertices.push_back(1.0f);
		vertices.push_back(1.0f);
		vertices.push_back(0.0f);


		for (int i = 0; i <= num_segments; ++i) {
			float theta = 2.0f * glm::pi<float>() * (float)i / num_segments;


			float x = radius * cos(theta);
			float y = radius * sin(theta);


			// Position
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(0.0f); // Z coordinate (0 for 2D)


			// Color (example: white)
			vertices.push_back(1.0f);
			vertices.push_back(0.0f);
			vertices.push_back(0.0f);
		}
		return vertices;
	}


	std::vector<unsigned int> create_circle_indices(int num_segments) {
		std::vector<unsigned int> indices;
		for (int i = 1; i <= num_segments; ++i) {
			indices.push_back(0); // Center vertex
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		indices.push_back(0); // Connect last segment to the first
		indices.push_back(num_segments + 1);
		indices.push_back(1);


		return indices;
	}

	glUtil::Mesh create_demo_mesh()
	{

		std::vector<glType::Vertex> vertices = create_sphere_vertices(0.5, 5, 5);


		std::vector<glType::Index> indices = create_sphere_indices(5, 5);

		//vertices = tools::calculate_face_normals(vertices, indices);

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
		glClearColor(1.0f, 0.5f, 0.0f, 1.0f);
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