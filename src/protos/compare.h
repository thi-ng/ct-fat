#pragma once

#include "fatptr.h"

ct_declare_type(Type_Compare, compare);

typedef struct { int (*compare)(const CT_Var, const CT_Var); } CT_Compare;

ct_inline int ct_compare(const CT_Var a, const CT_Var b) {
  ct_protocol_call(Type_Compare, CT_Compare, compare, a, a, b);
  const CT_Typedef *ta = ct_typeof(a);
  const CT_Typedef *tb = ct_typeof(b);
  if (ta == tb) {
    return memcmp(a, b, ta->size);
  }
  return 0;  // FIXME throw
}
