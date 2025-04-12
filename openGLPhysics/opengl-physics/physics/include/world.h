#pragma once

#include "config.h"
#include "tools/include/scene.h"
#include "glInit/include/program.h"
#include "tools/include/directional_light.h"
#include "physics/include/acceleration.h"


namespace physics
{

	class World
	{
	public:
		World();

		World(glInit::GLProgram& program, bool debug);

		World(World&&) noexcept = default;
		World& operator=(World&&) noexcept = default;

		World(const World&) noexcept = delete;
		World& operator=(const World&) noexcept = delete;

		void add_mesh(std::shared_ptr<glUtil::Mesh> mesh);
		
		void add_meshes(std::vector<std::shared_ptr<glUtil::Mesh>>& mesh);

		glm::mat4 get_model_matrix();
		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		void set_model_matrix(const glm::mat4& mat);
		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);

		void set_directional_light(tools::DirectionalLight& light);

		void bind_light();

		void unbind_light();

		void update_camera_pos(const glm::vec3& camPos);

		void init();

		glm::vec3 get_acc_due_to_gravity() const;
		
		void add_scene(std::shared_ptr<tools::Scene> scene);

		~World();

	private:
		std::shared_ptr<tools::Scene> _scene;

		std::vector<std::shared_ptr<glUtil::Mesh>> _meshes;
		std::vector<std::shared_ptr<glUtil::Texture>> _textures;

		tools::DirectionalLight _directionalLight;

		Acceleration _gravity = { glm::vec3(0.0f, 0.0f, 0.0f), false };
		

		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		} _matrix;
	
		bool _debug;
	};

}