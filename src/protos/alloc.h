#pragma once

#include "fatptr.h"

ct_declare_type(Type_Alloc, alloc);

typedef struct {
  CT_Var (*alloc)();
  void (*dealloc)(CT_Var);
} CT_Alloc;

CT_Var ct_new(const CT_Typedef *type);
