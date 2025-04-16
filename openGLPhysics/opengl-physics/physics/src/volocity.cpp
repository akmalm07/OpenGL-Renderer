#include "headers.h"
#include "physics/include/volocity.h"

namespace physics
{
	Volocity::Volocity() = default;

	Volocity::Volocity(const glm::vec3& val)
		: _volocity(val) {}
	
	Volocity::Volocity(const glm::vec3& val, const tools::Timer& timer)
		: _volocity(val), _timer(timer) {}

	void Volocity::set_volocity(const glm::vec3& val)
	{
		_volocity = val;
	}

	glm::vec3 Volocity::get_volocity() const
	{
		return _volocity;
	}

	glm::vec3 Volocity::get_distance() const // MAKE MORE EFFICIENT SO THE TIME IS CHECKED ONLY ONCE --Dev note
	{
		auto currentTime = _timer.current_time_s();
		return glm::vec3(
			_volocity.x * currentTime,
			_volocity.y * currentTime,
			_volocity.z * currentTime
		);
	}

	glm::vec3 Volocity::operator+(const glm::vec3& vec)
	{
		return _volocity + vec;
	}

	void Volocity::operator+=(const glm::vec3& vec)
	{
		_volocity += vec;
	}

	double Volocity::get_distance_x() const
	{
		return _volocity.x * _timer.current_time_s();
	}
	double Volocity::get_distance_y() const
	{
		return _volocity.y * _timer.current_time_s();
	}
	double Volocity::get_distance_z() const
	{
		return _volocity.z * _timer.current_time_s();
	}

	double Volocity::get_volocity_x() const
	{
		return _volocity.x;
	}
	

	double Volocity::get_volocity_y() const
	{
		return _volocity.y;
	}
	

	double Volocity::get_volocity_z() const
	{
		return _volocity.z;
	}

	void Volocity::start_time()
	{
		_timer.start_time();
	}

	void Volocity::reset_time()
	{
		_timer.reset();
	}

	double Volocity::stop_time()
	{
		return _timer.stop_time();
	}

	double Volocity::get_current_time() const
	{
		return _timer.current_time_s();
	}
}

