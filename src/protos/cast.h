#pragma once

#include "fatptr.h"

ct_declare_type(Type_Cast, cast);

typedef struct { CT_Var (*cast)(const CT_Var, const CT_Typedef *); } CT_Cast;

ct_inline CT_Var ct_cast(const CT_Var x, const CT_Typedef *type) {
  const CT_Typedef *tx = ct_typeof(x);
  if (tx == type) {
    return x;
  }
  const CT_Cast *proto = ((CT_Cast *)(tx->impls[(Type_Cast).id]));
  if (proto && proto->cast) {
    return proto->cast(x, type);
  }
  return NULL;  // FIXME thow error
}
