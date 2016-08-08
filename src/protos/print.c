#include "protos/print.h"

CT_Typedef Type_Print = {.name = "Print"};

static void init() __attribute__((constructor(10)));

static void init() {
  ct_register_type(&Type_Print);
}
