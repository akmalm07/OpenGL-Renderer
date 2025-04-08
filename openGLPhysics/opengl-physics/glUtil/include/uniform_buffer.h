#pragma once

#include "config.h"

namespace glUtil
{
	class UniformBuffer
	{
	public:
		UniformBuffer();
		UniformBuffer(bool debug);

		UniformBuffer(const UniformBuffer&);
		UniformBuffer& operator=(const UniformBuffer&);

		UniformBuffer(UniformBuffer&& other) noexcept;
		UniformBuffer& operator=(UniformBuffer&& other) noexcept;

		bool init(unsigned int programID, std::string_view blockName, unsigned int bindingPoint, bool usage);
		
		void bind() const;
		void unbind() const;

		template <class T>
		void update_data(const T& data, const std::string& name);

		~UniformBuffer();

	private:
		unsigned int _ubo = 0;
		unsigned int _bindingPoint = 0;
		size_t _totalSize = 0;
		bool _isInit = false;
		bool _debug = true;
		int _usage = 0;

		std::unordered_map<std::string, int> _offsets;
		std::unordered_map<std::string, int> _sizes;

		//add types check later 
		//std::unordered_map<std::string, int> _types;

	private:
		void destroy();
	};

}

#include "uniform_buffer.inl"