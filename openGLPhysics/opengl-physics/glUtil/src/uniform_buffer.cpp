#include "headers.h"
#include "glUtil/include/uniform_buffer.h"

namespace glUtil
{
    UniformBuffer::UniformBuffer()
    {
    }
    UniformBuffer::UniformBuffer(bool debug)
    {
        _debug = debug;
    }

    UniformBuffer::UniformBuffer(const UniformBuffer& other)
    {
        _ubo = 0;
        _bindingPoint = other._bindingPoint;
        _totalSize = other._totalSize;
        _usage = other._usage;
        _isInit = false;
        _debug = other._debug;
        _offsets = other._offsets;
        _sizes = other._sizes;

    }

    UniformBuffer& UniformBuffer::operator=(const UniformBuffer& other)
    {
        if (this != &other)
        {
            destroy();

            _ubo = 0;
            _bindingPoint = other._bindingPoint;
            _totalSize = other._totalSize;
            _usage = other._usage;
            _isInit = false;
            _debug = other._debug;
            _offsets = other._offsets;
            _sizes = other._sizes;

        }
        return *this;
    }

    UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept
    {
        *this = std::move(other);
    }

    UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept
    {
        if (this != &other)
        {
            destroy();
            _ubo = other._ubo;
            _bindingPoint = other._bindingPoint;
            _totalSize = other._totalSize;
            _usage = other._usage;
            _isInit = other._isInit;
            _debug = other._debug;
            _offsets = std::move(other._offsets);
            _sizes = std::move(other._sizes);
            other._ubo = 0;
            other._isInit = false;
        }
        return *this;
    }

    bool UniformBuffer::init(unsigned int programID, std::string_view blockName, unsigned int bindingPoint, bool usage)
    {
        _bindingPoint = bindingPoint;
        _usage = (usage ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);

        GLuint blockIndex = glGetUniformBlockIndex(programID, blockName.data());
        if (blockIndex == GL_INVALID_INDEX)
        {
            std::cerr << "Uniform block '" << blockName << "' not found." << std::endl;
            return false;
        }

        GLint blockSize = 0;
        glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
        _totalSize = blockSize;

        if (_debug)
        {
            std::cout << "UBO block size: " << _totalSize << " bytes" << std::endl;
        }

        glGenBuffers(1, &_ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, _ubo);
        glBufferData(GL_UNIFORM_BUFFER, _totalSize, nullptr, _usage);
        glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubo);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        GLint activeUniforms = 0;
        glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &activeUniforms);

        std::vector<GLuint> uniformIndices(activeUniforms);
        glGetActiveUniformBlockiv(programID, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, reinterpret_cast<GLint*>(uniformIndices.data()));

        std::vector<GLint> offsets(activeUniforms);
        std::vector<GLint> sizes(activeUniforms);
        glGetActiveUniformsiv(programID, static_cast<GLsizei>(uniformIndices.size()), uniformIndices.data(), GL_UNIFORM_OFFSET, offsets.data());
        glGetActiveUniformsiv(programID, static_cast<GLsizei>(uniformIndices.size()), uniformIndices.data(), GL_UNIFORM_SIZE, sizes.data());

        for (int i = 0; i < activeUniforms; ++i)
        {
            char nameBuffer[128];
            GLsizei length = 0;
            glGetActiveUniformName(programID, uniformIndices[i], sizeof(nameBuffer), &length, nameBuffer);

            std::string uniformName(nameBuffer, length);
            _offsets.emplace(uniformName, offsets[i]);
            _sizes.emplace(uniformName, sizes[i]);

            if (_debug)
            {
                std::cout << "Uniform '" << uniformName << "' offset: " << offsets[i] << ", size: " << sizes[i] << std::endl;
            }
        }

        _isInit = true;
        return true;
    }


    void UniformBuffer::bind() const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _ubo);
    }

    void UniformBuffer::unbind() const
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, 0);
    }

    void UniformBuffer::destroy()
    {
        if (_ubo != 0)
        {
            glDeleteBuffers(1, &_ubo);
            _ubo = 0;
        }
        _offsets.clear();
        _sizes.clear();
        _isInit = false;
    }

    UniformBuffer::~UniformBuffer()
    {
        destroy();
    }


}
