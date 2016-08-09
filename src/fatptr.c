#include "fatptr.h"

CT_TypeRegistry __ctfatptr_types = {.types = {0}, .next_id = 1};

void ct_register_type(CT_Typedef *type) {
  type->id = __ctfatptr_types.next_id;
  CT_DEBUG("register type: %s (id: %u, size: %zu, align: %zu)", type->name,
           type->id, type->size, type->align & CT_OFFSET_MASK);
  __ctfatptr_types.types[__ctfatptr_types.next_id] = type;
  __ctfatptr_types.next_id++;
}

void ct_typedef_set_impls(CT_Typedef *type, CT_TypeImpl *impls, size_t num) {
  CT_DEBUG("set type impls: %s (%zu)", type->name, type->id);
  for (size_t n = 0; n < num; n++) {
    CT_TypeImpl *i = &impls[n];
    CT_DEBUG("\tadd impl: %s (%zu), src: %p, size: %zu", i->type->name,
             i->type->id, i->impls, i->size);
    uint8_t *ptr = calloc(1, i->size);
    memcpy(ptr, i->impls, i->size);
    type->impls[i->type->id] = ptr;
  }
}

void ct_header_trace(CT_Header *hd) {
  CT_INFO("hd: %p, alloc: %u, refs: %u, align: %u", hd, hd->alloc, hd->refs,
          ct_decode_align(hd->align));
}

CT_Var ct_alloc(const CT_Typedef *type) {
  size_t align  = ct_decode_align(type->align);
  uint8_t *head = calloc(1, align + type->size);
  return (uint8_t *)head + align;
}

void ct_free(CT_Var self) {
  CT_Header *hd  = ct_get_header(self);
  size_t size    = ct_decode_align(hd->align);
  uint8_t *block = (uint8_t *)self - size;
  CT_DEBUG("dealloc: %p", block);
  if (CT_ALLOC_HEAP == hd->alloc) {
    CT_DEBUG("free: %p", block);
    free(block);
  }
fail:
  return;
}

CT_Var ct_header_init(CT_Var head, const CT_Typedef *type, size_t alloc) {
  size_t offset;
  CT_DEBUG("hd_init: %p", head);
  switch (type->align) {
    case 2:
      offset = 3 - (((uintptr_t)head & 0xf) >> 2);
      break;
    case 1:
      offset = 1 - (((uintptr_t)head & 0x7) >> 2);
      break;
    default:
      offset = 0;
  }
  offset <<= 2;
  CT_Header *hd = head + offset;
  CT_DEBUG("init: %p -> %p (offset: %zu, align: %zu)", head, hd, offset,
           type->align);
  hd->type_id = type->id;
  hd->alloc   = alloc;
  hd->align   = type->align;
  hd->refs    = 0;
  return (uint8_t *)hd + sizeof(CT_Header);
}
