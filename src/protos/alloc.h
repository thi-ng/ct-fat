#pragma once

#include "fatptr.h"

ct_declare_type(Type_Alloc, alloc);

typedef struct {
  CT_Var (*new)();
  void (*free)(CT_Var);
} CT_Alloc;

CT_Var ct_alloc(const CT_Typedef *type);

CT_Var ct_new(const CT_Typedef *type);
void ct_free(const CT_Var self);
