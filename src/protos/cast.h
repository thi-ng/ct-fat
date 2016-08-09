#pragma once

#include "fatptr.h"

typedef struct { CT_Var (*cast)(const CT_Var, const CT_Typedef *); } CT_Cast;

extern CT_Typedef Type_Cast;

void init_type_cast() __attribute__((constructor));

ct_inline CT_Var ct_cast(const CT_Var x, const CT_Typedef *type) {
  ct_protocol_call(Type_Cast, CT_Cast, cast, x, x, type);
  return NULL;  // FIXME thow error
}
