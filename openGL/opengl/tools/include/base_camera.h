#pragma once


#include "config.h"
#include "tools/include/keys.h"

namespace glInit
{
	class GLProgram;
}

namespace tools
{
	class BaseCamera
	{
	public:

		BaseCamera() = default;

		void update_matricies(const glInit::GLProgram& program) const;

		void set_view(const glm::mat4& mat);

		void set_projection(const glm::mat4& mat);


		glm::vec3 get_position() const;

		glm::mat4 get_view() const;

		glm::mat4 get_projection() const;

		virtual void event_key(Direction dir, float deltaTime) = 0;
		virtual void event_key(float deltaTime, float xMove, float yMove) = 0;
		virtual void event_scroll(float yOffset) = 0;

	protected:

		glm::mat4 _projection;
		glm::mat4 _view;
		glm::vec3 _position;

	};

}
