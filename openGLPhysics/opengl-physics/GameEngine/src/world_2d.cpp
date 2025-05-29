#include "headers.h"
#include "GameEngine/include/world_2d.h"

namespace GameEngine
{
	World2D::World2D() = default;


	World2D::World2D(glInit::GLProgram& program, tools::Window& window, bool debug)
	{
		init_quaternion_camera(window);

		_debug = debug;

		if (_debug)
		{
			std::cout << "World2D initialized with directional light with program ID: " << program.get_id() << std::endl;
		}
	}

	World2D::World2D(glInit::GLProgram& program, tools::Window& window, const glm::vec2& gravity, bool debug)
		: World2D(program, window, debug)
	{
		_gravity = glm::vec3(gravity, 0.0f);
	}

	void World2D::add_scene(std::shared_ptr<tools::Scene> scene)
	{
		_scene = scene;

		if (_debug)
		{
			std::cout << "Scene added to World2D." << std::endl;
		}
	}

	glm::mat4 World2D::get_model_matrix()
	{
		return _matrix.model;
	}

	glm::mat4 World2D::get_view_matrix() const
	{
		return _matrix.view;
	}

	glm::mat4 World2D::get_projection_matrix() const
	{
		return _matrix.projection;
	}

	void World2D::set_model_matrix(const glm::mat4& mat)
	{
		_matrix.model = mat;
	}

	void World2D::set_view_matrix(const glm::mat4& mat)
	{
		_matrix.view = mat;
	}

	void World2D::set_projection_matrix(const glm::mat4& mat)
	{
		_matrix.projection = mat;
	}

	void World2D::update_model_matrix(const glm::mat4& mat)
	{
		_matrix.model = mat;

		//_directionalLight.link_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));
	}


	void World2D::update_mv_matrices(const glm::mat4& model)
	{
		_matrix.model = model;
		_matrix.view = _camera.get_view();

		//_directionalLight.link_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));
		//_directionalLight.link_camera_pos(_camera.get_position());
	}

	void World2D::update_mv_matrices_and_link(glInit::GLProgram& program)
	{
		update_mv_matrices();

		program.link_projection_matrix(_matrix.projection);
		program.link_model_matrix(_matrix.model);
		program.link_view_matrix(_matrix.view);
	}


	void World2D::render_objects(float deltaTime)
	{
		for (auto& obj : _objects)
		{
			obj->render({}, deltaTime);
		}
	}


	void World2D::init_quaternion_camera(tools::Window& window)
	{
		tools::CameraBundleOrthographic cameraBundleOrtho = {};
		cameraBundleOrtho.nearZ = 0.1f;
		cameraBundleOrtho.farZ = 1000.0f;
		cameraBundleOrtho.speed = 0.05f;
		cameraBundleOrtho.turnSpeed = 0.05f;
		cameraBundleOrtho.position = glm::vec3(0.0f, 0.0f, 1.0f);
		cameraBundleOrtho.front = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraBundleOrtho.worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraBundleOrtho.top = window.get_top_ortho();
		cameraBundleOrtho.bottom = window.get_bottom_ortho();
		cameraBundleOrtho.left = window.get_left_ortho();
		cameraBundleOrtho.right = window.get_right_ortho();

		_camera.init(cameraBundleOrtho);

		_camera.set_commands_to_window(window);


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
				mouseMove->change_parameters(dt, -window.GetMouseChangeXf(), window.GetMouseChangeYf());
				return true;
			}
		);

		_matrix.projection = _camera.get_projection();
		_matrix.view = _camera.get_view();
		_matrix.model = glm::mat4(1.0f);
	}


	void World2D::add_object(std::shared_ptr<GameObject> object)
	{
		object->add_gravity(_gravity);
		_objects.push_back(object);
		if (_debug)
		{
			std::cout << "Mesh added to World2D." << std::endl;
		}
	}

	void World2D::add_objects(std::vector<std::shared_ptr<GameObject>>& object)
	{
		for (auto& m : object)
		{
			m->add_gravity(_gravity);
		}

		_objects.insert(_objects.end(), object.begin(), object.end());
		if (_debug)
		{
			std::cout << "Meshes added to World2D." << std::endl;
		}
	}

	void World2D::init()
	{

	}

	glm::vec3 World2D::get_acc_due_to_gravity() const
	{
		return _gravity;
	}


	World2D::~World2D() = default;






	//Ctor:
		//tools::DirectionalLightBundle dirLightBundle;
		//dirLightBundle.direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		//dirLightBundle.color = glm::vec3(1.0f);

		//_directionalLight = tools::DirectionalLight(dirLightBundle, program.get_id(), debug);

		//_directionalLight.set_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));

		//_directionalLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		//_directionalLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));



	//void World2D::set_directional_light(tools::DirectionalLight& light)
	//{
	//	_directionalLight = std::move(light);
	//	if (_debug)
	//	{
	//		std::cout << "Directional light set in World2D." << std::endl;
	//	}
	//}

	//void World2D::bind_light()
	//{
	//	_directionalLight.bind();
	//}

	//void World2D::unbind_light()
	//{
	//	_directionalLight.unbind();
	//}

	//void World2D::update_camera_pos(const glm::vec2& camPos)
	//{
	//	_directionalLight.link_camera_pos(camPos);
	//}
}