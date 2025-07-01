#pragma once


#include "config.h"
#include "tools/include/keys.h"

namespace tools
{
	class BaseCamera
	{
	public:

		BaseCamera() = default;

		glm::vec3 get_position() const;

		glm::mat4 get_view() const;

		glm::mat4 get_projection() const;

		virtual bool event_key(Direction dir, double deltaTime) = 0;
		virtual bool event_key(double deltaTime, float xMove, float yMove) = 0;
		virtual bool event_scroll(float yOffset) = 0;

	protected:

		glm::mat4 _projection;
		glm::mat4 _view;
		glm::vec3 _position;

	};

}
