#include "headers.h"

#include "tools/include/transform.h"

namespace tools
{
	glm::mat4 Transform::get_model_matrix() const
	{
		glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 rotationMat = glm::toMat4(rotation);
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		return translationMat * rotationMat * scaleMat;
	}


	Transform construct_default_transform()
	{
		return Transform();
	}

}