#pragma once


#include "config.h"


namespace physics
{
	 
	class PhysicsBody;


	class PhysicsManagerBase
	{
	public:

		PhysicsManagerBase() = default;

		PhysicsManagerBase(const PhysicsManagerBase&) = delete;
		PhysicsManagerBase& operator=(const PhysicsManagerBase&) = delete;
		PhysicsManagerBase(PhysicsManagerBase&&) noexcept = default;
		PhysicsManagerBase& operator=(PhysicsManagerBase&&) noexcept = default;
		virtual ~PhysicsManagerBase() = default;

		virtual void register_body(const PhysicsBody& body) = 0;
		virtual void update() = 0;
	
	private:

	};


	template<size_t CellCount>
	class PhysicsManager : public PhysicsManagerBase
	{
	public:
		PhysicsManager() = default;

		PhysicsManager(const PhysicsManager&) = delete;
		PhysicsManager& operator=(const PhysicsManager&) = delete;
		PhysicsManager(PhysicsManager&&) noexcept = default;
		PhysicsManager& operator=(PhysicsManager&&) noexcept = default;

		PhysicsManager(const glm::vec3& minBound, const glm::vec3& maxBound);

		void register_body(const PhysicsBody& body) override;
		
		void update() override;

		void update_positions();

		void respond_to_collisions();

	private:

		float _cellWidth;
		float _cellHeight;

		std::array<glm::vec3, CellCount> _cellMinBounds;
		std::array<glm::vec3, CellCount> _cellMaxBounds;

		std::array<std::vector<glType::Entity>, CellCount> _cells;

		// SoA design for participating entities
		struct {

			std::vector<glType::Entity> ents;
			std::vector<size_t> index;
			std::vector<float> vol;
		} _participatingEntities;
	private:

		void collision_response(physics::PhysicsBody* body, physics::PhysicsBody* other) const;

		float run_check_of_body_index(physics::PhysicsBody* body, size_t index, size_t initalIndex);

		float get_vol_of_entity_in_cell(physics::PhysicsBody* body, size_t index) const;
	};

}


#include "physics/include/physics_manager.inl"