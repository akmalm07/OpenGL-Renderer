#include "headers.h"
#include "glUtil/include/mesh.h"


//bool Mesh::_firstInstance = true;
namespace glUtil
{
	Mesh::Mesh(const MeshBundle& bundle)
	{
		init(bundle);
	}


	void Mesh::init(const MeshBundle& bundle)
	{
		_indexCount = bundle.indexCount;
		_vertexCount = bundle.vertexCount;

		_indexed = (bundle.indexed || bundle.indexCount == 0 ? true : false);

		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);


		glGenBuffers(1, &_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glType::Vertex) * bundle.vertexCount, bundle.pVertices, GL_STATIC_DRAW);

		DEBUG(
			std::cout << "\tCreating _VBO ID: " << _VBO << "\n";
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
		
		)
			

		if (_indexed)
		{
			glGenBuffers(1, &_IBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
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

	}

	Transform Mesh::get_transform() const
	{
		return _transform;
	}

	glm::vec3 Mesh::get_position() const
	{
		return _transform.position;
	}

	glm::mat4 Mesh::get_model_matrix() const
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, _transform.position);
		//model = model * glm::toMat4(_transform.rotation);
		model = glm::scale(model, _transform.scale);
		return model;
	}

	void Mesh::change_position(const glm::vec3& pos)
	{
		_transform.position += pos;
	}

	void Mesh::set_position(const glm::vec3& pos)
	{
		_transform.position = pos;
	}

	void Mesh::change_rotation(const glm::quat& rotation)
	{
		_transform.rotation = _transform.rotation * rotation;
	}

	void Mesh::change_rotation(const glm::vec3& rotation)
	{
		_transform.rotation = _transform.rotation * glm::quat(rotation);
	}


	void Mesh::set_rotation(const glm::quat& rotation)
	{
		_transform.rotation = rotation;
	}

	void Mesh::set_rotation(const glm::vec3& rotation)
	{
		_transform.rotation = glm::quat(rotation);
	}

	void Mesh::change_scale(const glm::vec3& scale)
	{
		_transform.scale += scale;
	}

	void Mesh::set_scale(const glm::vec3& scale)
	{
		_transform.scale = scale;
	}

	void Mesh::render()
	{

		glBindVertexArray(_VAO);

		if (_indexed)
		{
			glDrawElements(GL_TRIANGLES, _indexCount, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, _vertexCount);
		}
		glBindVertexArray(0);
	}



	void Mesh::clear()
	{
		if (_VAO != 0) {
			glDeleteVertexArrays(1, &_VAO);
			_VAO = 0;
		}
		if (_VBO != 0) {
			glDeleteBuffers(1, &_VBO);
			_VBO = 0;
		}
		if (_IBO != 0) {
			glDeleteBuffers(1, &_IBO);
			_IBO = 0;
		}
		_indexCount = 0;
	}

	Mesh::~Mesh()
	{
		clear();
	}

	void Mesh::set_vertex_attribs(const ArrayBufferLayout& arrLayout, FullStride absoluteStride)
	{

		unsigned int stride = enumerate_stride(arrLayout.stride);
		glVertexAttribPointer(arrLayout.location, stride, GL_FLOAT, GL_FALSE, sizeof(glType::Vertex) * enumerate_stride(absoluteStride), (void*)_offsetCount);
		glEnableVertexAttribArray(arrLayout.location);

		DEBUG(
			std::cout << "For _VAO ID: " << _VAO << "\n\tAttribute Location: " << arrLayout.location
				<< "\n\tStride: " << stride
				<< "\n\tOffset: " << _offsetCount
				<< std::endl;
		)

		_offsetCount += stride * sizeof(glType::Vertex);
	}

}

namespace tools
{

	std::vector<glType::Vertex> create_circle_vertices(glType::Vertex radius, int segments, const glm::vec3& center, const glm::vec3& color)
	{
		if (segments < 3)
		{
			std::cerr << "Error: Circle must have at least 3 segments." << std::endl;
			return {};
		}
		std::vector<glType::Vertex> vertices;
		vertices.push_back(center.x);
		vertices.push_back(center.y);
		vertices.push_back(center.z);

		vertices.push_back(color.r);
		vertices.push_back(color.g);
		vertices.push_back(color.b);

		for (int i = 0; i <= segments; i++)
		{
			glType::Vertex theta = 2.0f * glm::pi<glType::Vertex>() * (glType::Vertex)i / segments;

			glType::Vertex x = center.x + radius * cos(theta);
			glType::Vertex y = center.y + radius * sin(theta);

			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(center.z);

			vertices.push_back(color.r);
			vertices.push_back(color.g);
			vertices.push_back(color.b);
		}
		return vertices;
	}


