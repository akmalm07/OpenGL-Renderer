#include "headers.h"
#include "glUtil/include/texture.h"
#include <stb/stb_image.h>


namespace glUtil
{
    Texture::Texture() = default;


    Texture::Texture(const std::string& texAddress)
        : _textureLoc(texAddress), _textureId(0), _height(0), _width(0), _bitDepth(0), _isLoaded(false) {
    }


    bool Texture::load_texture()
    {
        unsigned char* texData = stbi_load(_textureLoc.c_str(), &_width, &_height, &_bitDepth, 0);
        if (!texData)
        {
            std::cerr << "Failed to find: " << _textureLoc << std::endl;
            return false;
        }

        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(texData);

        _isLoaded = true;
        return true;
    }

    bool Texture::is_loaded() const
    {
        return _isLoaded;
    }

    void Texture::use_texture(GLenum texUnit, GLuint textureLoc)
    {
        glActiveTexture(texUnit);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glUniform1i(textureLoc, texUnit - GL_TEXTURE0);
    }

    void Texture::clear_texture()
    {
        if (_textureId != 0)
        {
            glDeleteTextures(1, &_textureId);
            _textureId = 0;
        }
        _height = 0;
        _width = 0;
        _bitDepth = 0;
        _isLoaded = false;
    }

    Texture::~Texture()
    {
        clear_texture();
    }
}