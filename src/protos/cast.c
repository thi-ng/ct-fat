#include "protos/cast.h"

ct_defproto(Type_Cast, "Cast");

void init_type_cast() {
  if (!Type_Cast.id) {
    ct_register_type(&Type_Cast);
  }
}
