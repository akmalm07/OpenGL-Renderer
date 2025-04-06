#pragma once

#include "config.h"
#include "tools/include/stride.h"


class StrideComposition {
public:
    StrideComposition(const StrideComposition&) = delete;
    StrideComposition& operator=(const StrideComposition&) = delete;

    StrideComposition(StrideComposition&&) = default;
    StrideComposition& operator=(StrideComposition&&) = default;

    bool initalized() const;

    static StrideComposition& get();

    void set_composition(PosStride posOption, ColorStride colOption, TextureStride texOption, NormalStride normOption);

    PosStride get_pos() const;
    ColorStride get_col() const;
    TextureStride get_tex() const;
    NormalStride get_norm() const;

private:

    StrideComposition();
    ~StrideComposition();

    PosStride _posOption;
    ColorStride _colOption;
    TextureStride _texOption;
    NormalStride _normOption;

	bool _initialized = false;
};