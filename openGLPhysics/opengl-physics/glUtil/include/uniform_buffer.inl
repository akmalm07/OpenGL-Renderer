#pragma once 

#include "uniform_buffer.h"

namespace glUtil
{

    template <class T>
    void UniformBuffer::update_data(const T& data, const std::string& name)
    {
        if (!_isInit)
            return;

        auto it = _offsets.find(name);

        //for (const auto& [key, value] : _offsets) // debugging
        //{
        //	std::cout << "Key: " << key << ", Value: " << value << std::endl;
        //}

        if (it == _offsets.end())
        {
            std::cerr << "Uniform name '" << name << "' not found." << std::endl;
            return;
        }

        glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
        glBufferSubData(GL_UNIFORM_BUFFER, it->second, sizeof(T), &data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

} // namespace glUtil