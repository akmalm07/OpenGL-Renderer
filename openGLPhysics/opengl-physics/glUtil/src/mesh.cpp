#include "headers.h"
#include "glUtil/include/mesh.h"


//bool Mesh::_firstInstance = true;
namespace glUtil
{

	Mesh::Mesh()
	{
		VAO = 0;
		VBO = 0;
		IBO = 0;
		indexCount = 0;
		debug = 0;
	}

	Mesh::Mesh(bool debugMode) : Mesh()
	{
		debug = debugMode;
	}

	Mesh::Mesh(const MeshBundle& bundle, bool debugMode)
	{
		debug = debugMode;

		init(bundle);
	}


	Mesh::Mesh(Mesh&& other) noexcept = default;

	Mesh& Mesh::operator=(Mesh&& other) noexcept = default;


	bool Mesh::is_init() const
	{
		return isInit;
	}


	void Mesh::init(const MeshBundle& bundle, bool debugMode)
	{
		debug = debugMode;
		init(bundle);
	}

	void Mesh::init(const MeshBundle& bundle)
	{
		indexCount = bundle.indexCount;
		vertexCount = bundle.vertexCount;

		indexed = (bundle.indexed || bundle.indexCount == 0 ? true : false);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		if (debug)
		{
			std::cout << "Creating VAO ID: " << VAO << "\n";
		}

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glType::Vertex) * bundle.vertexCount, bundle.pVertices, GL_STATIC_DRAW);

		if (debug)
		{
			std::cout << "\tCreating VBO ID: " << VBO << "\n";
			std::cout << "\tVertex Count: " << bundle.vertexCount << " * " << sizeof(glType::Vertex) << "\n";
			std::cout << "\tBuffer Data: " << "\n";
			for (size_t i = 0; i < bundle.vertexCount; i++)
			{
				if (i % SIZET(bundle.fullStride) == 0)
				{
					std::cout << "\n\t\t";
				}
				std::cout << bundle.pVertices[i] << " ";
			}
			std::cout << std::endl;

			std::cout << "\tIndex data: " << std::endl;

			for (size_t i = 0; i < bundle.indexCount; i++)
			{
				if (i % 3 == 0)
				{
					std::cout << "\n\t\t";
				}
				std::cout << bundle.pIndices[i] << " ";
			}
			std::cout << std::endl;
		}


		if (indexed)
		{
			glGenBuffers(1, &IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glType::Index) * bundle.indexCount, bundle.pIndices, GL_STATIC_DRAW);
		}

		if (bundle.pLayout1)
		{
			set_vertex_attribs(*bundle.pLayout1, bundle.fullStride);
		}
		if (bundle.pLayout2)
		{
			set_vertex_attribs(*bundle.pLayout2, bundle.fullStride);
		}
		if (bundle.pLayout3)
		{
			set_vertex_attribs(*bundle.pLayout3, bundle.fullStride);
		}
		if (bundle.pLayout4)
		{
			set_vertex_attribs(*bundle.pLayout4, bundle.fullStride);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (indexed)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		isInit = true;
	}

	void Mesh::add_gravity(const glm::vec3& val)
	{}

	glm::mat4 Mesh::get_model_matrix() const
	{
		return glm::mat4(1.0f);
	}

	void Mesh::render()
	{

		glBindVertexArray(VAO);

		if (indexed)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

		}
		glBindVertexArray(0);
	}



	void Mesh::clear()
	{
		if (VAO != 0) {
			glDeleteVertexArrays(1, &VAO);
			VAO = 0;
		}
		if (VBO != 0) {
			glDeleteBuffers(1, &VBO);
			VBO = 0;
		}
		if (IBO != 0) {
			glDeleteBuffers(1, &IBO);
			IBO = 0;
		}
		indexCount = 0;
	}

	Mesh::~Mesh()
	{
		clear();
	}

	void Mesh::set_vertex_attribs(const ArrayBufferLayout& arrLayout, FullStride absoluteStride)
	{

		unsigned int stride = enumerate_stride(arrLayout.stride);
		glVertexAttribPointer(arrLayout.location, stride, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * enumerate_stride(absoluteStride), (void*)offsetCount);
		glEnableVertexAttribArray(arrLayout.location);

		if (debug)
		{
			std::cout << "For VAO ID: " << VAO << "\n\tAttribute Location: " << arrLayout.location
				<< "\n\tStride: " << stride
				<< "\n\tOffset: " << offsetCount
				<< std::endl;
		}

		offsetCount += stride * sizeof(glType::Vertex);

	}



	std::vector<float> create_circle_vertices(float radius, int segments, const glm::vec3& center, const glm::vec3& color)
	{
		if (segments < 3)
		{
			std::cerr << "Error: Circle must have at least 3 segments." << std::endl;
			return {};
		}
		std::vector<float> vertices;
		vertices.push_back(center.x);
		vertices.push_back(center.y);
		vertices.push_back(center.z);

		vertices.push_back(color.r);
		vertices.push_back(color.g);
		vertices.push_back(color.b);

		for (int i = 0; i <= segments; i++)
		{
			float theta = 2.0f * glm::pi<float>() * (float)i / segments;

			float x = center.x + radius * cos(theta);
			float y = center.y + radius * sin(theta);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(center.z);

			vertices.push_back(color.r);
			vertices.push_back(color.g);
			vertices.push_back(color.b);
		}
		return vertices;
	}

}