	std::vector<glType::Index> create_circle_indices(int segments)
	{
		if (segments < 3)
		{
			std::cerr << "Error: Circle must have at least 3 segments." << std::endl;
			return {};
		}
		std::vector<glType::Index> indices;
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

	std::vector<glType::Vertex> create_cube_vertices(const glm::vec3& color, const glm::vec3& center, glType::Vertex size)
	{
		std::vector<glType::Vertex> vertices;
		glType::Vertex halfSize = size / 2.0f;


		glType::Vertex cubeVertices[] = {
			-halfSize, -halfSize, halfSize, color.r, color.g, color.b,
			halfSize, -halfSize, halfSize, color.r, color.g, color.b, 
			halfSize, halfSize, halfSize, color.r, color.g, color.b, 
			-halfSize, halfSize, halfSize, color.r, color.g, color.b, 

			-halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 
			halfSize, -halfSize, -halfSize, color.r, color.g, color.b, 
			halfSize, halfSize, -halfSize, color.r, color.g, color.b, 
			-halfSize, halfSize, -halfSize, color.r, color.g, color.b, 
		};


		for (int i = 0; i < 8; i++)
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


	std::vector<glType::Index> create_cube_indices()
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
	std::vector<glType::Vertex> create_floor_vertices(const glm::vec3& color, const glm::vec3& position, glType::Vertex size)
	{
		std::vector<glType::Vertex> vertices = 
		{
			-size / 2.0f, position.y, size / 2.0f + position.z, color.r, color.g, color.b,
			size / 2.0f + position.x, position.y, size / 2.0f + position.z, color.r, color.g, color.b,
			size / 2.0f + position.x, position.y, -size / 2.0f + position.z, color.r, color.g, color.b,
			-size / 2.0f + position.x, position.y, -size / 2.0f + position.z, color.r, color.g, color.b,
		};


		return vertices;
	}


	std::vector<glType::Index> create_floor_indices()
	{
		return {
		0, 1, 2, 
		2, 3, 0 
		};
		
	}

	glUtil::Mesh construct_default_mesh()
	{
		glUtil::MeshBundle bundle;

		bundle.fullStride = glUtil::FullStride::STRIDE_6D;
		auto cubeVerts = tools::create_cube_vertices(glm::vec3(1.0f), glm::vec3(0.0f, 8.0, 0.0f), 2.0f);
		auto cubeIndices = tools::create_cube_indices();

		bundle.pVertices = cubeVerts.data();
		bundle.pIndices = cubeIndices.data();
		bundle.vertexCount = cubeVerts.size();
		bundle.indexCount = cubeIndices.size();

		bundle.indexed = true;

		glUtil::ArrayBufferLayout buff1{ glUtil::StrideType::POS, glUtil::Stride::STRIDE_3D, 0 };
		glUtil::ArrayBufferLayout buff2{ glUtil::StrideType::COL, glUtil::Stride::STRIDE_3D, 1 };

		bundle.pLayout1 = &buff1;
		bundle.pLayout2 = &buff2;

		return glUtil::Mesh(bundle);
	}

	glUtil::Mesh construct_default_floor_mesh()
	{
		glUtil::MeshBundle bundle;
		bundle.fullStride = glUtil::FullStride::STRIDE_6D;
		auto floorVerts = tools::create_floor_vertices(glm::vec3(1.0f), glm::vec3(0.0f, -3.0, 0.0f), 30.0f);
		auto floorIndices = tools::create_floor_indices();
		bundle.pVertices = floorVerts.data();
		bundle.pIndices = floorIndices.data();
		bundle.vertexCount = floorVerts.size();
		bundle.indexCount = floorIndices.size();
		glUtil::ArrayBufferLayout buff1{ glUtil::StrideType::POS, glUtil::Stride::STRIDE_3D, 0 };
		glUtil::ArrayBufferLayout buff2{ glUtil::StrideType::COL, glUtil::Stride::STRIDE_3D, 1 };
		bundle.pLayout1 = &buff1;
		bundle.pLayout2 = &buff2;

		return glUtil::Mesh(bundle);
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
