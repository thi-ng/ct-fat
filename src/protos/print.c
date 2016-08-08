#include "protos/print.h"

CT_Typedef Type_Print = {.abstract = 1, .name = "Print"};

void init_type_print() {
  if (!Type_Print.id) {
    ct_register_type(&Type_Print);
  }
}
