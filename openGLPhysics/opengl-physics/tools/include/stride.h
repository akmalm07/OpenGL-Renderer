#pragma once
#include "config.h"

#include "tools/include/types.h"



enum class Stride : uint8_t
{
	NONE = 0,
	STRIDE_2D,
	STRIDE_3D,
	STRIDE_4D
};


enum class FullStride
{
	NONE = 0,
	STRIDE_2D = 2,
	STRIDE_3D,
	STRIDE_4D,
	STRIDE_5D,
	STRIDE_6D,
	STRIDE_7D,
	STRIDE_8D,
	STRIDE_9D,
	STRIDE_10D,
	STRIDE_11D,
	STRIDE_12D,
};

enum class PosStride : uint8_t 
{
	NONE = 0,
	STRIDE_2D,
	STRIDE_3D
};

enum class ColorStride : uint8_t
{
	NONE = 0,
	RGB,
	RGBA
};

enum class NormalStride : uint8_t
{
	NONE = 0,
	STRIDE_2D,
	STRIDE_3D
};
	
enum class TextureStride : uint8_t
{
	NONE = 0,
	STRIDE_2D,
	STRIDE_3D
};


enum class StrideType : uint8_t
{
	POS = 0,
	COL,
	NORM,
	TEX
};


enum class BufferLayout : uint8_t
{
	BUFFER_1 = 0,
	BUFFER_2,
	BUFFER_3,
	BUFFER_4
};

struct ArrayBufferLayout
{
	StrideType type;
	Stride stride;
	size_t location = 0;
	//BufferLayout buffer = BufferLayout::BUFFER_1;
	//size_t offset = 0;

};



struct StrideBundle
{
	PosStride     pos;
	ColorStride   col;
	NormalStride  norm;
	TextureStride tex;
};



uint8_t enumerate_pos_stride(PosStride stride); 
uint8_t enumerate_color_stride(ColorStride stride);
uint8_t enumerate_tex_stride(TextureStride stride);
uint8_t enumerate_normal_stride(NormalStride stride);

uint8_t enumerate_stride(Stride stride);
uint8_t enumerate_stride(FullStride stride);



template <glType::GLMVec T>	
PosStride enumify_pos_type()
{
	if constexpr (std::is_same_v<T, glm::vec2>)   
	{
		return PosStride::STRIDE_2D;
	}
	else if constexpr (std::is_same_v<T, glm::vec3>) 
	{
		return PosStride::STRIDE_3D;
	}
	else if constexpr (std::is_same_v<T, std::nullptr_t>) 
	{
		return PosStride::NONE; 
	}
	else 
	{
		std::cout << "Error: an unknown type was entered for Position Stride!\n";
		return PosStride::NONE; 
	}
}

template <glType::GLMVec T>	
ColorStride enumify_col_type()
{
	if constexpr (std::is_same_v<T, glm::vec4>)
	{
		return ColorStride::RGBA;
	}
	else if constexpr (std::is_same_v<T, glm::vec3>)
	{
		return ColorStride::RGB;
	}
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
	{
		return ColorStride::NONE;
	}
	else
	{
		std::cout << "Error: an unknown type was entered for Color Stride!\n";
		return ColorStride::NONE;
	}
}

template <glType::GLMVec T>	
NormalStride enumify_norm_type()
{
	if constexpr (std::is_same_v<T, glm::vec2>)
	{
		return NormalStride::STRIDE_2D;
	}
	else if constexpr (std::is_same_v<T, glm::vec3>)
	{
		return NormalStride::STRIDE_3D;
	}
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
	{
		return NormalStride::NONE;
	}
	else
	{
		std::cout << "Error: an unknown type was entered for Normal Stride!\n";
		return NormalStride::NONE;
	}
}

template <glType::GLMVec T>	
TextureStride enumify_tex_type()
{
	if constexpr (std::is_same_v<T, glm::vec2>)
	{
		return TextureStride::STRIDE_2D;
	}
	else if constexpr (std::is_same_v<T, glm::vec3>)
	{
		return TextureStride::STRIDE_3D;
	}
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
	{
		return TextureStride::NONE;
	}
	else
	{
		std::cout << "Error: an unknown type was entered for Texture Stride!\n";
		return TextureStride::NONE;
	}
}




template <glType::GLMVec T>	
uint8_t enumerate_type()
{
	if constexpr (std::is_same_v<T, glm::vec2>)
	{
		return 2;
	}
	else if constexpr (std::is_same_v<T, glm::vec3>)
	{
		return 3;
	}
	else if constexpr (std::is_same_v<T, glm::vec4>)
	{
		return 4;
	}
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
	{
		return 0;
	}
	else
	{
		std::cout << "Error: an unknown type was entered!\n";
		return 0;
	}
}


template <glType::GLMVec T>
constexpr Stride enumify_type() 
{
	if constexpr (std::same_as<T, glm::vec2>)
	{
		return Stride::STRIDE_2D;
	}
	else if constexpr (std::same_as<T, glm::vec3>)
	{
		return Stride::STRIDE_3D;
	}
	else if constexpr (std::same_as<T, glm::vec4>)
	{
		return Stride::STRIDE_4D;
	}
	else if constexpr (std::same_as<T, std::nullptr_t>)
	{
		return Stride::NONE;
	}
	else
	{
		std::cout << "Error: an unknown type was entered for Stride!\n";
		return Stride::NONE;
	}
}

template <class T>
concept Enum = std::is_same_v<T, PosStride> || std::is_same_v<T, ColorStride> ||
	std::is_same_v<T, NormalStride> || std::is_same_v<T, TextureStride> || std::is_same_v<T, Stride>;
