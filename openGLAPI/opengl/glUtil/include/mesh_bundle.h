#pragma once

#include "config.h"
#include "tools/include/types.h"
#include "tools/include/stride.h"


struct MeshBundle
{

    const glType::Vertex* pVertices;
    const glType::Index* pIndices;

	FullStride fullStride = FullStride::NONE; 

    ArrayBufferLayout* pLayout1 = nullptr;
    ArrayBufferLayout* pLayout2 = nullptr;
    ArrayBufferLayout* pLayout3 = nullptr;
    ArrayBufferLayout* pLayout4 = nullptr;

    size_t vertexCount = 0;
    size_t indexCount = 0;
    
    bool indexed = true;

};