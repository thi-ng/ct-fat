#pragma once

#include "fatptr.h"

#define $vec4(x, y, z, w) $(Type_Vec4, CT_Vec4, 16, x, y, z, w)

ct_declare_type(Type_Vec4, vec);

typedef struct { float x, y, z, w; } CT_Vec4;
