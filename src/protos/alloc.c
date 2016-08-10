#include "protos/alloc.h"

ct_defproto(Type_Alloc, "Alloc");

ct_protocol_initializer(Type_Alloc, alloc);

typedef CT_Var (*hd_initializer)(CT_Var head,
                                 const CT_Typedef *type,
                                 size_t alloc);

static hd_initializer __hd_inits[] = {ct_header_init4, ct_header_init8,
                                      ct_header_init16, ct_header_init32};

CT_Var ct_new(const CT_Typedef *type) {
  CT_Alloc *impl = type->impls[Type_Alloc.id];
  if (impl && impl->alloc) {
    return impl->alloc();
  }
  CT_Var instance = ct_alloc(type) - ct_decode_align(type->align);
  return __hd_inits[type->align](instance, type, CT_ALLOC_HEAP);
}
