#pragma once

#include "fatptr.h"

#define $str(x) $(Type_String, CT_String, x)

typedef struct { char *val; } CT_String;

extern CT_Typedef Type_String;

void init_type_string() __attribute__((constructor));
