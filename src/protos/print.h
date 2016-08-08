#pragma once

#include "fatptr.h"

typedef struct { void (*print)(const CT_Var); } CT_Print;

extern CT_Typedef Type_Print;

ct_inline void print(CT_Var x) {
  protocol_lookup(Type_Print, CT_Print, x)->print(x);
}
