#pragma once

#include <stdio.h>

#include "fatptr.h"

typedef struct { void (*print)(FILE *out, const CT_Var); } CT_Print;

extern CT_Typedef Type_Print;

void init_type_print() __attribute__((constructor));

ct_inline void ct_print_to(FILE *out, CT_Var x) {
  ct_protocol_lookup(Type_Print, CT_Print, x)->print(out, x);
}

ct_inline void ct_print(CT_Var x) {
  ct_protocol_lookup(Type_Print, CT_Print, x)->print(stdout, x);
}
