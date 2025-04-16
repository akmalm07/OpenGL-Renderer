#include "headers.h"
#include "physics/include/acceleration.h"


namespace physics
{
	Acceleration::Acceleration() = default;

	Acceleration::Acceleration(const glm::vec3& val)
		: _acceleration(val) {}

	Acceleration::Acceleration(const glm::vec3& val, const tools::Timer& timer)
		: _acceleration(val), _timer(timer) {}


	void Acceleration::set_acceleration(const glm::vec3& val)
	{
		_acceleration = val;
	}

	void Acceleration::set_inital_vol(const glm::vec3& val)
	{
		_intialVol = val;
	}

	void Acceleration::start_time()
	{
		_timer.start_time();
	}

	double Acceleration::stop_time()
	{
		return _timer.stop_time();
	}

	void Acceleration::reset_time()
	{
		_timer.reset();
	}

	glm::vec3 Acceleration::get_acceleration() const
	{
		return _acceleration;
	}

	glm::vec3 Acceleration::get_intial_vol() const
	{
		return _intialVol;
	}


	glm::vec3 Acceleration::operator+(const glm::vec3& vec)
	{
		return _acceleration + vec;
	}

	void Acceleration::operator+=(const glm::vec3& vec)
	{
		_acceleration += vec;
	}

	double Acceleration::get_distance_x() const
	{
		double currentTime = _timer.current_time_s();
		return (_intialVol.x * currentTime) + (0.5 * _acceleration.x * (currentTime * currentTime));
	}

	double Acceleration::get_distance_y() const
	{
		double currentTime = _timer.current_time_s();
		return (_intialVol.y * currentTime) + (0.5 * _acceleration.y * (currentTime * currentTime));
	}
	
	double Acceleration::get_distance_z() const
	{
		double currentTime = _timer.current_time_s();
		return (_intialVol.z * currentTime) + (0.5 * _acceleration.z * (currentTime * currentTime));
	}

	glm::vec3 Acceleration::get_distance() const
	{
		double currentTime = _timer.current_time_s();

		return glm::vec3(
			(_intialVol.x * currentTime) + (0.5 * (_acceleration.x * (currentTime * currentTime))),
			(_intialVol.y * currentTime) + (0.5 * (_acceleration.y * (currentTime * currentTime))),
			(_intialVol.z * currentTime) + (0.5 * (_acceleration.z * (currentTime * currentTime)))
		);
	}

	double Acceleration::get_current_volocity_x() const
	{
		return _intialVol.x + (_acceleration.x * _timer.current_time_s());
	}
	
	double Acceleration::get_current_volocity_y() const
	{
		return _intialVol.y + (_acceleration.y * _timer.current_time_s());
	}
	
	double Acceleration::get_current_volocity_z() const
	{
		return _intialVol.z + (_acceleration.z * _timer.current_time_s());
	}

	glm::vec3 Acceleration::get_current_volocity() const
	{
		double currentTime = _timer.current_time_s();

		return glm::vec3(
			(_intialVol.x + (_acceleration.x * currentTime)),
			(_intialVol.y + (_acceleration.y * currentTime)),
			(_intialVol.z + (_acceleration.z * currentTime))
		);
	}

	double Acceleration::get_current_time() const
	{
		return _timer.current_time_s();
	}


}