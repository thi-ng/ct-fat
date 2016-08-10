#pragma once

#include "fatptr.h"

typedef struct { void (*print)(FILE *out, const CT_Var); } CT_Print;

extern CT_Typedef Type_Print;

void init_type_print() __attribute__((constructor));

ct_inline void ct_print_to(FILE *out, CT_Var x) {
  ct_protocol_call_fast(Type_Print, CT_Print, print, x, out, x);
}

ct_inline void ct_print(CT_Var x) {
  ct_protocol_call_fast(Type_Print, CT_Print, print, x, stdout, x);
}
