#include "headers.h"
#include "physics/include/moveible.h"

namespace physics
{

	Moveible<glType::MovementType::KINEMATIC>::Moveible() = default;

	Moveible<glType::MovementType::KINEMATIC>::Moveible(bool debugMode)
		: _debug(debugMode)
	{}

	Moveible<glType::MovementType::CONSTANT>::Moveible() = default;

	Moveible<glType::MovementType::CONSTANT>::Moveible(bool debugMode)
		: _debug(debugMode) {}

	Moveible<glType::MovementType::CONSTANT>::Moveible(const MoveibleBundle& bundle, bool debugMode)
		: _debug(debugMode)
	{
		init(bundle);
	}
	
	Moveible<glType::MovementType::KINEMATIC>::Moveible(const MoveibleBundle& bundle, bool debugMode)
	{
		_debug = debugMode;
		init(bundle);
	}

	void Moveible<glType::MovementType::KINEMATIC>::init(const MoveibleBundle& bundle)
	{
		_gravityAffected = bundle.gravityAffected;
		
		add_gravity(bundle.gravity);
		
		_mass = bundle.mass;
		_movementFactors.acceleration = bundle.movementFactors.acceleration;
		_movementFactors.volocity = bundle.movementFactors.volocity;
		
	}


	void Moveible<glType::MovementType::CONSTANT>::init(const MoveibleBundle& bundle)
	{
		_gravityAffected = bundle.gravityAffected;

		add_gravity(bundle.gravity);

		_mass = bundle.mass;
		_volocity = bundle.movementFactors.volocity;
	}
	
	void Moveible<glType::MovementType::CONSTANT>::set_volocity(const glm::vec3& val)
	{
		_volocity = val;
	}
	
	
	void Moveible<glType::MovementType::KINEMATIC>::set_volocity(const glm::vec3& val)
	{
		_movementFactors.volocity = val;
	}

	void Moveible<glType::MovementType::CONSTANT>::change_volocity(const glm::vec3& val)
	{
		_volocity += val;
	}

	void Moveible<glType::MovementType::KINEMATIC>::change_volocity(const glm::vec3& val)
	{
		_movementFactors.volocity += val;
	}
		
	void Moveible<glType::MovementType::KINEMATIC>::set_acceleration(const glm::vec3& val)
	{
		_movementFactors.acceleration = val;
	}

	void Moveible<glType::MovementType::KINEMATIC>::change_acceleration(const glm::vec3& val)
	{
		_movementFactors.acceleration += val;
	}
	
	void Moveible<glType::MovementType::KINEMATIC>::set_mass(float val)
	{
		_mass = val;
	}
	
	void Moveible<glType::MovementType::CONSTANT>::set_mass(float val)
	{
		_mass = val;
	}

	void Moveible<glType::MovementType::CONSTANT>::set_position(const glm::vec3& pos)
	{
		_position = pos;
	}

	void Moveible<glType::MovementType::KINEMATIC>::set_position(const glm::vec3& pos)
	{
		_position = pos;
	}
	
	float Moveible<glType::MovementType::KINEMATIC>::get_mass() const
	{
		return _mass;
	}
		
	float Moveible<glType::MovementType::CONSTANT>::get_mass() const
	{
		return _mass;
	}
	
	void Moveible<glType::MovementType::CONSTANT>::update(float deltaTime)
	{
		update_position(deltaTime);
	}

	void Moveible<glType::MovementType::KINEMATIC>::update(float deltaTime)
	{
		update_position(deltaTime);
	}
	
	glm::vec3 Moveible<glType::MovementType::KINEMATIC>::get_net_force() const
	{
		return glm::vec3(
			get_net_force_x(),
			get_net_force_y(),
			get_net_force_z());
	}

	float Moveible<glType::MovementType::KINEMATIC>::get_net_force_x() const
	{
		return _mass * static_cast<float>(_movementFactors.acceleration.x);
	}

	float Moveible<glType::MovementType::KINEMATIC>::get_net_force_y() const
	{
		return _mass * static_cast<float>(_movementFactors.acceleration.y);
	}