namespace tools
{
	std::vector<unsigned int> create_circle_indices(int segments)
	{
		if (segments < 3)
		{
			std::cerr << "Error: Circle must have at least 3 segments." << std::endl;
			return {};
		}
		std::vector<unsigned int> indices;
		for (int i = 1; i <= segments; i++)
		{
			indices.push_back(0);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		indices.push_back(0);
		indices.push_back(segments + 1);
		indices.push_back(1);

		return indices;
	}

	std::vector<float> create_cube_vertices(const glm::vec3& center, const glm::vec3& color, float size) {
		std::vector<float> vertices;
		float halfSize = size / 2.0f;


		float cubeVertices[] = {
			-halfSize, -halfSize, halfSize, color.r, color.g, color.b,
			halfSize, -halfSize, halfSize, color.r, color.g, color.b, 
			halfSize, halfSize, halfSize, color.r, color.g, color.b, 
			-halfSize, halfSize, halfSize, color.r, color.g, color.b, 

			-halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 
			halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 
			halfSize, halfSize, -halfSize, color.r, color.g, color.b, 
			-halfSize, halfSize, -halfSize, color.r, color.g, color.b, 
		};


		for (int i = 0; i < 8; ++i)
		{
			vertices.push_back(cubeVertices[i * 6 + 0] + center.x);
			vertices.push_back(cubeVertices[i * 6 + 1] + center.y);
			vertices.push_back(cubeVertices[i * 6 + 2] + center.z);
			vertices.push_back(cubeVertices[i * 6 + 3]);
			vertices.push_back(cubeVertices[i * 6 + 4]);
			vertices.push_back(cubeVertices[i * 6 + 5]);
		}


		return vertices;
	}

	std::vector<unsigned int> create_cube_indices()
	{
		return
		{
			0, 1, 2,
			2, 3, 0,

			1, 5, 6,
			6, 2, 1,

			5, 4, 7,
			7, 6, 5,

			4, 0, 3,
			3, 7, 4,

			4, 5, 1,
			1, 0, 4,

			3, 2, 6,
			6, 7, 3
		};
	}


}
/*

unsigned int location = 0;
unsigned int sizeOfStride = 0;
unsigned int jump = 0;

if (!StrideComposition::get().initalized())
{
	throw std::runtime_error("Stride Composition is not initialized!");
}

switch (StrideComposition::get().get_pos())
{
case PosStride::STRIDE_3D:
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 3, 0);
	glEnableVertexAttribArray(location);
	jump = 3;
	break;
case PosStride::STRIDE_2D:
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 2, 0);
	glEnableVertexAttribArray(location);
	jump = 2;
	break;
}

location++;


switch (StrideComposition::get().get_col())
{
case ColorStride::RGB:
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 3, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 3;
	location++;
	break;
case ColorStride::RGBA:
	glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 4, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 4;
	location++;
	break;

}

switch (StrideComposition::get().get_norm())
{
case NormalStride::STRIDE_3D:
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 3, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 3;
	location++;
	break;
case NormalStride::STRIDE_2D:
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 2, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 2;
	location++;
	break;
}

switch (StrideComposition::get().get_tex())
{
case TextureStride::STRIDE_2D:
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 2, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 2;
	location++;
	break;
case TextureStride::STRIDE_3D:
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * 3, (void*)(sizeof(glType::Vertex) * jump));
	glEnableVertexAttribArray(location);
	jump += 3;
	location++;
	break;
}*/
