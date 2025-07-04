#include "headers.h"
//#include "physics/include/volocity.h"
//
//namespace physics
//{
//
//	Volocity::Volocity(const glm::vec3& val, bool start)
//		: _volocity(val), Moveible(start) {
//	}
//	
//	Volocity::Volocity(const glm::vec3& val, const tools::Timer& timer)
//		: _volocity(val), Moveible(timer) {
//	}
//
//	void Volocity::set_volocity(const glm::vec3& val)
//	{
//		_volocity = val;
//	}
//
//	glm::vec3 Volocity::get_volocity() const
//	{
//		return _volocity;
//	}
//
//	glm::vec3 Volocity::get_distance() const // MAKE MORE EFFICIENT SO THE TIME IS CHECKED ONLY ONCE --Dev note
//	{
//		auto currentTime = _timer.current_time_s();
//		return glm::vec3(
//			_volocity.x * currentTime,
//			_volocity.y * currentTime,
//			_volocity.z * currentTime
//		);
//	}
//
//	glm::vec3 Volocity::operator+(const glm::vec3& vec)
//	{
//		return _volocity + vec;
//	}
//
//	glm::vec3 Volocity::operator+(const Volocity& vec)
//	{
//		return _volocity + vec._volocity;
//	}
//
//	glm::vec3 Volocity::operator*(const glm::vec3& vec) const
//	{
//		return vec * _volocity;
//	}
//
//	glm::vec3 Volocity::operator*(const float vec) const
//	{
//		return vec * _volocity;
//	}
//
//	void Volocity::operator=(const glm::vec3& vec)
//	{
//		_volocity = vec;
//	}
//
//	bool Volocity::in_motion() const
//	{
//		return _volocity != glm::vec3(0.0f, 0.0f, 0.0f) || _timer.get_state();
//	}
//
//	void Volocity::operator+=(const glm::vec3& vec)
//	{
//		_volocity += vec;
//	}
//}
//
