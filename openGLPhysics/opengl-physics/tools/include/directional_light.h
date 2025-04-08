#pragma once

#include "config.h"

#include "glUtil/include/uniform_buffer.h"


namespace tools
{

	std::vector<glm::vec3> calculate_vertex_normal(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
	std::vector<glType::Vertex> calculate_vertex_normals(const std::vector<glType::Vertex>& vertices, const std::vector<glType::Index>& indices);

	struct DirectionalLightBundle
	{
		glm::vec3 direction = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

		float ambInstensity =  0.4f;
		float diffInstensity = 1.0f;
		float specInstensity = 1.0f;
	};

	class DirectionalLight
	{
	public:
		DirectionalLight(unsigned int programID, bool debug);

		DirectionalLight(const glm::vec3& dir, const glm::vec3& col, unsigned int programID, bool debug);

		DirectionalLight(const DirectionalLightBundle& dir, unsigned int programID, bool debug);

		DirectionalLight(DirectionalLight&& other) noexcept; 
		DirectionalLight& operator=(DirectionalLight&& other) noexcept;

		void set_amb_intensity(float val);
		void set_diff_intensity(float val);
		void set_spec_intensity(float val);

		void bind() const; 
		void unbind() const; 

		//void set_intencity_loc(unsigned int loc);
		//void set_intencity(float val);

		void set_cam_pos_loc(unsigned int loc);
		void set_normal_mat_loc(unsigned int loc);

		void set_cam_pos(const glm::vec3& val);
		void set_normal_mat(const glm::mat3& val);

		void link_camera_pos(const glm::vec3& camPos);
		void link_normal_mat(const glm::mat3& val);
		void link_camera_pos();
		void link_normal_mat();

		void set_direction(const glm::vec3& dir);
		void set_color(const glm::vec3& col);
		void set_ambient(const glm::vec3& amb);
		void set_diffuse(const glm::vec3& diff);
		void set_specular(const glm::vec3& spec);

		void update_uniform_buffer();

		const glUtil::UniformBuffer& get_uniform_buffer() const;

	private:

		DirectionalLightBundle _light;

		glm::vec3 _cameraPos = glm::vec3(0.0f);
		unsigned int _cameraPosLoc = 0; 

		glm::mat3 _normalMatrix = glm::mat3(1.0f);
		unsigned int _normalMatrixLoc = 0;

		glUtil::UniformBuffer _uniformBuffer;

	private:

		void clamp_light_values();
	};


}
