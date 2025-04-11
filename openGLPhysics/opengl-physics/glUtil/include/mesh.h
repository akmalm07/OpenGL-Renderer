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

		void render();

		void clear();

		~Mesh();
	private:

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

	std::vector<float> create_circle_vertices(float radius, int segments, const glm::vec3& center, const glm::vec3& color);

	std::vector<unsigned int> create_circle_indices(int segments);

	std::vector<float> create_cube_vertices(const glm::vec3& center, const glm::vec3& color, float size);

	std::vector<unsigned int> create_cube_indices();
}

//
	//static bool _firstInstance;
	//template <size_t VerticiesN, size_t IndiciesN>  
	//inline void init(const std::array<glType::Vertex, VerticiesN>& vertices, const std::array<glType::Index, IndiciesN>& indices);
//template <size_t VerticiesN, size_t IndiciesN>
//inline void Mesh::init(const std::array<glType::Vertex, VerticiesN>& vertices, const std::array<glType::Index, IndiciesN>& indices)
//{
//
//	indexCount = indices.size();
//
//	glGenVertexArrays(1, &VAO);
//	glBindVertexArray(VAO);
//
//	glGenBuffers(1, &VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(glType::Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
//
//	glGenBuffers(1, &IBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
//
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}