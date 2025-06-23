#pragma once

#include "config.h"

#include "physics/include/physics_bundles.h"

#include "physics/include/acceleration.h"
#include "physics/include/volocity.h"


namespace physics
{

	template<glType::MovementType T>
	class Moveible;


	class MoveibleBody
	{
	public:
		MoveibleBody();

		virtual void add_gravity(const glm::vec3& val) = 0;
		
		virtual void erase_gravity() = 0;

		virtual void set_mass(float val) = 0;

		virtual float get_mass() const = 0;

		virtual void set_volocity(const glm::vec3& val) = 0;

		virtual void change_volocity(const glm::vec3& val) = 0;

		virtual void set_position(const glm::vec3& val) = 0;
		
		void change_position(const glm::vec3& val);

		virtual glm::vec3 get_position() const = 0;

		virtual glm::vec3 get_net_momentum() const = 0;

		virtual float get_momentum_x() const = 0;
		virtual float get_momentum_y() const = 0;
		virtual float get_momentum_z() const = 0;


		virtual void update_position(float deltaTime) = 0;

		virtual void update_position(glm::vec3 volocity) = 0;

		virtual void update(float deltaTime) = 0;

		virtual glm::mat4 get_model_matrix(const glm::mat4& mat = glm::mat4(1.0f)) const = 0;

		virtual ~MoveibleBody() = default;

	protected:

		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f);

		glm::vec3 _gravity = glm::vec3(0.0f, 0.0f, 0.0f);

		bool _gravityAffected = true;

		float _mass = 1.0f;
	};

	template<>
	class Moveible<glType::MovementType::Constant> : public MoveibleBody
	{
	public:
		Moveible();

		Moveible(bool debugMode);

		Moveible(const MoveibleBundle& bundle, bool debugMode);

		void init(const MoveibleBundle& bundle);

		void set_volocity(const glm::vec3& val) override;
		
		void change_volocity(const glm::vec3& val) override;
		
		void set_mass(float val) override;

		void set_position(const glm::vec3& val) override;

		float get_mass() const override;

		void update(float deltaTime) override;

		glm::vec3 get_net_momentum() const override;

		float get_momentum_x() const override;
		float get_momentum_y() const override;		
		float get_momentum_z() const override;

		void add_gravity(const glm::vec3& val) override;

		void erase_gravity() override;

		glm::vec3 get_position() const override;

		glm::mat4 get_model_matrix(const glm::mat4& mat = glm::mat4(1.0f)) const override;

		virtual void update_position(float deltaTime);

		virtual void update_position(glm::vec3 volocityTimesDeltaTime);

		~Moveible();

	protected:

		glm::vec3 _volocity;
		
		bool _debug = false;
	};

	template<>
	class Moveible<glType::MovementType::Kenimatic> : public MoveibleBody
	{
	public:
		Moveible();
		
		Moveible(bool debugMode);
		
		Moveible(const MoveibleBundle& bundle, bool debugMode);
		
		void init(const MoveibleBundle& bundle);
		
		void set_volocity(const glm::vec3& val) override;

		void change_volocity(const glm::vec3& val) override;

		void set_acceleration(const glm::vec3& val);
	
		void change_acceleration(const glm::vec3& val);
		
		void set_position(const glm::vec3& pos) override;

		void update(float deltaTime) override;
		
		void set_mass(float val) override;
		
		float get_mass() const override;
		

		glm::vec3 get_net_force() const;
		
		float get_net_force_x() const;
		float get_net_force_y() const;	
		float get_net_force_z() const;
		

		glm::vec3 get_net_momentum() const override;

		float get_momentum_x() const override;
		float get_momentum_y() const override;
		float get_momentum_z() const override;

		void add_gravity(const glm::vec3& val) override;

		void erase_gravity();
		
		glm::vec3 get_position() const override;

		glm::mat4 get_model_matrix(const glm::mat4& mat = glm::mat4(1.0f)) const override;

		virtual void update_position(float deltaTime);
		
		virtual void update_position(glm::vec3 volocityTimesAccelerationTimesDeltaTime);
		
		~Moveible();

	protected:

		MovementFactors _movementFactors;

		bool _debug = false;
	};


}
