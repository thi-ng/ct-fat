#include "protos/cast.h"

CT_Typedef Type_Cast = {.name = "Cast", .abstract = 1};

static void init() __attribute__((constructor(10)));

static void init() {
  ct_register_type(&Type_Cast);
}
