#include "headers.h"
#include "tools/include/base_camera.h"

namespace tools
{
	glm::vec3 BaseCamera::get_position() const
	{
		return _position;
	}

	glm::mat4 BaseCamera::get_view() const
	{
		return _view;
	}

	glm::mat4 BaseCamera::get_projection() const
	{
		return _projection;
	}




}
