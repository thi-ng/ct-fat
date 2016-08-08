#pragma once

#include "fatptr.h"

typedef struct { CT_Var (*cast)(const CT_Var, const CT_Typedef *); } CT_Cast;

extern CT_Typedef Type_Cast;

void init_type_cast() __attribute__((constructor(10)));

ct_inline CT_Var cast(const CT_Var x, const CT_Typedef *type) {
  return protocol_lookup(Type_Cast, CT_Cast, x)->cast(x, type);
}
