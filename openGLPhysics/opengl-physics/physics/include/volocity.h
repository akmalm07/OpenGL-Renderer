#pragma once

#include "config.h"
#include "tools/include/timer.h"


namespace physics  
{  
	class Volocity  
	{  
	public:  
		Volocity();  

		Volocity(const glm::vec3& val);  

		Volocity(const glm::vec3& val, const tools::Timer& timer);  

		void set_volocity(const glm::vec3& val);  

		glm::vec3 get_volocity() const;  

		glm::vec3 get_distance() const;  

		glm::vec3 operator+(const glm::vec3& vec);

		void operator+=(const glm::vec3& vec);

		double get_distance_x() const;  

		double get_distance_y() const;  

		double get_distance_z() const;  


		double get_volocity_x() const;

		double get_volocity_y() const;
		
		double get_volocity_z() const;

		void start_time();  

		void reset_time();  

		double stop_time();  

		double get_current_time() const;  

	private:  
		glm::vec3 _volocity = glm::vec3(0.0f, 0.0f, 0.0f);  
		tools::Timer _timer;  
	};  
}
