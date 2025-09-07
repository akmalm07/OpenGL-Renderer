#include "headers.h"
#include "tools/include/world.h"
#include "glInit/include/program.h"
#include "tools/include/window.h"
#include "glUtil/include/mesh.h"
#include "glUtil/include/texture.h"
#include "tools/include/component_registry.h"
#include "tools/include/transform.h"



namespace tools
{

	World::World() = default;


	World::World(glInit::GLProgram& program, tools::Window& window)
	{

		tools::DirectionalLightBundle dirLightBundle;

		_directionalLight = tools::DirectionalLight(dirLightBundle, program.get_id());

		_directionalLight.set_normal_mat(glm::mat3(1.0f));

		_directionalLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

		_directionalLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));

		_camera = window.get_camera_shared();

		if constexpr (DEBUG_MODE)
		{
			std::cout << "World initialized with directional light with program ID: " << program.get_id() << std::endl;
		}
	}


	glm::mat4 World::get_view_matrix() const
	{
		return _camera->get_view();
	}

	glm::mat4 World::get_projection_matrix() const
	{
		return _camera->get_projection();
	}


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
		if constexpr (DEBUG_MODE)
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

	void World::update(const glInit::GLProgram& program, float deltaTime)
	{
		link_and_update_mv_matrices(program);

		render_entities(program, deltaTime);
	}


	void World::render_entities(const glInit::GLProgram& program, float deltaTime)
	{
		const auto& meshes = ComponentRegistry<glUtil::Mesh>::get_instance();
		const auto& texes = ComponentRegistry<glUtil::Texture>::get_instance();
		const auto& transes = ComponentRegistry<tools::Transform>::get_instance();


		//const auto& materials = ComponentRegistry<glUtil::Material>::get_instance();

		for (const auto& entity : _entities.get_entities())
		{
			const auto msh = meshes.get_component_or_null(entity);
			const auto tex = texes.get_component_or_null(entity);
			const auto trans = transes.get_component_or_null(entity);

			if (tex)
			{
				tex->bind();
			}

			if (msh)
			{
				msh->render();
				glm::mat4 modelMatrix = msh->get_model_matrix();
				program.link_model_matrix(modelMatrix);
				_directionalLight.link_normal_mat(glm::inverse(glm::transpose(modelMatrix)));

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

	glType::Entity World::add_entity(const std::string& name)
	{
		if constexpr (DEBUG_MODE)
		{
			std::cout << "Entity '" << name << "' added to World." << std::endl;
		}
		return _entities.register_entity(name);
	}

	glType::Entity World::get_entity(const std::string& name)
	{
		if constexpr (DEBUG_MODE)
		{
			std::cout << "Entity '" << name << "' retrieved from World." << std::endl;
		}
		return _entities.get_entity(name);
	}

	World::~World() = default;

}





