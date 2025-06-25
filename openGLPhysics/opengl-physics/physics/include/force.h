#pragma once

#include "config.h"

#include "physics/include/acceleration.h"


namespace physics
{
	class Force 
	{
	public:

		Force() = default;

		Force(const glm::vec3& position, const glm::vec3& force, float mass);

		Force(const glm::vec3& position, const glm::vec3& force, float mass, const tools::Timer& timer);
		
		void set_force(const glm::vec3& force);

		glm::vec3 get_force() const;

		glm::vec3 get_distance() const;

		glm::vec3 get_current_momentum() const;

		Force operator+(const Force& vec);
	
		Force operator+(const Force& vec);

		void operator+=(const glm::vec3& vec);

		Force operator-(const Force& vec) const;

	private:

		float _mass = 1.0f;
		Acceleration _force; // This is internal force, so the mas must be removed. Mass belongs in physics Body, a seperate class
	};


}

