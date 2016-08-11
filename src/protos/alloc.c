#include "protos/alloc.h"

ct_defproto(Type_Alloc, "Alloc");

ct_protocol_initializer(Type_Alloc, alloc);

typedef CT_Var (*hd_initializer)(CT_Var head,
                                 const CT_Typedef *type,
                                 size_t alloc);

static hd_initializer __hd_inits[] = {ct_header_init4, ct_header_init8,
                                      ct_header_init16, ct_header_init32};

CT_Var ct_alloc(const CT_Typedef *type) {
  size_t align  = ct_decode_align(type->align);
  uint8_t *head = calloc(1, align + type->size);
  return head + align;
}

CT_Var ct_new(const CT_Typedef *type) {
  CT_Alloc *impl = type->impls[Type_Alloc.id];
  if (impl && impl->new) {
    return impl->new ();
  }
  CT_Var instance = ct_alloc(type) - ct_decode_align(type->align);
  return __hd_inits[type->align](instance, type, CT_ALLOC_HEAP);
}

void ct_free(const CT_Var self) {
  CT_Header *hd = ct_get_header(self);
  if (CT_ALLOC_HEAP == hd->alloc) {
    uint8_t *block = (uint8_t *)self - ct_decode_align(hd->align);
    ct_protocol_call(Type_Alloc, CT_Alloc, free, self, self);
    CT_DEBUG("free: %p", block);
    free(block);
  }
}
