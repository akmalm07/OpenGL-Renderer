#include "headers.h"

#include "tools\include\stride.h"



uint8_t enumerate_pos_stride(PosStride stride)
{
	switch (stride)
	{
	case PosStride::NONE:
		return 0;

	case PosStride::STRIDE_2D:
		return 2;

	case PosStride::STRIDE_3D:
		return 3;
	default:
		return 0;
	}
}


uint8_t enumerate_color_stride(ColorStride stride) 
{
	switch (stride)
	{
	case ColorStride::NONE:
		return 0;

	case ColorStride::RGB:
		return 2;

	case ColorStride::RGBA:
		return 3;
	default:
		return 0;
	}
}



uint8_t enumerate_tex_stride(TextureStride texStride)
{
	switch (texStride)
	{
	case TextureStride::NONE:
		return 0;

	case TextureStride::STRIDE_2D:
		return 2;

	case TextureStride::STRIDE_3D:
		return 3;
	default:
		return 0;
	}
}



uint8_t enumerate_normal_stride(NormalStride normStride)
{

	switch (normStride)
	{
	case NormalStride::NONE:
		return 0;

	case NormalStride::STRIDE_2D:
		return 2;

	case NormalStride::STRIDE_3D:
		return 3;
	default:
		return 0;
	}

}

uint8_t enumerate_stride(Stride stride)
{
	switch (stride)
	{
	case Stride::NONE:
		return 0;
		break;
	case Stride::STRIDE_2D:
		return 2;
		break;

	case Stride::STRIDE_3D:
		return 3;
		break;

	case Stride::STRIDE_4D:
		return 4;
		break;
	default:
		return 0;
		break;
	}
}

uint8_t enumerate_stride(FullStride stride)
{
	switch (stride)
	{
	case FullStride::STRIDE_2D:
		return 2;
		break;
	case FullStride::STRIDE_3D:
		return 3;
		break;
	case FullStride::STRIDE_4D:
		return 4;
		break;
	case FullStride::STRIDE_5D:
		return 5;
		break;
	case FullStride::STRIDE_6D:
		return 6;
		break;
	case FullStride::STRIDE_7D:
		return 7;
		break;
	case FullStride::STRIDE_8D:
		return 8;
		break;
	case FullStride::STRIDE_9D:
		return 9;
		break;
	case FullStride::STRIDE_10D:
		return 10;
		break;
	case FullStride::STRIDE_11D:
		return 11;
		break;
	case FullStride::STRIDE_12D:
		return 12;
		break;

	default:
		return 0;
		break;
	}
}
