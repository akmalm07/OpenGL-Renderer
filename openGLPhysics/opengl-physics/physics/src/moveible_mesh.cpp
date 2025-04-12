#include "headers.h"
#include "physics/include/moveible_mesh.h"

namespace physics
{

	MoveibleMesh<glType::MovementType::KINEMATIC>::MoveibleMesh()
		: glUtil::Mesh()
	{}

	MoveibleMesh<glType::MovementType::KINEMATIC>::MoveibleMesh(bool debugMode)
		: glUtil::Mesh(debugMode)
	{}
	
	MoveibleMesh<glType::MovementType::KINEMATIC>::MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode)
		: glUtil::Mesh(bundle, debugMode)
	{
		_gravityAffected = bundle.gravityAffected;
		if (_gravityAffected)
		{
			add_gravity(bundle.gravity);
		}
		_mass = bundle.mass;
		_acceleration = { bundle.rateOfChange, bundle.startTime };
	}

	MoveibleMesh<glType::MovementType::CONSTANT>::MoveibleMesh()
		: glUtil::Mesh()
	{
	}

	MoveibleMesh<glType::MovementType::CONSTANT>::MoveibleMesh(bool debugMode)
		: glUtil::Mesh(debugMode)
	{
	}

	MoveibleMesh<glType::MovementType::CONSTANT>::MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode)
		: glUtil::Mesh(bundle, debugMode)
	{
		_gravityAffected = bundle.gravityAffected;
		if (_gravityAffected)
		{
			add_gravity(bundle.gravity);
		}
		_mass = bundle.mass;
		_volocity = bundle.rateOfChange;
	}
	
	void MoveibleMesh<glType::MovementType::CONSTANT>::set_volocity(const Volocity& val)
	{
		_volocity = val;
	}
		
	void MoveibleMesh<glType::MovementType::KINEMATIC>::set_acceleration(const Acceleration& val)
	{
		_acceleration = val;
	}
	
	void MoveibleMesh<glType::MovementType::KINEMATIC>::set_mass(float val)
	{
		_mass = val;
	}
	
	void MoveibleMesh<glType::MovementType::CONSTANT>::set_mass(float val)
	{
		_mass = val;
	}

	void MoveibleMesh<glType::MovementType::CONSTANT>::set_position(const glm::vec3& val)
	{
		_positionRelativeToStart = val;
		_volocity.reset_time();
	}

	void MoveibleMesh<glType::MovementType::KINEMATIC>::set_position(const glm::vec3& val)
	{
		_positionRelativeToStart = val;
		_acceleration.reset_time(); 

	}
	
	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_mass() const
	{
		return _mass;
	}
		
	float MoveibleMesh<glType::MovementType::CONSTANT>::get_mass() const
	{
		return _mass;
	}
	
	void MoveibleMesh<glType::MovementType::KINEMATIC>::start_time()
	{
		_acceleration.start_time();
	}
	
	void MoveibleMesh<glType::MovementType::KINEMATIC>::reset_time()
	{
		_acceleration.reset_time();
	}
	
	double MoveibleMesh<glType::MovementType::KINEMATIC>::stop_time()
	{
		return _acceleration.stop_time();
	}

	void MoveibleMesh<glType::MovementType::CONSTANT>::start_time()
	{
		_volocity.start_time();
	}
	
	void MoveibleMesh<glType::MovementType::CONSTANT>::reset_time()
	{
		_volocity.reset_time();
	}
	
	double MoveibleMesh<glType::MovementType::CONSTANT>::stop_time()
	{
		return _volocity.stop_time();
	}

	void MoveibleMesh<glType::MovementType::CONSTANT>::render()
	{
		update_position();
		glUtil::Mesh::render();
	}

	void MoveibleMesh<glType::MovementType::KINEMATIC>::render()
	{
		update_position();
		glUtil::Mesh::render();
	}
	
	glm::vec3 MoveibleMesh<glType::MovementType::KINEMATIC>::get_net_force() const
	{
		return glm::vec3(
			get_net_force_x(),
			get_net_force_y(),
			get_net_force_z());
	}

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_net_force_x() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_x());
	}

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_net_force_y() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_y());
	}

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_net_force_z() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_z());
	}

	glm::vec3 MoveibleMesh<glType::MovementType::KINEMATIC>::get_current_net_momentum() const
	{
		return glm::vec3(
			get_current_momentum_x(),
			get_current_momentum_y(),
			get_current_momentum_z());
	}

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_current_momentum_x() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_x());
	}

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_current_momentum_y() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_y());
	}
	

	float MoveibleMesh<glType::MovementType::KINEMATIC>::get_current_momentum_z() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_z());
	}


	glm::vec3 MoveibleMesh<glType::MovementType::CONSTANT>::get_net_momentum() const
	{
		return glm::vec3(
			get_momentum_x(),
			get_momentum_y(),
			get_momentum_z());
	}

	float MoveibleMesh<glType::MovementType::CONSTANT>::get_momentum_x() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_x());
	}

	float MoveibleMesh<glType::MovementType::CONSTANT>::get_momentum_y() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_y());
	}

	float MoveibleMesh<glType::MovementType::CONSTANT>::get_momentum_z() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_z());
	}

	void MoveibleMesh<glType::MovementType::CONSTANT>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected) 
		{
			_volocity += val;
			_gravityAffected = false;
		}
	}

	void MoveibleMesh<glType::MovementType::KINEMATIC>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected) 
		{
			_acceleration += val;
			_gravityAffected = false;
		}
	}

	glm::vec3 MoveibleMesh<glType::MovementType::KINEMATIC>::get_position()
	{
		return _positionRelativeToStart;
	}

	void MoveibleMesh<glType::MovementType::KINEMATIC>::update_position()
	{
		_positionRelativeToStart = _acceleration.get_distance();
	}

	glm::vec3 MoveibleMesh<glType::MovementType::CONSTANT>::get_position() const 
	{
		return _positionRelativeToStart;
	}

	glm::mat4 MoveibleMesh<glType::MovementType::CONSTANT>::get_model_matrix() const
	{
		return glm::translate(glm::mat4(1.0f), _positionRelativeToStart);
	}
	
	glm::mat4 MoveibleMesh<glType::MovementType::KINEMATIC>::get_model_matrix() const
	{
		return glm::translate(glm::mat4(1.0f), _positionRelativeToStart);
	}

	void MoveibleMesh<glType::MovementType::CONSTANT>::update_position()
	{
		_positionRelativeToStart = _volocity.get_distance();

	}

	MoveibleMesh<glType::MovementType::KINEMATIC>::~MoveibleMesh() = default;

	MoveibleMesh<glType::MovementType::CONSTANT>::~MoveibleMesh() = default;

}