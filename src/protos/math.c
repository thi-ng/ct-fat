#include "protos/math.h"

CT_Typedef Type_Math = {.abstract = 1, .name = "Math"};

void init_type_math() {
  if (!Type_Math.id) {
    ct_register_type(&Type_Math);
  }
}
