#pragma once

#include "config.h"


namespace glUtil
{
    class Texture
    {
    public:
        Texture();
        Texture(const std::filesystem::path& texAddress);

        std::filesystem::path get_path() const;

        bool load_texture();

        bool is_loaded() const;

        void use_texture(GLenum texUnit, GLuint textureLoc);

        void clear_texture();

        ~Texture();

    private:
        std::filesystem::path _textureLoc;

        unsigned int _textureId;
        int _height, _width, _bitDepth;

        bool _isLoaded;
    };
}