#include "headers.h"
#include "glUtil/include/texture.h"
#include "glInit/include/program.h"
#include <stb/stb_image.h>


namespace glUtil
{
    Texture::Texture() = default;


    Texture::Texture(glInit::GLProgram& program, const std::filesystem::path& texAdress, TextureUnit unit)
        : _textureId(0), _height(0), _width(0), _bitDepth(0), _textureUnit(unit)
    {
        init(program, texAdress);
    }

    std::filesystem::path Texture::get_path() const
    {
        return _texturePath;
    }


    bool Texture::init(glInit::GLProgram& program, const std::filesystem::path& path, TextureUnit texUnit)
    {
		_texturePath = path;

		_textureUnit = texUnit;

		_hasTextureLoc = program.add_uniform("uUseTexture");


        unsigned char* texData = stbi_load(_texturePath.string().c_str(), &_width, &_height, &_bitDepth, 0);
        if (!texData)
        {
            std::cerr << "Failed to find: " << _texturePath << std::endl;
            return false;
        }

        int rgbVal = 0;
        if (_bitDepth == 3) 
        {
            rgbVal = GL_RGB;
        }
        else if (_bitDepth == 4) 
        {
            rgbVal = GL_RGBA;
        }
        else if (_bitDepth == 1) 
        {
            rgbVal = GL_LUMINANCE; 
        }
        else 
        {
            std::cerr << "Warning: Unknown bit depth: " << _bitDepth << std::endl;
        }

        glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, rgbVal, GL_UNSIGNED_BYTE, texData);
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

    void Texture::communicate_impl(glType::Entity entity)
    {
		_entityId = entity;
    }

    bool Texture::is_loaded() const
    {
        return _isLoaded;
    }

    void Texture::set_texture_loc(unsigned int location)
    {
        _textureLocation = location;
    }

    glType::Entity Texture::get_entity_id() const
    {
        return _entityId;
    }

    void Texture::bind()
    {
        
		glUniform1i(_hasTextureLoc, GL_TRUE);
        glActiveTexture(GL_TEXTURE0 + UINT(_textureUnit));
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glUniform1i(_textureLocation, UINT(_textureUnit));

    }

    void Texture::unbind()
    {
        glUniform1i(_hasTextureLoc, GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
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