#include "protos/compare.h"

ct_defproto(Type_Compare, "Compare");

void init_type_compare() {
  if (!Type_Compare.id) {
    ct_register_type(&Type_Compare);
  }
}
