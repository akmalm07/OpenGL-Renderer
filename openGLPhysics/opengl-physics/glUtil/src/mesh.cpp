#include "headers.h"
#include "glUtil/include/mesh.h"


//bool Mesh::_firstInstance = true;
namespace glUtil {
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

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		if (debug) {
			std::cout << "Creating VAO ID: " << VAO << "\n";
		}

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glType::Vertex) * bundle.vertexCount, bundle.pVertices, GL_STATIC_DRAW);

		if (debug) {
			std::cout << "\tCreating VBO ID: " << VBO << "\n";
			std::cout << "\tVertex Count: " << bundle.vertexCount << " * " << sizeof(glType::Vertex) << "\n";
			std::cout << "\tBuffer Data: " << "\n";
			for (size_t i = 0; i < bundle.vertexCount; ++i)
			{
				if (i % SIZET(bundle.fullStride) == 0)
				{
					std::cout << "\n\t\t";
				}
				std::cout << bundle.pVertices[i] << " ";
			}
			std::cout << std::endl;
		}



		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glType::Index) * bundle.indexCount, bundle.pIndices, GL_STATIC_DRAW);

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		isInit = true;
	}

	void Mesh::render()
	{

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
