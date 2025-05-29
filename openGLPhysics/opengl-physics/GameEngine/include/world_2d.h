#pragma once

#include "config.h"

#include "tools/include/scene.h"
#include "glInit/include/program.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/window.h"
#include "physics/include/acceleration.h"

#include "GameEngine/include/game_object.h"

namespace GameEngine
{
	class World2D
	{
	public:
		World2D();

		World2D(glInit::GLProgram& program, tools::Window& window, bool debug);
		
		World2D(glInit::GLProgram& program, tools::Window& window, const glm::vec2& gravity, bool debug);

		World2D(World2D&&) noexcept = default;
		World2D& operator=(World2D&&) noexcept = default;

		World2D(const World2D&) noexcept = delete;
		World2D& operator=(const World2D&) noexcept = delete;

		void add_object(std::shared_ptr<GameObject> object);

		void add_objects(std::vector<std::shared_ptr<GameObject>>& object);

		glm::mat4 get_model_matrix();
		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		void set_model_matrix(const glm::mat4& mat);
		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);

		void update_model_matrix(const glm::mat4& mat);

		void update_mv_matrices(const glm::mat4& model = glm::mat4(1.0f));
		void update_mv_matrices_and_link(glInit::GLProgram& program);

		void render_objects(float deltaTime);

		void init();

		glm::vec3 get_acc_due_to_gravity() const;

		void add_scene(std::shared_ptr<tools::Scene> scene);

		~World2D();

	private:
		std::shared_ptr<tools::Scene> _scene;

		std::vector<std::shared_ptr<GameObject>> _objects;
		std::vector<std::shared_ptr<glUtil::Texture>> _textures;


		glm::vec3 _gravity = glm::vec3(0.0f, -9.814f, 0.0f);

		tools::QuaternionCamera _camera;

		struct Matrix
		{
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
		} _matrix;

		bool _debug;

	private:
		void init_quaternion_camera(tools::Window& window);
	};

}





//TODO: Add directional light for 2D scene
//#include "tools/include/directional_light.h"
//public:
		//void set_directional_light(tools::DirectionalLight& light);
		//void bind_light();
		//void unbind_light();
		//void update_camera_pos(const glm::vec2& camPos);
//private:
		//tools::DirectionalLight _directionalLight;