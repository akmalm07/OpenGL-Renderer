#pragma once

#include "config.h"
#include "tools/include/stride.h"


namespace glUtil
{
	struct Transform
	{
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
	};

	struct MeshBundle
	{

		const glType::Vertex* pVertices;
		const glType::Index* pIndices;

		FullStride fullStride = FullStride::NONE;

		ArrayBufferLayout* pLayout1 = nullptr;
		ArrayBufferLayout* pLayout2 = nullptr;
		ArrayBufferLayout* pLayout3 = nullptr;
		ArrayBufferLayout* pLayout4 = nullptr;

		Transform transform;

		size_t vertexCount = 0;
		size_t indexCount = 0;

		bool indexed = true;

	};
}

	//struct MoveibleMeshBundle : public glUtil::MeshBundle
	//{
	//    glm::vec3 gravity = glm::vec3(0.0f, 0.0f, 0.0f);

	//    glm::vec3 rateOfChange = glm::vec3(0.0f, 0.0f, 0.0f);

	//    float mass = 1.0f;

	//    bool startTime = true;

	//    bool gravityAffected = true;
	//};