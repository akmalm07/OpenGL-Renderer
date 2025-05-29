#include "headers.h"  
#include "physics/include/rigid.h"  

namespace physics  
{  

	Rigid<glType::BoundType::AABB>::Rigid() = default;
	
	Rigid<glType::BoundType::AABB>::Rigid(bool debug)
		: _debug(debug)
	{}


	Rigid<glType::BoundType::AABB>::Rigid(const RigidBundle& bundle, bool debug)
		: _debug(debug)
	{
		init(bundle);
	}  


	void Rigid<glType::BoundType::AABB>::init(const RigidBundle& bundle)
	{
		_aabbCube.init(bundle.min, bundle.max);
	}

	void Rigid<glType::BoundType::AABB>::set_aabb_cube(const AABB& cube)
	{
		_aabbCube = std::move(cube);
	}

	AABB Rigid<glType::BoundType::AABB>::get_aabb_cube() const
	{
		return _aabbCube;
	}

	bool Rigid<glType::BoundType::AABB>::is_touching(const RigidBody& other) const
	{
		if (auto otherAABB = dynamic_cast<const Rigid<glType::BoundType::AABB>*>(&other))
		{
			return is_touching(otherAABB->get_aabb_cube());
		}
		else if (auto otherOBB = dynamic_cast<const Rigid<glType::BoundType::OBB>*>(&other))
		{
			return is_touching(otherOBB->get_obb_cube());
		}
		return false;
	}

	bool Rigid<glType::BoundType::AABB>::is_touching(const AABB& other) const
	{
		if (_aabbCube.is_touching(other))
		{
			if (_debug)
			{
				std::cout << "A collision occurred!\n";
			}
			return true;
		}
		return false;
	}

	bool Rigid<glType::BoundType::AABB>::is_touching(const OBB& other) const
	{
		if (_aabbCube.is_touching(other))
		{
			if (_debug)
			{
				std::cout << "A collision occurred!\n";
			}
			return true;
		}
		return false;
	}
	
	AABB Rigid<glType::BoundType::AABB>::get_cube() const
	{
		return get_aabb_cube();
	}


	void Rigid<glType::BoundType::AABB>::update(const RigidBody& other)
	{
		check_collision(other);
	}

	void Rigid<glType::BoundType::AABB>::update(const std::vector<RigidBody>& other)
	{
		for (const auto& mesh : other)
		{
			check_collision(mesh);
		}
	}

	Rigid<glType::BoundType::AABB>::~Rigid() = default;



	Rigid<glType::BoundType::OBB>::Rigid() = default;

	Rigid<glType::BoundType::OBB>::Rigid(bool debug)
		: _debug(debug)
	{}

	Rigid<glType::BoundType::OBB>::Rigid(const RigidBundle& bundle, bool debug)
		: _debug(debug)
	{
		init(bundle);
	}

	void Rigid<glType::BoundType::OBB>::init(const RigidBundle& bundle)
	{
		_obbCube.init(bundle.min, bundle.max,glm::vec3(), 1);
	}

	void Rigid<glType::BoundType::OBB>::set_obb_cube(const OBB& cube)
	{
		_obbCube = cube;
	}

	OBB Rigid<glType::BoundType::OBB>::get_cube() const
	{
		return get_obb_cube();
	}

	OBB Rigid<glType::BoundType::OBB>::get_obb_cube() const
	{
		return _obbCube;
	}

	bool Rigid<glType::BoundType::OBB>::is_touching(const OBB& other) const
	{
		if (_obbCube.is_touching(other))
		{
			if (_debug)
			{
				std::cout << "A collision occurred!\n";
			}
			return true;
		}
		return false;
	}

	bool Rigid<glType::BoundType::OBB>::is_touching(const AABB& other) const
	{
		if (_obbCube.is_touching(other))
		{
			if (_debug)
			{
				std::cout << "A collision occurred!\n";
			}
			return true;
		}
		return false;
	}

	bool Rigid<glType::BoundType::OBB>::is_touching(const RigidBody& other) const
	{
		if (auto otherAABB = dynamic_cast<const Rigid<glType::BoundType::AABB>*>(&other))
		{
			return is_touching(otherAABB->get_aabb_cube());
		}
		else if (auto otherOBB = dynamic_cast<const Rigid<glType::BoundType::OBB>*>(&other))
		{
			return is_touching(otherOBB->get_obb_cube());
		}
		return false;
	}

	void Rigid<glType::BoundType::OBB>::update(const RigidBody& other)
	{
		check_collision(other);
	}

	void Rigid<glType::BoundType::OBB>::update(const std::vector<RigidBody>& other)
	{
		for (const auto& mesh : other)
		{
			check_collision(mesh);
		}
	}


	Rigid<glType::BoundType::OBB>::~Rigid() = default;

	OBB RigidBody::get_obb_cube() const
	{
		return OBB();
	}

	AABB RigidBody::get_aabb_cube() const
	{
		return AABB();
	}

	RigidBody::RigidBody() = default;

	void RigidBody::set_collision_callback(std::function<void(const RigidBody& me, const RigidBody& other)>& callback)
	{
		_collisionCallback = std::move(callback);
	}

	void RigidBody::check_collision(const RigidBody& other)
	{
		if (_collisionCallback && is_touching(other))
		{
			_collisionCallback(*this, other);
		}
	}

	void RigidBody::check_collisions(const std::vector<RigidBody>& other)
	{
		for (const auto& mesh : other)
		{
			check_collision(mesh);
		}
	}
}
