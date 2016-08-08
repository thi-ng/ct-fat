#pragma once

#include "fatptr.h"

#define $vec4(x, y, z, w) $(Type_Vec4, CT_Vec4, x, y, z, w)

typedef struct { float x, y, z, w; } CT_Vec4;

extern CT_Typedef Type_Vec4;

void init_type_vec() __attribute__((constructor(101)));
