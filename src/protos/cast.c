#include "protos/cast.h"

CT_Typedef Type_Cast = {.abstract = 1, .name = "Cast"};

void init_type_cast() {
  if (!Type_Cast.id) {
    ct_register_type(&Type_Cast);
  }
}
