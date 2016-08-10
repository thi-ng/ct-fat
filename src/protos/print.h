#pragma once

#include "fatptr.h"

ct_declare_type(Type_Print, print);

typedef struct { void (*print)(FILE *out, const CT_Var); } CT_Print;

ct_inline void ct_print_to(FILE *out, CT_Var x) {
  ct_protocol_call_fast(Type_Print, CT_Print, print, x, out, x);
}

ct_inline void ct_print(CT_Var x) {
  ct_protocol_call_fast(Type_Print, CT_Print, print, x, stdout, x);
}
