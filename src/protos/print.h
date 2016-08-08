#pragma once

#include "fatptr.h"

typedef struct { void (*print)(const CT_Var); } CT_Print;

extern CT_Typedef Type_Print;

void init_type_print() __attribute__((constructor(10)));

ct_inline void print(CT_Var x) {
  protocol_lookup(Type_Print, CT_Print, x)->print(x);
}
