#pragma once

#include "config.h"
#include "tools/include/scene.h"
#include "glInit/include/program.h"
#include "tools/include/directional_light.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/window.h"
#include "tools/include/entity_registry.h"
#include "tools/include/component_registry.h"

#include "glUtil/include/mesh.h"
#include "glUtil/include/texture.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "physics/include/sphere_bound.h"
#include "physics/include/force.h"

//#include "glUtil/include/shadow_map.h"


namespace tools
{

	class World //NEW!!
	{
	public:
		World() = default;

		World(glInit::GLProgram& program, tools::Window& window, bool debug);

		World(World&&) noexcept = default;
		World& operator=(World&&) noexcept = default;

		World(const World&) noexcept = delete;
		World& operator=(const World&) noexcept = delete;

		glType::Entity add_entity(const std::string& name);

		glType::Entity get_entity(const std::string& name);

		void set_camera(tools::BaseCamera* camera);

		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		//void bind_shadow_tex();
		//void unbind_shadow_tex();

		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);
		void update_normal_matrix(const glm::mat3& mat);


		void set_directional_light(tools::DirectionalLight& light);

		void bind_light();

		void link_and_update_mv_matrices(const glInit::GLProgram& program);

		void update(const glInit::GLProgram& program);

		void render_entities(const glInit::GLProgram& program);

		void unbind_light();

		void update_camera_pos(const glm::vec3& camPos);

		glm::vec3 get_acc_due_to_gravity() const;
		
		//void add_scene(std::shared_ptr<tools::Scene> scene);

		~World();

		//void run_shadow_pass();
	private:
		//std::shared_ptr<tools::Scene> _scene;

		EntityRegistry _entities;
		
		tools::DirectionalLight _directionalLight;

		glm::vec3 _gravity = { 0.0f, -0.1f, 0.0f };
		
		tools::BaseCamera* _camera;
		
		//glUtil::ShadowMap _shadowMap;
	
		bool _debug;
	};

}