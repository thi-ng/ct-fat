#pragma once

#include "fatptr.h"

#define $str(x) $(Type_String, CT_String, 4, x)

ct_declare_type(Type_String, string);

typedef struct { char *val; } CT_String;
