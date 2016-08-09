#include "protos/print.h"

ct_defproto(Type_Print, "Print");

void init_type_print() {
  if (!Type_Print.id) {
    ct_register_type(&Type_Print);
  }
}
