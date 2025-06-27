#pragma once


#include "config.h"


namespace physics
{

	class PhysicsBodyBase;


	class SpatialPartioningBase
	{
	public:
		SpatialPartioningBase() = default;
		SpatialPartioningBase(const SpatialPartioningBase&) = delete;
		SpatialPartioningBase& operator=(const SpatialPartioningBase&) = delete;
		SpatialPartioningBase(SpatialPartioningBase&&) noexcept = default;
		SpatialPartioningBase& operator=(SpatialPartioningBase&&) noexcept = default;
		virtual ~SpatialPartioningBase() = default;

		virtual void register_body(const PhysicsBodyBase& body) = 0;
		virtual void update_entity_position(glType::Entity entity, const glm::vec3& newPosition) = 0;
		virtual std::vector<glType::Entity> query(const glm::vec3& position, const glm::vec3& size) const = 0;
	};


	template<size_t CellCount>
	class SpatialPartioning : public SpatialPartioningBase
	{
	public:
		SpatialPartioning() = default;
		SpatialPartioning(const SpatialPartioning&) = delete;
		SpatialPartioning& operator=(const SpatialPartioning&) = delete;
		SpatialPartioning(SpatialPartioning&&) noexcept = default;
		SpatialPartioning& operator=(SpatialPartioning&&) noexcept = default;

		SpatialPartioning(const glm::vec3& minBound, const glm::vec3& maxBound);

		static void register_body(const PhysicsBodyBase& body);
		void update_entity_position(glType::Entity entity, const glm::vec3& newPosition);
		std::vector<glType::Entity> query(const glm::vec3& position, const glm::vec3& size) const;

	private:

		float _cellWidth;
		float _cellHeight;

		std::array<glm::vec3, CellCount> _cellMinBounds;
		std::array<glm::vec3, CellCount> _cellMaxBounds;

		std::array<std::vector<glType::Entity>, CellCount> _cells;

	};

}


#include "physics/include/spatial_partioning.inl"