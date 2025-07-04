#pragma once

#include "config.h"

#include "physics/include/physics_bundles.h"

#include "physics/include/aabb.h"

#include "physics/include/obb.h"


namespace physics
{
	//template <glType::BoundType T>
	//class Rigid;


	//class RigidBody // Why this design? I want the _collisionCallback to be able to take both 
	//	an AABB and OBB version of the class and not have to worry about which one it is. 
	//	 This way, I can just pass the base class to the callback and it will work for both types of meshes.
	//{
	//public:

	//	RigidBody();

	//	void check_collisions(const std::vector<RigidBody>& other);

	//	virtual bool is_touching(const RigidBody& other) const = 0;

	//	virtual bool is_touching(const AABB& other) const = 0;

	//	virtual bool is_touching(const OBB& other) const = 0;


	//	Currently TESTING
	//	void set_collision_callback(std::function<void(const RigidBody& me, const RigidBody& other)>& callback);// EZ_FIND_ME_collision_response_feature

	//	void check_collision(const RigidBody& other);

	//	virtual void update(const RigidBody& other) = 0;

	//	virtual void update(const std::vector<RigidBody>& other) = 0;

	//	virtual OBB get_obb_cube() const;

	//	virtual AABB get_aabb_cube() const;
	//
	//protected:

	//	std::function<void(const RigidBody& me, const RigidBody& other)> _collisionCallback;	
	//};

	//template<>
	//class Rigid<glType::BoundType::OBB>;

	//template<>
	//class Rigid<glType::BoundType::AABB> : public RigidBody
	//{
	//public:
	//	Rigid();

	//	Rigid(bool debug);

	//	Rigid(const RigidBundle& bundle, bool debug = false);

	//	void init(const RigidBundle& bundle);

	//	void set_aabb_cube(const AABB& box);

	//	AABB get_cube() const;
	//	
	//	bool is_touching(const RigidBody& other) const override;

	//	bool is_touching(const AABB& other) const override;

	//	bool is_touching(const OBB& other) const override;

	//	void update(const RigidBody& other) override;

	//	void update(const std::vector<RigidBody>& other) override;


	//	virtual ~Rigid();

	//protected:
	//	AABB _aabbCube;

	//	bool _debug;

	//protected:
	//	AABB get_aabb_cube() const override;
	//	friend class Rigid <glType::BoundType::OBB>;
	//};


	//template<>
	//class Rigid<glType::BoundType::OBB> : public RigidBody
	//{
	//public:
	//	Rigid();
	//	
	//	Rigid(bool debug);
	//	
	//	Rigid(const RigidBundle& bundle, bool debug = false);

	//	void init(const RigidBundle& bundle);

	//	void set_obb_cube(const OBB& cube);

	//	OBB get_cube() const;

	//	bool is_touching(const RigidBody& other) const override;

	//	bool is_touching(const AABB& other) const override;

	//	bool is_touching(const OBB& other) const override;

	//	void update(const RigidBody& other) override;

	//	void update(const std::vector<RigidBody>& other) override;

	//	virtual ~Rigid();

	//protected:
	//	OBB _obbCube;

	//	bool _debug;

	//protected:
	//	OBB get_obb_cube() const override;
	//	friend class Rigid<glType::BoundType::AABB>;
	//};

	//using RigidBodyAABB = Rigid<glType::BoundType::AABB>;
	//using RigidBodyOBB = Rigid<glType::BoundType::OBB>;


}