#include "headers.h"

#include "physics\include\world.h"
#include "tools\include\key_usage_registry.h"

namespace physics
{
	World::World() = default;


	World::World(glInit::GLProgram& program, tools::Window& window, bool debug)
	{

		tools::DirectionalLightBundle dirLightBundle;
		dirLightBundle.direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		dirLightBundle.color = glm::vec3(1.0f);

		_directionalLight = tools::DirectionalLight(dirLightBundle, program.get_id(), debug);

		_directionalLight.set_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));

		_directionalLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		_directionalLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));
		 
		init_quaternion_camera(window);
		
		_debug = debug;

		if (_debug)
		{
			std::cout << "World initialized with directional light with program ID: " << program.get_id() << std::endl;
		}
	}

	void World::add_scene(std::shared_ptr<tools::Scene> scene)
	{
		_scene = scene;
		
		if (_debug)
		{
			std::cout << "Scene added to World." << std::endl;
		}
	}

	glm::mat4 World::get_model_matrix()
	{
		return _matrix.model;
	}

	glm::mat4 World::get_view_matrix() const
	{
		return _matrix.view;
	}

	glm::mat4 World::get_projection_matrix() const
	{
		return _matrix.projection;
	}

	//void World::bind_shadow_tex()
	//{
	//	_shadowMap.bind_shadow_tex();

	//}

	//void World::unbind_shadow_tex()
	//{
	//	_shadowMap.unbind_shadow_tex();
	//}

	void World::set_model_matrix(const glm::mat4& mat)
	{
		_matrix.model = mat;
	}

	void World::set_view_matrix(const glm::mat4& mat)
	{
		_matrix.view = mat;
	}

	void World::set_projection_matrix(const glm::mat4& mat)
	{
		_matrix.projection = mat;
	}

	void World::update_model_matrix(const glm::mat4& mat)
	{
		_matrix.model = mat;
		_directionalLight.link_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));
	}

	void World::set_directional_light(tools::DirectionalLight& light)
	{
		_directionalLight = std::move(light);
		if (_debug)
		{
			std::cout << "Directional light set in World." << std::endl;
		}
	}

	void World::bind_light()
	{
		_directionalLight.bind();
	}

	void World::update_mv_matrices(const glm::mat4& model)
	{
		_matrix.model = model;
		_matrix.view = _camera.get_view();
		_directionalLight.link_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));
		_directionalLight.link_camera_pos(_camera.get_position());
	}

	void World::update_mv_matrices_and_link(glInit::GLProgram& program)
	{
		update_mv_matrices();

		program.link_projection_matrix(_matrix.projection);
		program.link_model_matrix(_matrix.model);
		program.link_view_matrix(_matrix.view);
	}

	//void World::run_shadow_pass()
	//{
	//	_shadowMap.run_shadow_map_pass(_meshes);
	//}

	void World::render_meshes(glInit::GLProgram& program)
	{
		for (auto& mesh : _meshes)
		{
			_matrix.model = mesh->get_model_matrix();
			program.link_model_matrix(_matrix.model);
			mesh->render();
		}
	}
	
	void World::unbind_light()
	{
		_directionalLight.unbind();
	}

	void World::update_camera_pos(const glm::vec3& camPos)
	{
		_directionalLight.link_camera_pos(camPos);
	}


	void World::init_quaternion_camera(tools::Window& window)
	{
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

		_camera.init(cameraBundlePersp);

		_camera.set_commands_to_window(window);
		/*
		auto keys = tools::KeyUsageRegistry::get_instance().a_to_z_keys_in_use(); //Go on from here~

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
		*/
		_matrix.projection = _camera.get_projection();
		_matrix.view = _camera.get_view();
		_matrix.model = glm::mat4(1.0f);
	}


	void World::add_mesh(std::shared_ptr<glUtil::Mesh> mesh)
	{
		mesh->add_gravity(_gravity.get_acceleration());
		_meshes.push_back(mesh);
		if (_debug)
		{
			std::cout << "Mesh added to World." << std::endl;
		}
	}

	void World::add_meshes(std::vector<std::shared_ptr<glUtil::Mesh>>& mesh)
	{
		for (auto& m : mesh)
		{
			m->add_gravity(_gravity.get_acceleration());
		}

		_meshes.insert(_meshes.end(), mesh.begin(), mesh.end());
		if (_debug)
		{
			std::cout << "Meshes added to World." << std::endl;
		}
	}

	void World::init()
	{

	}

	glm::vec3 World::get_acc_due_to_gravity() const
	{
		return _gravity.get_acceleration();
	}


	World::~World() = default;

}




//Untested Code that belongs in the ctor of the World(glInit::GLProgram& program, tools::Window& window, bool debug)

		//glUtil::ShadowMapBundle shadowMapBundle;
		//
		//shadowMapBundle.lightDir = dirLightBundle.direction;
		//shadowMapBundle.textureNumber = glUtil::TextureUnit::TEX0;
		//shadowMapBundle.shadowWidth =  1024;
		//shadowMapBundle.shadowHeight = 1024;
		//shadowMapBundle.pOriginalHeight = (const unsigned int *)window.get_buffer_height_p();
		//shadowMapBundle.pOriginalWidth = (const unsigned int *)window.get_buffer_width_p();
		//shadowMapBundle.sceneDepthSize = 10.0f;
		//shadowMapBundle.sceneSize = 10.0f;
		//shadowMapBundle.worldUp;
		//
		//Config& config = Config::instance();
		//
		//shadowMapBundle.vertShadowShaderPath = config.get_vert_shadow_shader_path();
		//shadowMapBundle.fragShadowShaderPath = config.get_frag_shadow_shader_path();
		//
		//_shadowMap.init(shadowMapBundle, true);
		//
		//_shadowMap.set_shadow_map_loc(program.add_uniform("tShadowMap"));
