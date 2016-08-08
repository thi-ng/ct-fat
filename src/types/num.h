#pragma once

#include "fatptr.h"

#define $i32(x) $(Type_I32, CT_I32, x)
#define $u32(x) $(Type_U32, CT_U32, x)
#define $f32(x) $(Type_F32, CT_F32, x)
#define $f64(x) $(Type_F64, CT_F64, x)

#define new_i32() ct_new(Type_I32, CT_I32)
#define new_u32() ct_new(Type_U32, CT_U32)
#define new_f32() ct_new(Type_F32, CT_F32)
#define new_f64() ct_new(Type_F64, CT_F64)

typedef struct { int32_t val; } CT_I32;
typedef struct { uint32_t val; } CT_U32;
typedef struct { float val; } CT_F32;
typedef struct { double val; } CT_F64;

extern CT_Typedef Type_I32;
extern CT_Typedef Type_U32;
extern CT_Typedef Type_F32;
extern CT_Typedef Type_F64;