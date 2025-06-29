#pragma once

#include "config.h"

#include "tools\include\stride.h"
#include "glUtil\include\mesh_bundle.h"


namespace physics
{
	class PhysicsBodyBase;
}


namespace glUtil
{

	class Mesh : public glType::Component<Mesh>
	{
	public:
		Mesh() = default;

		Mesh(const MeshBundle& bundle);

		Mesh(Mesh&& other) noexcept = default;
		Mesh& operator=(Mesh&& other) noexcept = default;

		glUtil::Mesh(const glUtil::Mesh&) = delete;
		glUtil::Mesh& operator=(const glUtil::Mesh&) = delete;

		void init(const MeshBundle& bundle);

		void render();
	
		void change_position(const glm::vec3& pos);

		void set_position(const glm::vec3& pos);

		void change_rotation(const glm::quat& rotation);
		void change_rotation(const glm::vec3& rotation);

		void set_rotation(const glm::quat& rotation);
		void set_rotation(const glm::vec3& rotation);

		void change_scale(const glm::vec3& scale);

		void set_scale(const glm::vec3& scale);

		Transform get_transform() const;

		glm::vec3 get_position() const;

		glm::mat4 get_model_matrix() const;

		void clear();

		virtual ~Mesh();

		private:

		unsigned int _VAO = 0, _VBO = 0, _IBO = 0;
		size_t _indexCount = 0;
		size_t _vertexCount = 0;

		bool _indexed = true;

		unsigned int _offsetCount = 0;

		Transform _transform;

		friend class physics::PhysicsBodyBase;

	private:
		void set_vertex_attribs(const ArrayBufferLayout& arrLayout, FullStride absoluteStride);

	};

}

namespace tools
{

	std::vector<glType::Vertex> create_circle_vertices(float radius, int segments, const glm::vec3& center, const glm::vec3& color);

	std::vector<glType::Index> create_circle_indices(int segments);

	std::vector<glType::Vertex> create_cube_vertices(const glm::vec3& center, const glm::vec3& color, float size);
	
	std::vector<glType::Index> create_cube_indices();

	std::vector<glType::Vertex> create_floor_vertices(const glm::vec3& color, const glm::vec3& position, float size);
	
	std::vector<glType::Index> create_floor_indices();

	glUtil::Mesh construct_default_mesh();

	glUtil::Mesh construct_default_floor_mesh();
}
