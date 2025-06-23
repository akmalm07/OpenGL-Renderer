#pragma once

#include "config.h"

#include "glUtil/include/mesh_bundle.h"
#include "glUtil/include/mesh.h"

#include "physics/include/acceleration.h"
#include "physics/include/volocity.h"


namespace physics
{

	template<glType::MovementType T>
	class MoveibleMesh : public glUtil::Mesh {};

	template<>
	class MoveibleMesh<glType::MovementType::Static> : public glUtil::Mesh
	{
	public:
		MoveibleMesh();
		MoveibleMesh(bool debugMode);
		MoveibleMesh(const glUtil::MeshBundle& bundle, bool debugMode);
		virtual ~MoveibleMesh();
	};

	template<>
	class MoveibleMesh<glType::MovementType::Constant> : public glUtil::Mesh
	{
	public:
		MoveibleMesh();

		MoveibleMesh(bool debugMode);

		MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode);

		void set_volocity(const physics::Volocity& val);
		
		void set_mass(float val);

		void set_position(const glm::vec3& val);

		float get_mass() const;

		void start_time();

		void reset_time();

		double stop_time();

		void render() override;

		glm::vec3 get_net_momentum() const;

		float get_momentum_x() const;
		
		float get_momentum_y() const;
		
		float get_momentum_z() const;

		void add_gravity(const glm::vec3& val);

		glm::vec3 get_position() const;

		glm::mat4 get_model_matrix() const override;

		void update_position();

		~MoveibleMesh();

	private:
		Volocity _volocity = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 _positionRelativeToStart = glm::vec3(0.0f, 0.0f, 0.0f);

		bool _gravityAffected = true;

		float _mass = 1.0f;
	};

	template<>
	class MoveibleMesh<glType::MovementType::Kenimatic> : public glUtil::Mesh
	{
	public:
		MoveibleMesh();
		
		MoveibleMesh(bool debugMode);
		
		MoveibleMesh(const MoveibleMeshBundle& bundle, bool debugMode);
		
		void set_acceleration(const physics::Acceleration& val);

		void start_time();

		void reset_time();

		void set_position(const glm::vec3& val);

		void render() override;

		double stop_time();
		
		void set_mass(float val);
		
		float get_mass() const;
		
		glm::vec3 get_net_force() const;
		
		float get_net_force_x() const;
	
		float get_net_force_y() const;
		
		float get_net_force_z() const;
		
		glm::vec3 get_current_net_momentum() const;

		float get_current_momentum_x() const;

		float get_current_momentum_y() const;

		float get_current_momentum_z() const;

		void add_gravity(const glm::vec3& val);
		
		glm::vec3 get_position();

		glm::mat4 get_model_matrix() const override;

		void update_position();
		
		~MoveibleMesh();

	private:
		
		Acceleration _acceleration = glm::vec3(0.0f, 0.0f, 0.0f);
		
		glm::vec3 _positionRelativeToStart = glm::vec3(0.0f, 0.0f, 0.0f);

		bool _gravityAffected = true;

		float _mass = 1.0f;
	};

}
