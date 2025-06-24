#pragma once

#include "config.h"

#include "physics/include/acceleration.h"


namespace physics
{
	class Force : public Moveible
	{
	public:

		Force() = default;

		Force(const glm::vec3& position, const glm::vec3& force);

		Force(const glm::vec3& position, const glm::vec3& force, const tools::Timer& timer);
		
		void set_force(const glm::vec3& force);

		glm::vec3 get_force() const;

		glm::vec3 get_distance() const;

		glm::vec3 get_current_momentum() const;

		glm::vec3 operator+(const glm::vec3& vec);
	
		glm::vec3 operator+(const Force& vec);

		void operator+=(const glm::vec3& vec);

	private:
		float _mass = 1.0f;
		Acceleration _force;
	};


}

