#pragma once

#include "fatptr.h"

#define $str(x) $(Type_String, CT_String, x)

typedef struct {
  char *val;
  size_t len;
} CT_String;

extern CT_Typedef Type_String;