	float Moveible<glType::MovementType::KINEMATIC>::get_net_force_z() const
	{
		return _mass * static_cast<float>(_movementFactors.acceleration.z);
	}

	glm::vec3 Moveible<glType::MovementType::KINEMATIC>::get_net_momentum() const
	{
		return glm::vec3(
			_mass * static_cast<float>(_movementFactors.volocity.z),
			_mass * static_cast<float>(_movementFactors.volocity.z),
			_mass * static_cast<float>(_movementFactors.volocity.z));
	}

	float Moveible<glType::MovementType::KINEMATIC>::get_momentum_x() const
	{
		return _mass * static_cast<float>(_movementFactors.volocity.x);
	}

	float Moveible<glType::MovementType::KINEMATIC>::get_momentum_y() const
	{
		return _mass * static_cast<float>(_movementFactors.volocity.y);
	}
	

	float Moveible<glType::MovementType::KINEMATIC>::get_momentum_z() const
	{
		return _mass * static_cast<float>(_movementFactors.volocity.z);
	}


	glm::vec3 Moveible<glType::MovementType::CONSTANT>::get_net_momentum() const
	{
		return glm::vec3(
			_mass * static_cast<float>(_volocity.x),
			_mass * static_cast<float>(_volocity.y),
			_mass * static_cast<float>(_volocity.z));
	}

	float Moveible<glType::MovementType::CONSTANT>::get_momentum_x() const
	{
		return _mass * static_cast<float>(_volocity.x);
	}

	float Moveible<glType::MovementType::CONSTANT>::get_momentum_y() const
	{
		return _mass * static_cast<float>(_volocity.y);
	}

	float Moveible<glType::MovementType::CONSTANT>::get_momentum_z() const
	{
		return _mass * static_cast<float>(_volocity.z);
	}

	void Moveible<glType::MovementType::CONSTANT>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected && val != glm::vec3(0.0f)) 
		{
			_volocity += val;
			_gravityAffected = false;
			_gravity = val;
		}
	}

	void Moveible<glType::MovementType::CONSTANT>::erase_gravity()
	{
		_volocity -= _gravity;
	}

	void Moveible<glType::MovementType::KINEMATIC>::erase_gravity()
	{
		_movementFactors.acceleration -= _gravity;
	}

	void Moveible<glType::MovementType::KINEMATIC>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected && val != glm::vec3(0.0f))
		{
			_movementFactors.acceleration += val;
			_gravityAffected = false;
			_gravity = val;
		}
	}

	glm::vec3 Moveible<glType::MovementType::KINEMATIC>::get_position() const
	{
		return _position;
	}

	void Moveible<glType::MovementType::KINEMATIC>::update_position(float deltaTime)
	{
		_movementFactors.volocity += _movementFactors.acceleration * deltaTime;
		_position += _movementFactors.volocity * deltaTime;
	}

	void Moveible<glType::MovementType::KINEMATIC>::update_position(glm::vec3 volocityTimesAccelerationTimesDeltaTime)
	{
		_position += volocityTimesAccelerationTimesDeltaTime;
	}


	glm::vec3 Moveible<glType::MovementType::CONSTANT>::get_position() const 
	{
		return _position;
	}

	glm::mat4 Moveible<glType::MovementType::CONSTANT>::get_model_matrix(const glm::mat4& mat) const
	{
		return glm::translate(mat, _position);
	}
	
	glm::mat4 Moveible<glType::MovementType::KINEMATIC>::get_model_matrix(const glm::mat4& mat) const
	{
		return glm::translate(mat, _position);
	}

	void Moveible<glType::MovementType::CONSTANT>::update_position(float deltaTime)
	{ 
		_position += _volocity * deltaTime;		
	}

	void Moveible<glType::MovementType::CONSTANT>::update_position(glm::vec3 volocityTimesDeltaTime)
	{
		_position += volocityTimesDeltaTime;
	}

	void MoveibleBody::change_position(const glm::vec3& val)
	{
		_position += val;
	}

	Moveible<glType::MovementType::KINEMATIC>::~Moveible() = default;

	Moveible<glType::MovementType::CONSTANT>::~Moveible() = default;

	MoveibleBody::MoveibleBody() = default;
}