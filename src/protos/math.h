#pragma once

#include "fatptr.h"

typedef struct { CT_Var (*add)(CT_Var, const CT_Var); } CT_Math;

extern CT_Typedef Type_Math;

ct_inline CT_Var add(CT_Var a, CT_Var b) {
  return method_lookup(Type_Math, CT_Math, a)->add(a, b);
}
