#include "protos/math.h"

CT_Typedef Type_Math = {.name = "Math"};

static void init() __attribute__((constructor(10)));

static void init() {
  ct_register_type(&Type_Math);
}
