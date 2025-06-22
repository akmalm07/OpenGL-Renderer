#pragma once

#include "config.h"
#include "tools/include/scene.h"
#include "glInit/include/program.h"
#include "tools/include/directional_light.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/window.h"
#include "physics/include/acceleration.h"
//#include "glUtil/include/shadow_map.h"


namespace physics
{

	class World
	{
	public:
		World();

		World(glInit::GLProgram& program, tools::Window& window, bool debug);

		World(World&&) noexcept = default;
		World& operator=(World&&) noexcept = default;

		World(const World&) noexcept = delete;
		World& operator=(const World&) noexcept = delete;

		void add_mesh(std::shared_ptr<glUtil::Mesh> mesh);
		
		void add_meshes(std::vector<std::shared_ptr<glUtil::Mesh>>& mesh);

		void set_camera(std::shared_ptr<tools::BaseCamera> camera);

		glm::mat4 get_model_matrix();
		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		//void bind_shadow_tex();
		//void unbind_shadow_tex();

		void set_model_matrix(const glm::mat4& mat);
		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);

		void update_model_matrix(const glm::mat4& mat);

		void set_directional_light(tools::DirectionalLight& light);

		void bind_light();

		void update_mv_matrices(const glm::mat4& model = glm::mat4(1.0f));
		void update_mv_matrices_and_link(glInit::GLProgram& program);

		void render_meshes(glInit::GLProgram& program);

		void unbind_light();

		void update_camera_pos(const glm::vec3& camPos);

		glm::vec3 get_acc_due_to_gravity() const;
		
		void add_scene(std::shared_ptr<tools::Scene> scene);

		~World();

		//void run_shadow_pass();
	private:
		std::shared_ptr<tools::Scene> _scene;

		std::vector<std::shared_ptr<glUtil::Mesh>> _meshes;
		std::vector<std::shared_ptr<glUtil::Texture>> _textures;

		tools::DirectionalLight _directionalLight;

		glm::vec3 _gravity = { 0.0f, -0.1f, 0.0f };
		
		std::shared_ptr<tools::BaseCamera> _camera;
		
		//glUtil::ShadowMap _shadowMap;


		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		} _matrix;
	
		bool _debug;
	};

}