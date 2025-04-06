#pragma once

#include "config.h"

namespace glUtil
{

	class UniformBuffer
	{
	public:

		UniformBuffer();
		
		UniformBuffer(bool debugMode);

		UniformBuffer(UniformBuffer&& other) noexcept; 
		UniformBuffer& operator=(UniformBuffer&& other) noexcept;

		UniformBuffer(const UniformBuffer&) = delete;
		UniformBuffer& operator=(const UniformBuffer&) = delete; 

		template <class T>
		UniformBuffer(unsigned int bindingPoint, const T& data, const std::vector<size_t>& sizesInBytes, bool debugMode);

		template <class T>
		UniformBuffer(unsigned int bindingPoint, const T& data, bool debugMode);

		template <class T>
		UniformBuffer(unsigned int bindingPoint, const T& data, size_t eachItemByte, size_t count, bool debugMode);

		template <class T>
		void init(unsigned int bindingPoint, const T& data, size_t eachItemByte, size_t count, bool debugMode);

		template <class T> // Critical Contition -- dod not TEST
		void init(unsigned int bindingPoint, const T& data, const std::vector<size_t>& sizesInBytes, bool debugMode);

		void bind() const;
		void unbind() const;

		template <class T>
		void update_data(const T& data, size_t index);
		
		template <class T>
		void update_data(const T& data);

		~UniformBuffer();

	private:
		unsigned int UBO;
		std::vector<size_t> bufferSizes;
		size_t totBufferSize = 0;
		unsigned int bindingPoint;

		bool isInit = false;

		bool debug = false;

	private:

		template<class T>
		void allocate_buffer(const T& data);

		unsigned int get_offset_from_index(size_t index);
	};


}

#include "uniform_buffer.inl"