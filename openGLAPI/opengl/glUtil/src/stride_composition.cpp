#include "headers.h"
#include "glUtil/include/stride_composition.h"


StrideComposition::StrideComposition() = default;

StrideComposition::~StrideComposition() = default;

bool StrideComposition::initalized() const
{
	return _initialized;
}

StrideComposition& StrideComposition::get()
{
	static StrideComposition instance;
	return instance;
}

void StrideComposition::set_composition(PosStride posOption, ColorStride colOption, TextureStride texOption, NormalStride normOption)
{
	_posOption = posOption;
	_colOption = colOption;
	_texOption = texOption;
	_normOption = normOption;
	
	_initialized = true;
}

PosStride StrideComposition::get_pos() const
{
	return _posOption;
}

ColorStride StrideComposition::get_col() const
{
	return _colOption;
}

TextureStride StrideComposition::get_tex() const
{
	return _texOption;
}

NormalStride StrideComposition::get_norm() const
{
	return _normOption;
}