#pragma once

#include "config.h"

#include "glUtil/include/uniform_buffer.h"


namespace tools
{

	std::vector<glm::vec3> calculate_vertex_normal(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
	std::vector<glType::Vertex> calculate_vertex_normals(const std::vector<glType::Vertex>& vertices, const std::vector<glType::Index>& indices);

	struct DirectionalLightBundle
	{
		glm::vec3 direction; 
		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular; 
		DirectionalLightBundle()
			: direction(0.0f, -1.0f, 0.0f), color(1.0f, 1.0f, 1.0f),
			ambient(0.2f, 0.2f, 0.2f), diffuse(0.5f, 0.5f, 0.5f), specular(1.0f, 1.0f, 1.0f) {
		}
	};

	class DirectionalLight
	{
	public:
		DirectionalLight();

		DirectionalLight(const glm::vec3& dir, const glm::vec3& col);

		DirectionalLight(const DirectionalLightBundle& dir);

		DirectionalLight(DirectionalLight&& other) noexcept = default; 
		DirectionalLight& operator=(DirectionalLight&& other) noexcept = default;

		void bind() const; 
		void unbind() const; 

		void set_intencity_loc(unsigned int loc);
		void set_intencity(float val);

		void set_cam_pos_loc(unsigned int loc);
		void set_cam_pos(const glm::vec3& val);

		void link_camera_pos(const glm::vec3& camPos);
		void link_intencity(float val);

		void set_direction(const glm::vec3& dir);
		void set_color(const glm::vec3& col);
		void set_ambient(const glm::vec3& amb);
		void set_diffuse(const glm::vec3& diff);
		void set_specular(const glm::vec3& spec);



		void update_uniform_buffer();

		const glUtil::UniformBuffer& get_uniform_buffer() const;

	private:

		struct DirLight
		{
			glm::vec3 direction;
			glm::vec3 color;
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
		} _light;

		float intensity = 1.0f;
		unsigned int intensityLoc;

		glm::vec3 _cameraPos;
		unsigned int _cameraPosLoc = 0; 

		glUtil::UniformBuffer _uniformBuffer;
	};


}
