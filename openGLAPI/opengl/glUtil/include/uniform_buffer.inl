#pragma once 

#include "uniform_buffer.h"

namespace glUtil
{
	template <class T>
	UniformBuffer::UniformBuffer(unsigned int bindingPoint, const T& data, const std::vector<size_t>& sizes, bool debugMode)
		: bindingPoint(bindingPoint), bufferSizes(sizes)
	{
		size_t thesize = 0;
		for (const auto& size : sizes)
		{
			thesize += size; 
		}

		if (sizeof(data) == thesize)
		{
			totBufferSize = thesize;
		}
		else
		{
			std::cerr << "Error: Total size of data does not match the expected size for Uniform Buffer!" << std::endl;
			totBufferSize = 0;
			return;
		}
		
		allocate_buffer(data);
	}

	template<class T>
	inline UniformBuffer::UniformBuffer(unsigned int bindingPoint, const T& data, bool debugModel)
		: bindingPoint(bindingPoint), totBufferSize(sizeof(data)), debug(debugModel)
	{
		bufferSizes.push_back(sizeof(data));
		
		allocate_buffer(data);
	}

	template<class T>
	inline UniformBuffer::UniformBuffer(unsigned int bindingPoint, const T& data, size_t eachItemByte, size_t count, bool debugMode)
		: bindingPoint(bindingPoint), debug(debugMode)
	{
		for (size_t i = 0; i < count; ++i)
		{
			bufferSizes.push_back(eachItemByte);
		}

		if (eachItemByte * count == sizeof(data))
		{
			totBufferSize = eachItemByte * count; 
		}
		else
		{
			std::cerr << "Error: Total size of data does not match the expected size for Uniform Buffer!" << std::endl;
			totBufferSize = 0;
			return;
		}

		allocate_buffer(data);
	}

	template<class T>
	inline void UniformBuffer::update_data(const T& data, size_t index)
	{
		if (debug)
		{
			std::cout << "Updating Uniform Buffer at index: " << index << std::endl;
		}
		if (index >= bufferSizes.size())
		{
			std::cerr << "Error: Index out of range for Uniform Buffer update!" << std::endl;
			return;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, get_offset_from_index(index), bufferSizes[index], &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<class T>
	inline void UniformBuffer::update_data(const T& data)
	{
		if (debug)
		{

			std::cout << "Updating entire Uniform Buffer" << std::endl;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, UBO);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, totBufferSize, &data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	template<class T>
	void UniformBuffer::allocate_buffer(const T& data)
	{
		if (debug)
		{
			std::cout << "Allocating Uniform Buffer with size: " << totBufferSize << " for binding: " << bindingPoint << std::endl;
		}
		glGenBuffers(1, &UBO);
		glBindBuffer(GL_UNIFORM_BUFFER, UBO);

		if (sizeof(data) > totBufferSize)
		{
			std::cerr << "Error: Data size exceeds Uniform Buffer size!" << std::endl;
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			return;
		}

		glBufferData(GL_UNIFORM_BUFFER, totBufferSize, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, UBO);

		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), &data);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

} // namespace glUtil