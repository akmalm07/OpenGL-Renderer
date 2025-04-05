#pragma once

#include "config.h"


namespace glUtil
{
    class Texture
    {
    public:
        Texture();
        Texture(const std::string& texAddress);

        bool load_texture();
        bool is_loaded() const;
        void use_texture(GLenum texUnit, GLuint textureLoc);
        void clear_texture();

        ~Texture();

    private:
        std::string _textureLoc;

        unsigned int _textureId;
        int _height, _width, _bitDepth;
        bool _isLoaded;
    };
}