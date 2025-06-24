#include "headers.h"
#include "physics/include/moveible.h"

namespace physics
{
	void Moveible::start_time()
	{
		_timer.start_time();
	}

	double Moveible::stop_time()
	{
		return _timer.stop_time();
	}

	void Moveible::reset_time()
	{
		_timer.reset();
	}

	glm::vec3 Moveible::get_position() const
	{
		return _position + get_distance();
	}

	float Moveible::get_current_time() const
	{
		return _timer.current_time_s();
	}

	float Moveible::get_current_time_ms() const
	{
		return _timer.current_time_ms();
	}

	tools::Timer Moveible::get_timer() const

	{
		return _timer;
	}
}


/*
Moveible<glType::MovementType::Kenimatic>::Moveible() = default;

Moveible<glType::MovementType::Kenimatic>::Moveible(bool debugMode)
	: _debug(debugMode)
{}

Moveible<glType::MovementType::Constant>::Moveible() = default;

Moveible<glType::MovementType::Constant>::Moveible(bool debugMode)
	: _debug(debugMode) {}

Moveible<glType::MovementType::Constant>::Moveible(const MoveibleBundle& bundle, bool debugMode)
	: _debug(debugMode)
{
	init(bundle);
}

Moveible<glType::MovementType::Kenimatic>::Moveible(const MoveibleBundle& bundle, bool debugMode)
{
	_debug = debugMode;
	init(bundle);
}

void Moveible<glType::MovementType::Kenimatic>::init(const MoveibleBundle& bundle)
{
	_gravityAffected = bundle.gravityAffected;

	add_gravity(bundle.gravity);

	_mass = bundle.mass;
	_movementFactors.acceleration = bundle.movementFactors.acceleration;
	_movementFactors.volocity = bundle.movementFactors.volocity;

}


void Moveible<glType::MovementType::Constant>::init(const MoveibleBundle& bundle)
{
	_gravityAffected = bundle.gravityAffected;

	add_gravity(bundle.gravity);

	_mass = bundle.mass;
	_volocity = bundle.movementFactors.volocity;
}

void Moveible<glType::MovementType::Constant>::set_volocity(const glm::vec3& val)
{
	_volocity = val;
}


void Moveible<glType::MovementType::Kenimatic>::set_volocity(const glm::vec3& val)
{
	_movementFactors.volocity = val;
}

void Moveible<glType::MovementType::Constant>::change_volocity(const glm::vec3& val)
{
	_volocity += val;
}

void Moveible<glType::MovementType::Kenimatic>::change_volocity(const glm::vec3& val)
{
	_movementFactors.volocity += val;
}

void Moveible<glType::MovementType::Kenimatic>::set_acceleration(const glm::vec3& val)
{
	_movementFactors.acceleration = val;
}

void Moveible<glType::MovementType::Kenimatic>::change_acceleration(const glm::vec3& val)
{
	_movementFactors.acceleration += val;
}

void Moveible<glType::MovementType::Kenimatic>::set_mass(float val)
{
	_mass = val;
}

void Moveible<glType::MovementType::Constant>::set_mass(float val)
{
	_mass = val;
}

void Moveible<glType::MovementType::Constant>::set_position(const glm::vec3& pos)
{
	_position = pos;
}

void Moveible<glType::MovementType::Kenimatic>::set_position(const glm::vec3& pos)
{
	_position = pos;
}

float Moveible<glType::MovementType::Kenimatic>::get_mass() const
{
	return _mass;
}

float Moveible<glType::MovementType::Constant>::get_mass() const
{
	return _mass;
}

void Moveible<glType::MovementType::Constant>::update(float deltaTime)
{
	update_position(deltaTime);
}

void Moveible<glType::MovementType::Kenimatic>::update(float deltaTime)
{
	update_position(deltaTime);
}

glm::vec3 Moveible<glType::MovementType::Kenimatic>::get_net_force() const
{
	return glm::vec3(
		get_net_force_x(),
		get_net_force_y(),
		get_net_force_z());
}

float Moveible<glType::MovementType::Kenimatic>::get_net_force_x() const
{
	return _mass * static_cast<float>(_movementFactors.acceleration.x);
}

float Moveible<glType::MovementType::Kenimatic>::get_net_force_y() const
{
	return _mass * static_cast<float>(_movementFactors.acceleration.y);
}

float Moveible<glType::MovementType::Kenimatic>::get_net_force_z() const
{
	return _mass * static_cast<float>(_movementFactors.acceleration.z);
}

glm::vec3 Moveible<glType::MovementType::Kenimatic>::get_net_momentum() const
{
	return glm::vec3(
		_mass * static_cast<float>(_movementFactors.volocity.z),
		_mass * static_cast<float>(_movementFactors.volocity.z),
		_mass * static_cast<float>(_movementFactors.volocity.z));
}

float Moveible<glType::MovementType::Kenimatic>::get_momentum_x() const
{
	return _mass * static_cast<float>(_movementFactors.volocity.x);
}

float Moveible<glType::MovementType::Kenimatic>::get_momentum_y() const
{
	return _mass * static_cast<float>(_movementFactors.volocity.y);
}


float Moveible<glType::MovementType::Kenimatic>::get_momentum_z() const
{
	return _mass * static_cast<float>(_movementFactors.volocity.z);
}


glm::vec3 Moveible<glType::MovementType::Constant>::get_net_momentum() const
{
	return glm::vec3(
		_mass * static_cast<float>(_volocity.x),
		_mass * static_cast<float>(_volocity.y),
		_mass * static_cast<float>(_volocity.z));
}

float Moveible<glType::MovementType::Constant>::get_momentum_x() const
{
	return _mass * static_cast<float>(_volocity.x);
}

float Moveible<glType::MovementType::Constant>::get_momentum_y() const
{
	return _mass * static_cast<float>(_volocity.y);
}

float Moveible<glType::MovementType::Constant>::get_momentum_z() const
{
	return _mass * static_cast<float>(_volocity.z);
}

void Moveible<glType::MovementType::Constant>::add_gravity(const glm::vec3& val)
{
	if (_gravityAffected && val != glm::vec3(0.0f))
	{
		_volocity += val;
		_gravityAffected = false;
		_gravity = val;
	}
}

void Moveible<glType::MovementType::Constant>::erase_gravity()
{
	_volocity -= _gravity;
}

void Moveible<glType::MovementType::Kenimatic>::erase_gravity()
{
	_movementFactors.acceleration -= _gravity;
}

void Moveible<glType::MovementType::Kenimatic>::add_gravity(const glm::vec3& val)
{
	if (_gravityAffected && val != glm::vec3(0.0f))
	{
		_movementFactors.acceleration += val;
		_gravityAffected = false;
		_gravity = val;
	}
}

glm::vec3 Moveible<glType::MovementType::Kenimatic>::get_position() const
{
	return _position;
}

void Moveible<glType::MovementType::Kenimatic>::update_position(float deltaTime)
{
	_movementFactors.volocity += _movementFactors.acceleration * deltaTime;
	_position += _movementFactors.volocity * deltaTime;
}

void Moveible<glType::MovementType::Kenimatic>::update_position(glm::vec3 volocityTimesAccelerationTimesDeltaTime)
{
	_position += volocityTimesAccelerationTimesDeltaTime;
}


glm::vec3 Moveible<glType::MovementType::Constant>::get_position() const
{
	return _position;
}

glm::mat4 Moveible<glType::MovementType::Constant>::get_model_matrix(const glm::mat4& mat) const
{
	return glm::translate(mat, _position);
}

glm::mat4 Moveible<glType::MovementType::Kenimatic>::get_model_matrix(const glm::mat4& mat) const
{
	return glm::translate(mat, _position);
}

void Moveible<glType::MovementType::Constant>::update_position(float deltaTime)
{
	_position += _volocity * deltaTime;
}

void Moveible<glType::MovementType::Constant>::update_position(glm::vec3 volocityTimesDeltaTime)
{
	_position += volocityTimesDeltaTime;
}

void MoveibleBody::change_position(const glm::vec3& val)
{
	_position += val;
}

Moveible<glType::MovementType::Kenimatic>::~Moveible() = default;

Moveible<glType::MovementType::Constant>::~Moveible() = default;

MoveibleBody::MoveibleBody() = default;

*/