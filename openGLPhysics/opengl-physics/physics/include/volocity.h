#pragma once

#include "config.h"

#include "physics/include/moveible.h"


namespace physics  
{  
	class Volocity : public Moveible
	{  
	public:  
		Volocity() = default;  

		Volocity(const glm::vec3& val, bool start = true);
		Volocity(const glm::vec3& val, const tools::Timer& timer);


		void set_volocity(const glm::vec3& val);  

		glm::vec3 get_volocity() const;  

		glm::vec3 get_distance() const override;

		glm::vec3 operator+(const glm::vec3& vec) override;
		void operator+=(const glm::vec3& vec) override;

		glm::vec3 operator+(const Volocity& vec);

		glm::vec3 operator*(const glm::vec3& vec) const override;
		glm::vec3 operator*(const float vec) const override;

		void operator=(const glm::vec3& vec) override;

		bool in_motion() const override;

	private:  
		glm::vec3 _volocity = glm::vec3(0.0f, 0.0f, 0.0f);  
	};  
}
