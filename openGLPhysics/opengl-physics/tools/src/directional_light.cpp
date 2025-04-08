#include "headers.h"
#include "tools/include/directional_light.h"


namespace tools
{


	std::vector<glm::vec3> calculate_vertex_normal(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices)
	{
		std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f));

		// Go through each triangle
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			unsigned int i0 = indices[i];
			unsigned int i1 = indices[i + 1];
			unsigned int i2 = indices[i + 2];

			glm::vec3 v0 = vertices[i0];
			glm::vec3 v1 = vertices[i1];
			glm::vec3 v2 = vertices[i2];

			// Compute face normal
			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;
			glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));

			// Add the face normal to each vertex normal
			normals[i0] += faceNormal;
			normals[i1] += faceNormal;
			normals[i2] += faceNormal;
		}

		// Normalize the vertex normals
		for (auto& normal : normals)
		{
			normal = glm::normalize(normal);
		}

		return normals;
	}

	std::vector<glType::Vertex> calculate_vertex_normals(const std::vector<glType::Vertex>& vertices, const std::vector<glType::Index>& indices)
	{
		const size_t vertex_count = vertices.size() / 6;
		std::vector<glm::vec3> positions(vertex_count);
		std::vector<glm::vec3> colors(vertex_count);
		std::vector<glm::vec3> normals(vertex_count, glm::vec3(0.0f));
		std::vector<glType::Vertex> result;

		// Step 1: Split positions and colors
		for (size_t i = 0; i < vertex_count; ++i)
		{
			positions[i] = glm::vec3(
				vertices[i * 6 + 0],
				vertices[i * 6 + 1],
				vertices[i * 6 + 2]
			);
			colors[i] = glm::vec3(
				vertices[i * 6 + 3],
				vertices[i * 6 + 4],
				vertices[i * 6 + 5]
			);
		}

		// Step 2: Calculate normals
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			glType::Index i0 = indices[i];
			glType::Index i1 = indices[i + 1];
			glType::Index i2 = indices[i + 2];

			const glm::vec3& v0 = positions[i0];
			const glm::vec3& v1 = positions[i1];
			const glm::vec3& v2 = positions[i2];

			glm::vec3 edge1 = v1 - v0;
			glm::vec3 edge2 = v2 - v0;
			glm::vec3 face_normal = glm::normalize(glm::cross(edge1, edge2));

			normals[i0] += face_normal;
			normals[i1] += face_normal;
			normals[i2] += face_normal;
		}

		// Step 3: Interleave everything back into result vector
		for (size_t i = 0; i < vertex_count; ++i)
		{
			glm::vec3 normal = glm::normalize(normals[i]);

			result.push_back(positions[i].x);
			result.push_back(positions[i].y);
			result.push_back(positions[i].z);

			result.push_back(colors[i].r);
			result.push_back(colors[i].g);
			result.push_back(colors[i].b);

			result.push_back(normal.x);
			result.push_back(normal.y);
			result.push_back(normal.z);
		}

		return result;
	}


	DirectionalLight::DirectionalLight(unsigned int programID, bool debug)
	{
		clamp_light_values();
		_uniformBuffer = debug;
		_uniformBuffer.init(programID, "LightData", 0, true);
		update_uniform_buffer();
	}

	DirectionalLight::DirectionalLight(const glm::vec3& dir, const glm::vec3& col, unsigned int programID, bool debug)
	{
		_light.direction = dir;
		_light.color = col;
	   
		clamp_light_values();
		_uniformBuffer = debug;
		_uniformBuffer.init(programID, "LightData", 0, true);
		update_uniform_buffer();
	}

	DirectionalLight::DirectionalLight(const DirectionalLightBundle& dir, unsigned int programID, bool debug)
	{
		_light = dir;

		clamp_light_values();
		_uniformBuffer = debug;
		_uniformBuffer.init(programID, "LightData", 0, true);
		update_uniform_buffer();
	}

	DirectionalLight::DirectionalLight(DirectionalLight&& other) noexcept = default;

	DirectionalLight& DirectionalLight::operator=(DirectionalLight&& other) noexcept = default;

	void DirectionalLight::set_amb_intensity(float val)
	{
		_light.ambInstensity = glm::clamp(val, 0.0f, 1.0f);
		_uniformBuffer.update_data(val, "ambIntensity");
	}

	void DirectionalLight::set_diff_intensity(float val)
	{
		_light.diffInstensity = glm::clamp(val, 0.0f, 1.0f);
		_uniformBuffer.update_data(val, "diffIntensity");
	}

	void DirectionalLight::set_spec_intensity(float val)
	{
		_light.specInstensity = glm::clamp(val, 0.0f, 1.0f);
		_uniformBuffer.update_data(val, "specIntensity");
	}


	void DirectionalLight::bind() const
	{
		_uniformBuffer.bind(); 
	}

	void DirectionalLight::unbind() const
	{
		_uniformBuffer.unbind();
	}


	void DirectionalLight::set_direction(const glm::vec3& dir)
	{
		_light.direction = dir;
		update_uniform_buffer();
	}

	void DirectionalLight::set_cam_pos_loc(unsigned int loc)
	{
		_cameraPosLoc = loc;
	}

	void DirectionalLight::set_normal_mat_loc(unsigned int loc)
	{
		_normalMatrixLoc = loc;
	}

	void DirectionalLight::set_cam_pos(const glm::vec3& val)
	{
		_cameraPos = val;
	}

	void DirectionalLight::set_normal_mat(const glm::mat3& val)
	{
		_normalMatrix = val;
	}

	void DirectionalLight::link_camera_pos(const glm::vec3& camPos)
	{
		_cameraPos = camPos;
		if (_cameraPosLoc != -1)
		{
			glUniform3fv(_cameraPosLoc, 1, glm::value_ptr(_cameraPos));
		}
		else
		{
			std::cerr << "Warning: uCameraPos location not set correctly." << std::endl;
		}
	}

	void DirectionalLight::link_normal_mat(const glm::mat3& val)
	{
		_normalMatrix = val;
		if (_normalMatrixLoc != 0)
		{
			glUniformMatrix3fv(_normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
		}
		else
		{
			std::cerr << "Warning: NormalMatrix location not set correctly." << std::endl;
		}
	}

	void DirectionalLight::link_camera_pos()
	{
		if (_cameraPosLoc != -1)
		{
			glUniform3fv(_cameraPosLoc, 1, glm::value_ptr(_cameraPos));
		}
		else
		{
			std::cerr << "Warning: uCameraPos location not set correctly." << std::endl;
		}
	}

	void DirectionalLight::link_normal_mat()
	{
		if (_normalMatrixLoc != 0)
		{
			glUniformMatrix3fv(_normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
		}
		else
		{
			std::cerr << "Warning: NormalMatrix location not set correctly." << std::endl;
		}
	}



	void DirectionalLight::set_color(const glm::vec3& col)
	{
		_light.color = col;
		_uniformBuffer.update_data(_light.color, "lightColor");
	}

	void DirectionalLight::set_ambient(const glm::vec3& amb)
	{
		_light.ambient = amb;
		_uniformBuffer.update_data(_light.ambient, "lightAmbient");
	}

	void DirectionalLight::set_diffuse(const glm::vec3& diff)
	{
		_light.diffuse = diff;
		_uniformBuffer.update_data(_light.diffuse, "lightDiffuse");
	}

	void DirectionalLight::set_specular(const glm::vec3& spec)
	{
		_light.specular = spec;
		_uniformBuffer.update_data(_light.specular, "lightSpecular");
	}

	void DirectionalLight::update_uniform_buffer()
	{
		_uniformBuffer.update_data(_light.ambient, "lightAmbient");
		_uniformBuffer.update_data(_light.diffuse, "lightDiffuse");
		_uniformBuffer.update_data(_light.specular, "lightSpecular");
		_uniformBuffer.update_data(_light.ambInstensity, "ambIntensity");
		_uniformBuffer.update_data(_light.diffInstensity, "diffIntensity");
		_uniformBuffer.update_data(_light.specInstensity, "specIntensity");
		_uniformBuffer.update_data(_light.direction, "lightDir");
		_uniformBuffer.update_data(_light.color, "lightColor");

		if (_cameraPosLoc != 0)
		{
			// Update camera position if location is set
			glUniform3fv(_cameraPosLoc, 1, glm::value_ptr(_cameraPos));
		}
		if (_normalMatrixLoc != 0)
		{
			glUniformMatrix3fv(_normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(_normalMatrix));
		}
	}

	const glUtil::UniformBuffer& DirectionalLight::get_uniform_buffer() const
	{
		return _uniformBuffer;
	}

	void DirectionalLight::clamp_light_values()
	{
		_light.ambInstensity = glm::clamp(_light.ambInstensity, 0.0f, 1.0f);
		_light.diffInstensity = glm::clamp(_light.diffInstensity, 0.0f, 1.0f);
		_light.specInstensity = glm::clamp(_light.specInstensity, 0.0f, 1.0f);
	}
}

/*void DirectionalLight::set_intencity_loc(unsigned int loc)
{
	intensityLoc = loc;
	if (intensityLoc != 0)
	{
		glUniform1f(intensityLoc, intensity);
	}
	else
	{
		std::cerr << "Warning: Intensity location not set correctly." << std::endl;
	}
}

void DirectionalLight::set_intencity(float val)
{
	glm::clamp(val, 0.0f, 1.0f);
	intensity = val;

void DirectionalLight::link_intencity(float val)
{
	if (intensityLoc != 0)
	{
		glUniform1f(intensityLoc, val);
	}
	else
	{
		std::cerr << "Warning: Intensity location not set correctly." << std::endl;
	}
}
}*/
