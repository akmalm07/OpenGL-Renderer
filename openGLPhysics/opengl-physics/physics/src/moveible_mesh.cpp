#include "headers.h"

/*

#include "physics/include/moveible_mesh.h"

namespace physics
{

	MoveibleMesh<glType::MovementType::Kenimatic>::MoveibleMesh()
		: glUtil::Mesh()
	{}

	MoveibleMesh<glType::MovementType::Kenimatic>::MoveibleMesh(bool debugMode)
		: glUtil::Mesh(debugMode)
	{}

	MoveibleMesh<glType::MovementType::Static>::MoveibleMesh()
		: Mesh() {}

	MoveibleMesh<glType::MovementType::Static>::MoveibleMesh(bool debugMode)
		: Mesh(debugMode) {}

	MoveibleMesh<glType::MovementType::Static>::MoveibleMesh(const glUtil::MeshBundle& bundle, bool debugMode)
	: Mesh(bundle, debugMode) {}

	MoveibleMesh<glType::MovementType::Static>::~MoveibleMesh() = default;

	
	MoveibleMesh<glType::MovementType::Kenimatic>::MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode)
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

	MoveibleMesh<glType::MovementType::Constant>::MoveibleMesh()
		: glUtil::Mesh()
	{
	}

	MoveibleMesh<glType::MovementType::Constant>::MoveibleMesh(bool debugMode)
		: glUtil::Mesh(debugMode)
	{
	}

	MoveibleMesh<glType::MovementType::Constant>::MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode)
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
	
	void MoveibleMesh<glType::MovementType::Constant>::set_volocity(const Volocity& val)
	{
		_volocity = val;
	}
		
	void MoveibleMesh<glType::MovementType::Kenimatic>::set_acceleration(const Acceleration& val)
	{
		_acceleration = val;
	}
	
	void MoveibleMesh<glType::MovementType::Kenimatic>::set_mass(float val)
	{
		_mass = val;
	}
	
	void MoveibleMesh<glType::MovementType::Constant>::set_mass(float val)
	{
		_mass = val;
	}

	void MoveibleMesh<glType::MovementType::Constant>::set_position(const glm::vec3& val)
	{
		_positionRelativeToStart = val;
		_volocity.reset_time();
	}

	void MoveibleMesh<glType::MovementType::Kenimatic>::set_position(const glm::vec3& val)
	{
		_positionRelativeToStart = val;
		_acceleration.reset_time(); 

	}
	
	float MoveibleMesh<glType::MovementType::Kenimatic>::get_mass() const
	{
		return _mass;
	}
		
	float MoveibleMesh<glType::MovementType::Constant>::get_mass() const
	{
		return _mass;
	}
	
	void MoveibleMesh<glType::MovementType::Kenimatic>::start_time()
	{
		_acceleration.start_time();
	}
	
	void MoveibleMesh<glType::MovementType::Kenimatic>::reset_time()
	{
		_acceleration.reset_time();
	}
	
	double MoveibleMesh<glType::MovementType::Kenimatic>::stop_time()
	{
		return _acceleration.stop_time();
	}

	void MoveibleMesh<glType::MovementType::Constant>::start_time()
	{
		_volocity.start_time();
	}
	
	void MoveibleMesh<glType::MovementType::Constant>::reset_time()
	{
		_volocity.reset_time();
	}
	
	double MoveibleMesh<glType::MovementType::Constant>::stop_time()
	{
		return _volocity.stop_time();
	}

	void MoveibleMesh<glType::MovementType::Constant>::render()
	{
		update_position();
		glUtil::Mesh::render();
	}

	void MoveibleMesh<glType::MovementType::Kenimatic>::render()
	{
		update_position();
		glUtil::Mesh::render();
	}
	
	glm::vec3 MoveibleMesh<glType::MovementType::Kenimatic>::get_net_force() const
	{
		return glm::vec3(
			get_net_force_x(),
			get_net_force_y(),
			get_net_force_z());
	}

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_net_force_x() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_x());
	}

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_net_force_y() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_y());
	}

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_net_force_z() const
	{
		return _mass * static_cast<float>(_acceleration.get_distance_z());
	}

	glm::vec3 MoveibleMesh<glType::MovementType::Kenimatic>::get_current_net_momentum() const
	{
		return glm::vec3(
			get_current_momentum_x(),
			get_current_momentum_y(),
			get_current_momentum_z());
	}

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_current_momentum_x() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_x());
	}

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_current_momentum_y() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_y());
	}
	

	float MoveibleMesh<glType::MovementType::Kenimatic>::get_current_momentum_z() const
	{
		return _mass * static_cast<float>(_acceleration.get_current_volocity_z());
	}


	glm::vec3 MoveibleMesh<glType::MovementType::Constant>::get_net_momentum() const
	{
		return glm::vec3(
			get_momentum_x(),
			get_momentum_y(),
			get_momentum_z());
	}

	float MoveibleMesh<glType::MovementType::Constant>::get_momentum_x() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_x());
	}

	float MoveibleMesh<glType::MovementType::Constant>::get_momentum_y() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_y());
	}

	float MoveibleMesh<glType::MovementType::Constant>::get_momentum_z() const
	{
		return _mass * static_cast<float>(_volocity.get_distance_z());
	}

	void MoveibleMesh<glType::MovementType::Constant>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected && val != glm::vec3(0.0f)) 
		{
			_volocity += val;
			_gravityAffected = false;
		}
	}

	void MoveibleMesh<glType::MovementType::Kenimatic>::add_gravity(const glm::vec3& val)
	{
		if (_gravityAffected && val != glm::vec3(0.0f))
		{
			_acceleration += val;
			_gravityAffected = false;
		}
	}

	glm::vec3 MoveibleMesh<glType::MovementType::Kenimatic>::get_position()
	{
		return _positionRelativeToStart;
	}

	void MoveibleMesh<glType::MovementType::Kenimatic>::update_position()
	{
		_positionRelativeToStart = _acceleration.get_distance();
	}

	glm::vec3 MoveibleMesh<glType::MovementType::Constant>::get_position() const 
	{
		return _positionRelativeToStart;
	}

	glm::mat4 MoveibleMesh<glType::MovementType::Constant>::get_model_matrix() const
	{
		return glm::translate(glm::mat4(1.0f), _positionRelativeToStart);
	}
	
	glm::mat4 MoveibleMesh<glType::MovementType::Kenimatic>::get_model_matrix() const
	{
		return glm::translate(glm::mat4(1.0f), _positionRelativeToStart);
	}

	void MoveibleMesh<glType::MovementType::Constant>::update_position()
	{
		_positionRelativeToStart = _volocity.get_distance();
	}

	MoveibleMesh<glType::MovementType::Kenimatic>::~MoveibleMesh() = default;

	MoveibleMesh<glType::MovementType::Constant>::~MoveibleMesh() = default;

}

*/