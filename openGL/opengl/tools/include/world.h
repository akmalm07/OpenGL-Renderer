#pragma once

#include "config.h"

#include "tools/include/directional_light.h"
#include "tools/include/entity_registry.h"
#include "tools/include/base_camera.h"

namespace glInit
{
	class GLProgram;
}


namespace tools
{
	class Window;

	class World
	{
	public:
		World();

		World(glInit::GLProgram& program, tools::Window& window);

		World(World&&) noexcept = default;
		World& operator=(World&&) noexcept = default;

		World(const World&) noexcept = delete;
		World& operator=(const World&) noexcept = delete;

		glType::Entity add_entity(const std::string& name);

		glType::Entity get_entity(const std::string& name);

		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);
		void update_normal_matrix(const glm::mat3& mat);

		void set_directional_light(tools::DirectionalLight& light);

		void bind_light();

		void link_and_update_mv_matrices(const glInit::GLProgram& program);

		void update(const glInit::GLProgram& program, float deltaTime);

		void render_entities(const glInit::GLProgram& program, float deltaTime);

		void unbind_light();

		void update_camera_pos(const glm::vec3& camPos);

		~World();

	private:

		EntityRegistry _entities;

		tools::DirectionalLight _directionalLight;

		std::shared_ptr<BaseCamera> _camera;

	};

}


