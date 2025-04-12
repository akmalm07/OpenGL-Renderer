#include "headers.h"

#include "physics\include\world.h"

namespace physics
{
	World::World() = default;


	World::World(glInit::GLProgram& program, bool debug)
	{

		tools::DirectionalLightBundle dirLightBundle;
		dirLightBundle.direction = glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f));
		dirLightBundle.color = glm::vec3(1.0f);

		_directionalLight = tools::DirectionalLight(dirLightBundle, program.get_id(), debug);

		_directionalLight.set_cam_pos_loc(program.add_uniform("uCameraPos"));

		_directionalLight.set_normal_mat_loc(program.add_uniform("uNormalMatrix"));

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
		_directionalLight.set_normal_mat(glm::transpose(glm::inverse(glm::mat3(_matrix.model))));
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
	
	void World::unbind_light()
	{
		_directionalLight.unbind();
	}

	void World::update_camera_pos(const glm::vec3& camPos)
	{
		_directionalLight.link_camera_pos(camPos);
	}

	World::~World() = default;


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

}

