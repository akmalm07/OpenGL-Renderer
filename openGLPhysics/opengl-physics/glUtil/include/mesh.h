#pragma once

#include "headers.h"

#include "tools\include\stride.h"
#include "glUtil\include\mesh_bundle.h"


namespace glUtil
{
	class Mesh
	{
	public:
		Mesh();

		Mesh(bool debugMode);

		Mesh(const MeshBundle& bundle, bool debugMode);

		Mesh(Mesh&& other) noexcept;

		Mesh& operator=(Mesh&& other) noexcept;

		bool is_init() const;

		void init(const MeshBundle& bundle, bool debugMode);

		void init(const MeshBundle& bundle);

		virtual void add_gravity(const glm::vec3& val);

		virtual glm::mat4 get_model_matrix() const;

		virtual void render();

		void clear();

		virtual ~Mesh();
	protected:

		unsigned int VAO, VBO, IBO;
		size_t indexCount;
		size_t vertexCount;

		bool  isInit = false;
		bool indexed = true;

		bool debug = false;

		unsigned int offsetCount = 0;


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
}
