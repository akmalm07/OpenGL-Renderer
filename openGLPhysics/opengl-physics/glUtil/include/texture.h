#pragma once

#include "config.h"

namespace glInit
{
	class GLProgram;
}

namespace glUtil
{

	enum class TextureUnit
	{
		TEX0 = 0,
		TEX1 = 1,
		TEX2 = 2,
		TEX3 = 3,
		TEX4 = 4,
		TEX5 = 5,
		TEX6 = 6,
		TEX7 = 7,
		TEX8 = 8,
		TEX9 = 9,
		TEX10 = 10,

	};

	class Texture : public glType::Component<Texture> // ADD Mip map level 
	{
	public:
		Texture();
		Texture(glInit::GLProgram& program, const std::filesystem::path& texAddress, TextureUnit unit = TextureUnit::TEX0);

		std::filesystem::path get_path() const;

		bool init(glInit::GLProgram& program, const std::filesystem::path& path, TextureUnit texUnit = TextureUnit::TEX0);

		void communicate_impl(glType::Entity entity);

		bool is_loaded() const;

		void set_texture_loc(unsigned int location);

		void bind();
		
		void unbind();

		void clear_texture();

		~Texture();

	private:

		TextureUnit _textureUnit = TextureUnit::TEX0;

		unsigned int _textureLocation = 0;

		unsigned int _textureId = 0;
		int _height = 0, _width = 0, _bitDepth = 0;

		unsigned int _hasTextureLoc = 0;

		glType::Entity _entityId = 0;

		bool _isLoaded = false;
		
		std::filesystem::path _texturePath;
	};
}