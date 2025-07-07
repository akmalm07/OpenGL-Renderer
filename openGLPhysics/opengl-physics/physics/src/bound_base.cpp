#include "headers.h"
#include "physics/include/bound_base.h"
#include "physics/include/aabb.h"
#include "physics/include/obb.h"
#include "glUtil/include/mesh.h"

#include "physics/include/sphere_bound.h"

namespace physics
{


	MinMax BoundTypeBase::get_min_max() const
	{
		return { _min, _max };
	}

	void BoundTypeBase::change(const glm::vec3& val)
	{
		_min += val;
		_max += val;
		_center += val;
	}
	void BoundTypeBase::change_x(float val)
	{
		_min.x += val;
		_max.x += val;
		_center.x += val;
	}

	void BoundTypeBase::change_y(float val)
	{
		_min.y += val;
		_max.y += val;
		_center.y += val;
	}

	void BoundTypeBase::change_z(float val)
	{
		_min.z += val;
		_max.z += val;
		_center.z += val;
	}

	TouchingData BoundTypeBase::is_touching(const AABB& a, const AABB& b) const
	{
		if (a._max.x >= b._min.x && a._min.x <= b._max.x &&
			a._max.y >= b._min.y && a._min.y <= b._max.y &&
			a._max.z >= b._min.z && a._min.z <= b._max.z)
		{

			return { true, (glm::max(a._min, b._min) + glm::min(a._max, b._max)) * 0.5f };
		}
		return { false };
	}



	TouchingData BoundTypeBase::is_touching(const OBB& a, const OBB& b) const
	{
		if (BoundTypeBase::sphere_check(a, b) && BoundTypeBase::aabb_cast_check(b, a).result)
		{
			return full_sat_check(a, b);
		}
		else
		{
			return { false };
		}
	}


	TouchingData BoundTypeBase::is_touching(const AABB& a, const OBB& b) const
	{
		if (BoundTypeBase::sphere_check(a, b) && BoundTypeBase::partial_sat_check(b, a).result)
		{
			return full_sat_check(b, a);
		}
		else
		{
			return { false };
		}
	}


	bool BoundTypeBase::sphere_check(const AABB& a, const AABB& b) const
	{
		return (glm::length(a._center - b._center) <
			(glm::length(0.5f * (a._max - a._min)) +
				glm::length(0.5f * (b._max - b._min))));
	}

	bool BoundTypeBase::sphere_check(const OBB& a, const OBB& b) const
	{
		return (glm::length(a._center - b._center) <
			(glm::length(0.5f * (a._max - a._min)) +
				glm::length(0.5f * (b._max - b._min))));
	}

	bool BoundTypeBase::sphere_check(const AABB& a, const OBB& b) const
	{
		return (glm::length(a._center - b._center) <
			(glm::length(0.5f * (a._max - a._min)) +
				glm::length(0.5f * (b._max - b._min))));
	}


	TouchingData BoundTypeBase::sphere_check(const SphereBound& a, const SphereBound& b) const
	{
		if (glm::length(a._center - b._center) < (a._radius + b._radius))
		{
			return { true, a._center + glm::normalize(b._center - a._center) * a._radius };
		}
		return { false };
	}


	TouchingData BoundTypeBase::sphere_check(const SphereBound& a, const AABB& b) const
	{
		if (glm::length(a._center - b._center) <
			(a._radius + glm::length(0.5f * (b._max - b._min))))
		{
			return { true, glm::clamp(a._center, b._min, b._max) };
		}
		return { false };
	}

	TouchingData BoundTypeBase::sphere_check(const SphereBound& a, const OBB& b) const
	{
		glm::vec3 local = glm::transpose(b._rotationMat) * glm::vec4(a._center - b._center, 0.0f);
	
		glm::vec3 closestPointLocal = glm::clamp(local, -b._halfExtent, b._halfExtent);
		
		glm::vec3 closestPoint = b._center + glm::mat3(b._rotationMat) * closestPointLocal;
		
		float distance = glm::length(a._center - closestPoint);
		
		if (distance < a._radius)
		{
			return { true, closestPoint };
		}
		return { false };
	}




	TouchingData BoundTypeBase::full_sat_check(const OBB& a, const OBB& b) const
	{
		glm::vec3 centerDiff = b._center - a._center;

		glm::mat4 aRotationMat = a.get_rotation_matrix();
		std::array<glm::vec3, 3> axisA = {
			glm::vec3(aRotationMat[0]),
			glm::vec3(aRotationMat[1]),
			glm::vec3(aRotationMat[2])
		};

		glm::mat4 bRotationMat = b.get_rotation_matrix();
		std::array<glm::vec3, 3> axisB = {
			glm::vec3(bRotationMat[0]),
			glm::vec3(bRotationMat[1]),
			glm::vec3(bRotationMat[2])
		};

		std::array<glm::vec3, 15> testAxes;

		int axisCount = 0;

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisA[i];
		}

		for (int i = 0; i < 3; i++)
		{
			testAxes[axisCount++] = axisB[i];
		}

