#include "headers.h"
#include "tools/include/base_camera.h"
#include "glInit/include/program.h"


namespace tools
{

	void BaseCamera::update_matricies(const glInit::GLProgram& program) const
	{

		program.link_projection_matrix(_projection);

		program.link_view_matrix(_view);
		
	}
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

	void BaseCamera::set_view(const glm::mat4& mat)
	{
		_view = mat;
	}

	void BaseCamera::set_projection(const glm::mat4& mat)
	{
		_projection = mat;
	}


}
