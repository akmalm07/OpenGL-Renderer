#pragma once

#include "config.h"

namespace tools
{
	struct DirectionalLightBundle
	{
		glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

		float ambIntensity = 0.3f;
		float diffIntensity = 0.5f;
		float specIntensity = 0.9f;
	};
}