#pragma once

#include "fatptr.h"

ct_declare_type(Type_Len, len);

typedef struct { size_t (*length)(const CT_Var); } CT_Len;

size_t ct_length(const CT_Var self);
