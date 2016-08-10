#pragma once

#include "fatptr.h"

ct_declare_type(Type_Math, math);

typedef struct { CT_Var (*add)(CT_Var, const CT_Var); } CT_Math;

ct_inline CT_Var ct_add(CT_Var a, CT_Var b) {
  ct_protocol_call_fast(Type_Math, CT_Math, add, a, a, b);
  return NULL;  // FIXME
}
