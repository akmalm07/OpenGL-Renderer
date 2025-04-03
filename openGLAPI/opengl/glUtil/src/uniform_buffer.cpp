#include "headers.h"
#include "glUtil/include/uniform_buffer.h"

namespace glUtil
{
    UniformBuffer::UniformBuffer() = default;

    UniformBuffer::UniformBuffer(bool debugMode)
    {
		debug = debugMode;
    }

    UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
        : UBO(other.UBO), totBufferSize(other.totBufferSize), bindingPoint(other.bindingPoint)
    {
        other.UBO = 0;
    }

    UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
    {
        if (this != &other)
        {
            glDeleteBuffers(1, &UBO);
            UBO = other.UBO;
            totBufferSize = other.totBufferSize;
            bindingPoint = other.bindingPoint;

            other.UBO = 0;
        }
        return *this;
    }


    void UniformBuffer::bind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    }

    void UniformBuffer::unbind() const
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    UniformBuffer::~UniformBuffer()
    {
        if (UBO != 0)
        {
            glDeleteBuffers(1, &UBO);
        }
    }
    unsigned int UniformBuffer::get_offset_from_index(size_t index)
    {
        if (index > bufferSizes.size())
        {
            std::cerr << "Uniform Buffer: Index provided is larger then the size!" << std::endl;
            return 0;
        }

        unsigned int offset = 0;

        for (size_t i = 0; i < index; i++)
        {
            offset += bufferSizes[index];
        }
		return offset;
    }
}
