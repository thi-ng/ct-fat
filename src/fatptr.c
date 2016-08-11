#include "fatptr.h"

CT_TypeRegistry __ctfat_registry = {.types = {0}, .next_id = 1};

void ct_register_type(CT_Typedef *type) {
  type->id = __ctfat_registry.next_id;
  CT_DEBUG("register type: %s (id: %u, parent: %u, size: %zu, align: %zu)",
           type->name, type->id, type->parent_id, type->size,
           type->align & CT_OFFSET_MASK);
  __ctfat_registry.types[__ctfat_registry.next_id] = type;
  __ctfat_registry.next_id++;
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

ct_inline CT_Var set_header(CT_Header *hd,
                            const CT_Typedef *type,
                            size_t alloc) {
  hd->type_id = type->id;
  hd->alloc   = alloc;
  hd->align   = type->align;
  hd->refs    = 0;
  return (uint8_t *)hd + sizeof(CT_Header);
}

CT_Var ct_header_init4(CT_Var head, const CT_Typedef *type, size_t alloc) {
  CT_DEBUG("init4: %p", head);
  return set_header(head, type, alloc);
}

CT_Var ct_header_init8(CT_Var head, const CT_Typedef *type, size_t alloc) {
  size_t offset = (1 - (((uintptr_t)head & 0x7) >> 2)) << 2;
  CT_Header *hd = (CT_Header *)((uint8_t *)head + offset);
  CT_DEBUG("init8: %p -> %p (offset: %zu, align: %zu)", head, hd, offset,
           type->align & CT_OFFSET_MASK);
  return set_header(hd, type, alloc);
}

CT_Var ct_header_init16(CT_Var head, const CT_Typedef *type, size_t alloc) {
  size_t offset = (3 - (((uintptr_t)head & 0xf) >> 2)) << 2;
  CT_Header *hd = (CT_Header *)((uint8_t *)head + offset);
  CT_DEBUG("init16: %p -> %p (offset: %zu, align: %zu)", head, hd, offset,
           type->align & CT_OFFSET_MASK);
  return set_header(hd, type, alloc);
}

CT_Var ct_header_init32(CT_Var head, const CT_Typedef *type, size_t alloc) {
  CT_DEBUG("hd_init32: %p", head);
  size_t offset = (7 - (((uintptr_t)head & 0x1f) >> 2)) << 2;
  CT_Header *hd = (CT_Header *)((uint8_t *)head + offset);
  CT_DEBUG("init32: %p -> %p (offset: %zu, align: %zu)", head, hd, offset,
           type->align & CT_OFFSET_MASK);
  return set_header(hd, type, alloc);
}
