#pragma once

#include "config.h"
#include "glUtil/include/texture.h"

namespace glUtil
{

	struct ShadowMapBundle
	{
		std::vector<glm::vec3> lightPos = { glm::vec3(0.0f, 0.0f, 0.0f) };
		glm::vec3 lightDir = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);

		unsigned int shadowHeight = 600.0f;
		unsigned int shadowWidth = 800.0f;
		const unsigned int* pOriginalWidth = nullptr;
		const unsigned int* pOriginalHeight = nullptr;

		std::filesystem::path vertShadowShaderPath;
		std::filesystem::path fragShadowShaderPath;

		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float sceneSize = 1.0f;
		float sceneDepthSize = 1.0f;

		TextureUnit textureNumber = TextureUnit::TEX0;
	};

}