		for (const auto& axis1 : axisA)
		{
			for (const auto& axis2 : axisB)
			{
				glm::vec3 cross = glm::cross(axis1, axis2);
				float lengthSquared = glm::dot(cross, cross);

				if (lengthSquared > 0.0001f)
				{
					testAxes[axisCount++] = glm::normalize(cross);
				}
			}
		}

		glm::vec3 aHalfExtent = a._halfExtent;
		glm::vec3 bHalfExtent = b._halfExtent;

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];

			float projA = project_extent_along_axis(aRotationMat, aHalfExtent, axis);
			float projB = project_extent_along_axis(bRotationMat, bHalfExtent, axis);

			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return { false };
		}

		glm::vec3 pointOnA = a._center;

		for (int i = 0; i < 3; i++)
		{
			float dist = glm::dot(b._center - a._center, axisA[i]);
			dist = glm::clamp(dist, -aHalfExtent[i], aHalfExtent[i]);
			pointOnA += axisA[i] * dist;
		}

		glm::vec3 pointOnB = b._center;

		for (int i = 0; i < 3; i++)
		{
			float dist = glm::dot(a._center - b._center, axisB[i]);
			dist = glm::clamp(dist, -bHalfExtent[i], bHalfExtent[i]);
			pointOnB += axisB[i] * dist;
		}

		return { true, 0.5f * (pointOnA + pointOnB) };
	}


	TouchingData BoundTypeBase::full_sat_check(const OBB& a, const AABB& b) const
	{
		glm::vec3 centerDiff = b._center - a._center;

		std::array<glm::vec3, 3> axisA = {
			glm::vec3(a.get_rotation_matrix()[0]),
			glm::vec3(a.get_rotation_matrix()[1]),
			glm::vec3(a.get_rotation_matrix()[2])
		};

		std::array<glm::vec3, 3> axisB = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};

		std::array<glm::vec3, 15> testAxes{};
		int axisCount = 0;

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisA[i];

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisB[i];

		for (const auto& axis1 : axisA)
		{
			for (const auto& axis2 : axisB)
			{
				glm::vec3 cross = glm::cross(axis1, axis2);
				float lengthSquared = glm::dot(cross, cross);

				if (lengthSquared > 0.0001f)
				{
					testAxes[axisCount++] = glm::normalize(cross);
				}
			}
		}


		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];
			float projA = project_extent_along_axis(axisA, a._halfExtent, axis);
			float projB = project_extent_along_axis(axisB, b._halfExtent, axis);
			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return { false };
		}

		glm::vec3 pointOnA = a._center;

		for (int i = 0; i < 3; i++)
		{
			float dist = glm::dot(b._center - a._center, axisA[i]);
			dist = glm::clamp(dist, -a._halfExtent[i], a._halfExtent[i]);
			pointOnA += axisA[i] * dist;
		}

		glm::vec3 pointOnB = glm::clamp(a._center, b._center - b._halfExtent, b._center + b._halfExtent);


		return { true, 0.5f * (pointOnA + pointOnB) };
	}


	TouchingData BoundTypeBase::aabb_cast_check(const OBB& a, const AABB& b) const
	{
		if (a._max.x >= b._min.x && a._min.x <= b._max.x &&
			a._max.y >= b._min.y && a._min.y <= b._max.y &&
			a._max.z >= b._min.z && a._min.z <= b._max.z)
		{
			return { true, (glm::max(a._min, b._min) + glm::min(a._max, b._max)) * 0.5f };
		}
		return { false };
		}


	TouchingData BoundTypeBase::aabb_cast_check(const OBB& a, const OBB& b) const
	{

		if (a._max.x >= b._min.x && a._min.x <= b._max.x &&
			a._max.y >= b._min.y && a._min.y <= b._max.y &&
			a._max.z >= b._min.z && a._min.z <= b._max.z)
		{
			return { true, (glm::max(a._min, b._min) + glm::min(a._max, b._max)) * 0.5f };
		}
		return { false };
	}


	TouchingData BoundTypeBase::partial_sat_check(const OBB& a, const AABB& b) const
	{
		glm::vec3 centerDiff = b._center - a._center;

		std::array<glm::vec3, 3> axisA = {
			glm::vec3(a.get_rotation_matrix()[0]),
			glm::vec3(a.get_rotation_matrix()[1]),
			glm::vec3(a.get_rotation_matrix()[2])
		};

		std::array<glm::vec3, 3> axisB = {
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, 0, 1)
		};

		std::array<glm::vec3, 6> testAxes;
		int axisCount = 0;

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisA[i];

		for (int i = 0; i < 3; i++)
			testAxes[axisCount++] = axisB[i];

		glm::vec3 aHalfExtent = a._halfExtent;
		glm::vec3 bHalfExtent = b._halfExtent;

		for (int i = 0; i < axisCount; i++)
		{
			const glm::vec3& axis = testAxes[i];

			float projA = project_extent_along_axis(axisA, aHalfExtent, axis);
			float projB = project_extent_along_axis(axisB, bHalfExtent, axis);

			float distance = std::abs(glm::dot(centerDiff, axis));

			if (distance > projA + projB)
				return { false };  // No collision
		}

		glm::vec3 pointOnA = a._center;

		for (int i = 0; i < 3; i++)
		{
			float dist = glm::dot(b._center - a._center, axisA[i]);
			dist = glm::clamp(dist, -a._halfExtent[i], a._halfExtent[i]);
			pointOnA += axisA[i] * dist;
		}

		glm::vec3 pointOnB = glm::clamp(a._center, b._center - b._halfExtent, b._center + b._halfExtent);

		return { true, 0.5f * (pointOnA + pointOnB) };
	}

	glm::vec3 BoundTypeBase::get_center() const
	{
		return _center;
	}

	float BoundTypeBase::get_volume() const
	{
		return (_max.x - _min.x) * (_max.y - _min.y) * (_max.z - _min.z);
	}

	TouchingData BoundTypeBase::is_touching(BoundTypeBase* other)
	{
		switch (other->get_bound_type())
		{
		case glType::BoundType::AABB:
			return is_touching(*static_cast<AABB*>(other));
			break;
		case glType::BoundType::OBB:
			return is_touching(*static_cast<OBB*>(other));
			break;
		case glType::BoundType::Sphere:
			return is_touching(*static_cast<SphereBound*>(other));
			break;
		default:
			return { false };
			break;
		}
	}

	float BoundTypeBase::project_extent_along_axis(const glm::mat4& rotationMat, const glm::vec3& halfExtent, const glm::vec3& axis) const
	{
		std::array<glm::vec3, 3> _OBBAxes = {
			rotationMat[0],
			rotationMat[1],
			rotationMat[2]
		};

		return  std::abs(glm::dot(_OBBAxes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(_OBBAxes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(_OBBAxes[2], axis)) * halfExtent.z;
	}

	float BoundTypeBase::project_extent_along_axis(const std::array<glm::vec3, 3>& axes, const glm::vec3& halfExtent, const glm::vec3& axis) const
	{
		return std::abs(glm::dot(axes[0], axis)) * halfExtent.x +
			std::abs(glm::dot(axes[1], axis)) * halfExtent.y +
			std::abs(glm::dot(axes[2], axis)) * halfExtent.z;
	}


	glm::vec3 find_center_of_mass(std::vector<glType::Vertex> verts, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{
		if (verts.empty())
		{
			return glm::vec3(0.0f);
		}
		std::vector<glm::vec3> vertices;
		for (size_t i = 0; i < verts.size(); i += static_cast<size_t>(fullStride))
		{
			glm::vec3 vertex;
			vertex.x = verts[i];
			vertex.y = verts[i + 1];
			if (posStride == glUtil::PosStride::STRIDE_3D)
			{
				vertex.z = verts[i + 2];
			}
			else if (posStride == glUtil::PosStride::STRIDE_2D)
			{
				vertex.z = 0.0f;
			}
			vertices.push_back(vertex);
		}
		glm::vec3 centerOfMass(0.0f);
		for (const auto& vertex : vertices)
		{
			centerOfMass += vertex;
		}
		centerOfMass /= vertices.size();
		return centerOfMass;
	}

	glm::vec3 find_center_of_mass(std::vector<glm::vec3> verts)
	{
		if (verts.empty())
		{
			return glm::vec3(0.0f);
		}
		
		glm::vec3 centerOfMass(0.0f);
	
		for (const auto& vertex : verts)
		{
			centerOfMass += vertex;
		}
		centerOfMass /= verts.size();

		return centerOfMass;
	}

	MinMax get_min_max_from_vertices(const std::vector<glType::Vertex>& verts, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{

		if (verts.empty())
		{
			return { glm::vec3(0.0f), glm::vec3(0.0f) };
		}

		std::vector<glm::vec3> vertices;

		for (size_t i = 0; i < verts.size(); i += static_cast<size_t>(fullStride))
		{

			glm::vec3 vertex;


			vertex.x = verts[i];
			vertex.y = verts[i + 1];

			if (posStride == glUtil::PosStride::STRIDE_3D)
			{
				vertex.z = verts[i + 2];
			}
			else if (posStride == glUtil::PosStride::STRIDE_2D)
			{
				vertex.z = 0.0f;
			}

			vertices.push_back(vertex);
		}

		glm::vec3 minVec = vertices[0];
		glm::vec3 maxVec = vertices[0];

		for (const auto& vertex : vertices)
		{
			minVec = glm::min(minVec, vertex);
			maxVec = glm::max(maxVec, vertex);
		}

		return { minVec, maxVec };
	}

	MinMax get_min_max_of_mesh(const glUtil::Mesh& mesh, glUtil::FullStride fullStride, glUtil::PosStride posStride)
	{
		auto vert = mesh.get_verticies();
		if (vert.empty())
		{
			return { glm::vec3(0.0f), glm::vec3(0.0f) };
		}
		return get_min_max_from_vertices(vert, fullStride, posStride);
	}

} // namespace physics