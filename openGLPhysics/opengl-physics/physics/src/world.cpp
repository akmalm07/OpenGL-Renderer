#include "headers.h"

#include "physics\include\world.h"
#include "tools\include\key_usage_registry.h"

namespace tools
{

	World::World(glInit::GLProgram& program, tools::Window& window, bool debug)
	{

		tools::DirectionalLightBundle dirLightBundle;
		//dirLightBundle.direction = glm::normalize(glm::vec3(0.5f, 1.0f, 1.0f));
		dirLightBundle.color = glm::vec3(1.0f);

		_directionalLight = tools::DirectionalLight(dirLightBundle, program.get_id(), debug);

		_directionalLight.set_normal_mat(glm::mat3(1.0f));

		_directionalLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		_directionalLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));
		
		window.world_visitor(*this); // Camera is set here

		_debug = debug;

		if (_debug)
		{
			std::cout << "World initialized with directional light with program ID: " << program.get_id() << std::endl;
		}
	}

	//void World::add_scene(std::shared_ptr<tools::Scene> scene)
	//{
	//	_scene = scene;
	//	
	//	if (_debug)
	//	{
	//		std::cout << "Scene added to World." << std::endl;
	//	}
	//}

	glm::mat4 World::get_view_matrix() const
	{
		return _camera->get_view();
	}

	glm::mat4 World::get_projection_matrix() const
	{
		return _camera->get_projection();
	}

	//void World::bind_shadow_tex()
	//{
	//	_shadowMap.bind_shadow_tex();

	//}

	//void World::unbind_shadow_tex()
	//{
	//	_shadowMap.unbind_shadow_tex();
	//}

	void World::set_view_matrix(const glm::mat4& mat)
	{
		_camera->set_view(mat);
	}

	void World::set_projection_matrix(const glm::mat4& mat)
	{
		_camera->set_projection(mat);
	}

	void World::update_normal_matrix(const glm::mat3& mat)
	{
		_directionalLight.link_normal_mat(mat);
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

		_directionalLight.set_normal_mat(glm::mat3(1.0f));
		_directionalLight.set_cam_pos(_camera->get_position());

	}

	void World::link_and_update_mv_matrices(const glInit::GLProgram& program)
	{
		_camera->update_matricies(program);

		_directionalLight.link_camera_pos(_camera->get_position());
	}

	void World::update(const glInit::GLProgram& program)
	{
		link_and_update_mv_matrices(program);

		render_entities(program);
	}

	//void World::run_shadow_pass()
	//{
	//	_shadowMap.run_shadow_map_pass(_meshes);
	//}

	void World::render_entities(const glInit::GLProgram& program)
	{
		const auto& meshes = ComponentRegistry<glUtil::Mesh>::get_instance();
		const auto& texes = ComponentRegistry<glUtil::Texture>::get_instance();
		//const auto& materials = ComponentRegistry<glUtil::Material>::get_instance();

		for (const auto& entity : _entities.get_entities())
		{
			const auto msh = meshes.get_component_or_null(entity);
			const auto tex = texes.get_component_or_null(entity);

			if (tex)
			{
				tex->bind();
			}

			if (msh)
			{
				glm::mat4 modelMatrix = msh->get_model_matrix();
				program.link_model_matrix(modelMatrix);
				_directionalLight.link_normal_mat(glm::inverse(glm::transpose(modelMatrix)));
				msh->render();
			}

			if (tex)
			{
				tex->unbind();
			}
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


	void World::set_camera(tools::BaseCamera* camera)
	{
		_camera = camera;
	}
	
	glType::Entity World::add_entity(const std::string& name)
	{
		if (_debug)
		{
			std::cout << "Entity '" << name << "' added to World." << std::endl;
		}
		return _entities.register_entity(name);
	}

	glType::Entity World::get_entity(const std::string& name)
	{
		if (_debug)
		{
			std::cout << "Entity '" << name << "' retrieved from World." << std::endl;
		}
		return _entities.get_entity(name);
	}

	glm::vec3 World::get_acc_due_to_gravity() const
	{
		return _gravity;
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
