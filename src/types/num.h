#pragma once

#include "fatptr.h"

#define $i32(x) $(Type_I32, CT_I32, 4, (int32_t)x)
#define $i64(x) $(Type_I64, CT_I64, 8, (int64_t)x)
#define $u32(x) $(Type_U32, CT_U32, 4, (uint32_t)x)
#define $u64(x) $(Type_U64, CT_U64, 8, (uint64_t)x)
#define $f32(x) $(Type_F32, CT_F32, 4, (float)x)
#define $f64(x) $(Type_F64, CT_F64, 8, (double)x)

#define ct_new_i32() ct_new(&Type_I32)
#define ct_new_i64() ct_new(&Type_I64)
#define ct_new_u32() ct_new(&Type_U32)
#define ct_new_u64() ct_new(&Type_U64)
#define ct_new_f32() ct_new(&Type_F32)
#define ct_new_f64() ct_new(&Type_F64)

typedef int32_t CT_I32;
typedef int64_t CT_I64;
typedef uint32_t CT_U32;
typedef uint64_t CT_U64;
typedef float CT_F32;
typedef double CT_F64;

extern CT_Typedef Type_I32;
extern CT_Typedef Type_I64;
extern CT_Typedef Type_U32;
extern CT_Typedef Type_U64;
extern CT_Typedef Type_F32;
extern CT_Typedef Type_F64;

void ct_init_type_num() __attribute__((constructor));
