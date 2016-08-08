#pragma once

#include "fatptr.h"

typedef struct { CT_Var (*cast)(const CT_Var, const CT_Typedef *); } CT_Cast;

extern CT_Typedef Type_Cast;

ct_inline CT_Var cast(const CT_Var x, const CT_Typedef *type) {
  return method_lookup(Type_Cast, CT_Cast, x)->cast(x, type);
}
