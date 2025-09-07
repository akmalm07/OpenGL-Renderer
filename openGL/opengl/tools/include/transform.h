#pragma once

#include "config.h"

namespace tools
{
	struct Transform : public glType::Component<Transform>
	{
	public:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

		Transform() = default;

		Transform(const glm::vec3& pos, const glm::quat& rot, const glm::vec3& scl)
			: position(pos), rotation(rot), scale(scl) {
		}

		Transform(Transform&&) noexcept = default;
		Transform(const Transform&) noexcept = default;

		void communicate_impl(glType::Entity entity) {}
		
		glm::mat4 get_model_matrix() const;
	private:

	};

	Transform construct_default_transform();

}