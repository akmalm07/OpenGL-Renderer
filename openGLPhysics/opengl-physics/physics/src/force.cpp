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

	Force Force::operator+(const Force& vec)
	{
		return ;
	}

	Force Force::operator+(const Force& vec)
	{
		return (_force * _mass) + (vec._force * vec._mass);
	}

	void Force::operator+=(const glm::vec3& vec)
	{
		_force += vec;
	}

	Force Force::operator-(const Force& vec) const
	{
		return Force();
	}

	void Force::set_force(const glm::vec3& force)
	{
		_force = force;
	}
	
}

