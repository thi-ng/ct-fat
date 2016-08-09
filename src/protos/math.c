#include "protos/math.h"

ct_defproto(Type_Math, "Math");

void init_type_math() {
  if (!Type_Math.id) {
    ct_register_type(&Type_Math);
  }
}
