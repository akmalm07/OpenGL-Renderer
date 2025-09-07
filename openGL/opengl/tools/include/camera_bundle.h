#pragma once

#include "config.h"

namespace tools
{

	struct _COMMON_FATHER_CAMERA_
	{
		float nearZ = 0.1f;
		float farZ = 100.0f;
		float speed = 1.0f;
		float turnSpeed = 1.0f;

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 startPYR = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	};

	struct CameraBundlePerspective : _COMMON_FATHER_CAMERA_
	{
		float fov = 45.0f;
		float aspectRatio;
		float zoomSpeed = 1.0f; // Speed of zooming in and out

	};

	struct CameraBundleOrthographic : _COMMON_FATHER_CAMERA_
	{
		float left;
		float right;
		float bottom;
		float top;

	};
}