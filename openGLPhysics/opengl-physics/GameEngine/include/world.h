#pragma once

#include "config.h"

#include "tools/include/scene.h"
#include "glInit/include/program.h"
#include "tools/include/directional_light.h"
#include "tools/include/quaternion_camera.h"
#include "tools/include/window.h"
#include "physics/include/acceleration.h"

#include "GameEngine/include/game_object.h"

//#include "glUtil/include/shadow_map.h"


namespace GameEngine
{

	class World
	{
	public:
		World();

		World(std::shared_ptr<glInit::GLProgram>& program, tools::Window& window, bool debug);
		
		World(std::shared_ptr<glInit::GLProgram>& program, tools::Window& window, const glm::vec3& gravity, bool debug);

		World(World&&) noexcept = default;
		World& operator=(World&&) noexcept = default;

		World(const World&) noexcept = delete;
		World& operator=(const World&) noexcept = delete;

		void add_object(std::shared_ptr<GameObject> obj, std::string_view tag = "unspecified");
		
		void add_objects(std::vector<std::shared_ptr<GameObject>>& mesh, std::string_view tag = "unspecified");

		glm::mat4 get_model_matrix();
		glm::mat4 get_view_matrix() const;
		glm::mat4 get_projection_matrix() const;

		//void bind_shadow_tex();
		//void unbind_shadow_tex();

		void update();

		void set_collision_response(const GameObject& me, const GameObject& other, std::function<void()> event); // EZ_FIND_ME_collision_response_feature

		void set_model_matrix(const glm::mat4& mat);
		void set_view_matrix(const glm::mat4& mat);
		void set_projection_matrix(const glm::mat4& mat);

		void update_model_matrix(const glm::mat4& mat);

		void set_directional_light(tools::DirectionalLight& light);

		void bind_light();

		void update_mv_matrices(const glm::mat4& model = glm::mat4(1.0f));
		void update_mv_matrices_and_link(glInit::GLProgram& program);

		void render_objects(float deltaTime);

		void render_objects(float deltaTime, std::string_view tag);

		void unbind_light();

		void update_camera_pos(const glm::vec3& camPos);
		
		void set_gravity(const glm::vec3& gravity);

		void init();

		glm::vec3 get_acc_due_to_gravity() const;
		
		void add_scene(std::shared_ptr<tools::Scene> scene);

		~World();

		//void run_shadow_pass();
	private:
		std::shared_ptr<tools::Scene> _scene;

		std::unordered_map <std::string_view, std::vector<std::shared_ptr<GameObject>> > _objects;
		std::vector<std::shared_ptr<glUtil::Texture>> _textures;

		tools::DirectionalLight _directionalLight;

		glm::vec3 _gravity = glm::vec3(0.0f, -9.814f, 0.0f);
		
		//glUtil::ShadowMap _shadowMap;

		std::shared_ptr<glInit::GLProgram> _program;

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