#include "protos/len.h"

ct_defproto(Type_Len, "Len");

ct_protocol_initializer(Type_Len, len);

size_t ct_length(const CT_Var self) {
  ct_protocol_call(Type_Len, CT_Len, length, self, self);
  return 0;  // FIXME throw
}
