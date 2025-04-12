#pragma once

#include "config.h"

#include "tools/include/timer.h"



namespace physics
{
	class Acceleration
	{
	public:
		Acceleration();

		Acceleration(const glm::vec3& val);
		
		Acceleration(const glm::vec3& val, const tools::Timer& timer);
		
		void set_acceleration(const glm::vec3& val);
		
		void set_inital_vol(const glm::vec3& val);

		void start_time();

		double stop_time();
		
		void reset_time();

		glm::vec3 operator+(const glm::vec3& vec);

		void operator+=(const glm::vec3& vec);

		glm::vec3 get_acceleration() const;
		
		glm::vec3 get_intial_vol() const;

		double get_distance_x() const;
		
		double get_distance_y() const;

		double get_distance_z() const;

		glm::vec3 get_distance() const;

		glm::vec3 get_current_volocity() const;
		
		double get_current_volocity_x() const;
		double get_current_volocity_y() const;
		double get_current_volocity_z() const;

		double get_current_time() const;

	private:
		
		glm::vec3 _acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 _intialVol = glm::vec3(0.0f, 0.0f, 0.0f);
		tools::Timer _timer;
	};
}
