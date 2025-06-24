#include "headers.h"
#include "physics/include/force.h"


namespace physics
{

	Force::Force(const glm::vec3& position, const glm::vec3& force)
		: _force(position, force)
	{
	}
	Force::Force(const glm::vec3& position, const glm::vec3& force, const tools::Timer& timer)
		: _force(position, force, timer)
	{
	}


    glm::vec3 Force::get_force() const
    {
        return _force * _mass;
    }

	glm::vec3 Force::get_distance() const
	{
		return _force.get_distance();
	}

	glm::vec3 Force::get_current_momentum() const
	{
		return _force.get_current_volocity() * _mass;
	}

	glm::vec3 Force::operator+(const glm::vec3& vec)
	{
		return _force.get_acceleration() + vec;
	}

	glm::vec3 Force::operator+(const Force& vec)
	{
		return _force.get_acceleration() + vec._force.get_acceleration();
	}

	void Force::operator+=(const glm::vec3& vec)
	{
		_force += vec;
	}

	void Force::set_force(const glm::vec3& force)
	{
		_force = force;
	}
	
}
