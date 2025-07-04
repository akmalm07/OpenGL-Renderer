#include "headers.h"
//#include "physics/include/acceleration.h"
//
//#include "physics/include/units.h"
//
//namespace physics
//{
//
//	
//	Acceleration::Acceleration() = default;
//
//	Acceleration::Acceleration(const glm::vec3& val, bool start)
//		: _acceleration(units::to_meters_per_sec2(val)), Moveible(true) {
//	}
//
//	Acceleration::Acceleration(const glm::vec3& val, const tools::Timer& timer)
//		: _acceleration(units::to_meters_per_sec2(val)), Moveible(timer) {
//	}
//
//
//	void Acceleration::set_inital_vol(const glm::vec3& val)
//	{
//		_intialVol = val;
//	}
//
//	glm::vec3 Acceleration::get_acceleration() const
//	{
//		return _acceleration;
//	}
//
//	glm::vec3 Acceleration::get_intial_vol() const
//	{
//		return _intialVol;
//	}
//
//
//	glm::vec3 Acceleration::operator+(const glm::vec3& vec)
//	{
//		return _acceleration + vec;
//	}
//
//	glm::vec3 Acceleration::operator+(const Acceleration& vec)
//	{
//		return _acceleration + vec._acceleration;
//
//	}
//
//	void Acceleration::operator+=(const glm::vec3& vec)
//	{
//		_acceleration += vec;
//	}
//
//	void Acceleration::operator=(const glm::vec3& vec)
//	{
//		_acceleration = vec;
//	}
//
//	glm::vec3 Acceleration::operator*(const glm::vec3& vec)const
//	{
//		return _acceleration * vec;
//	}
//
//	glm::vec3 Acceleration::operator*(const float vec) const
//	{
//		return _acceleration * vec;
//	}
//
//
//	glm::vec3 Acceleration::get_distance() const
//	{
//		double currentTime = _timer.current_time_s();
//
//		return glm::vec3(
//			(_intialVol.x * currentTime) + (0.5 * (_acceleration.x * (currentTime * currentTime))),
//			(_intialVol.y * currentTime) + (0.5 * (_acceleration.y * (currentTime * currentTime))),
//			(_intialVol.z * currentTime) + (0.5 * (_acceleration.z * (currentTime * currentTime)))
//		);
//	}
//
//
//	glm::vec3 Acceleration::get_current_volocity() const
//	{
//		double currentTime = _timer.current_time_s();
//
//		return glm::vec3(
//			(_intialVol.x + (_acceleration.x * currentTime)),
//			(_intialVol.y + (_acceleration.y * currentTime)),
//			(_intialVol.z + (_acceleration.z * currentTime))
//		);
//	}
//
//	bool Acceleration::in_motion() const
//	{
//		return _acceleration != glm::vec3(0.0f, 0.0f, 0.0f) || _intialVol != glm::vec3(0.0f, 0.0f, 0.0f) || _timer.get_state();
//	}
//
//
//}