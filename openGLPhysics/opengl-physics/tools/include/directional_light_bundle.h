#pragma once

#include "config.h"

namespace tools
{
	struct DirectionalLightBundle
	{
		glm::vec3 direction = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);

		float ambInstensity = 0.3f;
		float diffInstensity = 0.5f;
		float specInstensity = 0.7f;
	};
}