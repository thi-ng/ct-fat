#include <stdio.h>
#include <string.h>

#include "cthing.h"

#define CT_TYPE_BITS 13
#define CT_ALLOC_BITS 2
#define CT_MARK_BITS 1
#define CT_REF_BITS 13
#define CT_OFFSET_BITS 3

#define ct_decode_align(a) (2 << (a + 1))

#define ct_alloc_stack(type, id, align) \
  ct_header_init((uint8_t[ct_decode_align(align) + sizeof(type)]){0}, id, CT_ALLOC_STACK, align)

#define $(type, id, align, ...) \
  memcpy(ct_alloc_stack(type, id, align), &((type){__VA_ARGS__}), sizeof(type))

#define $str(x) $(CT_String, CT_TYPE_STR, CT_ALIGN4, x)
#define $i32(x) $(CT_I32, CT_TYPE_I32, CT_ALIGN4, x)
#define $u32(x) $(CT_U32, CT_TYPE_U32, CT_ALIGN4, x)
#define $f32(x) $(CT_F32, CT_TYPE_F32, CT_ALIGN4, x)
#define $f64(x) $(CT_F64, CT_TYPE_F64, CT_ALIGN8, x)
#define $vec4(x, y, z, w) $(CT_Vec4, CT_TYPE_VEC4, CT_ALIGN16, x, y, z, w)

#define ct_new(type, id, align)                                                  \
  ct_header_init(ct_alloc(sizeof(type), align) - ct_decode_align(align), id, CT_ALLOC_HEAP, align)

#define new_i32() ct_new(CT_I32, CT_TYPE_I32, CT_ALIGN4)
#define new_u32() ct_new(CT_U32, CT_TYPE_U32, CT_ALIGN4)
#define new_f32() ct_new(CT_F32, CT_TYPE_F32, CT_ALIGN4)
#define new_f64() ct_new(CT_F64, CT_TYPE_F64, CT_ALIGN8)

typedef void *CT_Var;

enum {
  CT_TYPE_I32 = 1,
  CT_TYPE_U32,
  CT_TYPE_F32,
  CT_TYPE_F64,
  CT_TYPE_VEC4,
  CT_TYPE_PTR,
  CT_TYPE_STR,
  CT_TYPE_CONS
};

enum { CT_ALLOC_STACK = 0, CT_ALLOC_HEAP = 1, CT_ALLOC_GC = 2 };
enum { CT_ALIGN4 = 0, CT_ALIGN8, CT_ALIGN16, CT_ALIGN32 };

typedef struct {
  uint32_t type_id : CT_TYPE_BITS;
  uint32_t alloc : CT_ALLOC_BITS;
  uint32_t mark : CT_MARK_BITS;
  uint32_t refs : CT_REF_BITS;
  uint32_t align : CT_OFFSET_BITS;
} CT_Header;

typedef struct {
  char *val;
  size_t len;
} CT_String;

typedef struct { int32_t val; } CT_I32;

typedef struct { uint32_t val; } CT_U32;

typedef struct { float val; } CT_F32;

typedef struct { double val; } CT_F64;

typedef struct { float x, y, z, w; } CT_Vec4;

ct_inline CT_Header *ct_get_header(CT_Var self) {
  return (CT_Header*)((uint8_t*)self - sizeof(CT_Header));
}

void ct_header_trace(CT_Header *hd) {
  CT_INFO("hd: %p, alloc: %u, refs: %u, align: %u", hd, hd->alloc, hd->refs, ct_decode_align(hd->align));
}

CT_Var ct_alloc(size_t size, size_t align) {
  align         = ct_decode_align(align);
  uint8_t *head = calloc(1, align + size);
  return (uint8_t *)head + align;
}

void ct_dealloc(CT_Var self) {
  CT_Header *hd = ct_get_header(self);
  size_t size    = ct_decode_align(hd->align);
  uint8_t *block = (uint8_t *)self - size;
  CT_INFO("block: %p", block);
  ct_header_trace(hd);
  if (CT_ALLOC_HEAP == hd->alloc) {
    CT_DEBUG("free: %p", block);
    free(block);
  }
fail:
  return;
}

CT_Var init_header(CT_Var block, size_t type_id, size_t alloc, size_t align, size_t offset) {
  CT_Header *hd = block + offset;
  CT_DEBUG("init: %p -> %p (offset: %zu, align: %zu)", block, hd, offset, align);
  hd->type_id = type_id;
  hd->alloc   = alloc;
  hd->align  = align;
  hd->refs = 0;
  return (uint8_t *)hd + sizeof(CT_Header);
}

CT_Var ct_header_init(CT_Var head, size_t type_id, size_t alloc, size_t align) {
  size_t offset;
  CT_DEBUG("hd_init: %p", head);
  switch (align) {
    case 2:
      offset = 3 - (((uintptr_t)head & 0xf) >> 2);
      break;
    case 1:
      offset = 1 - (((uintptr_t)head & 0x7) >> 2);
      break;
    default:
      offset = 0;
  }
  return init_header(head, type_id, alloc, align, offset << 2);
}

int main() {
  CT_INFO("hd size: %zu", sizeof(CT_Header));
  CT_Var *a = $i32(0xdeadbeef);
  //CT_Var *b = $u32(0xcafebabe);
  CT_F64 *b = new_f64();
  b->val = 3.1415926;
  CT_Vec4 *c = $vec4(1,2,3,4);
  CT_String *d = $str("hello world!");
  CT_INFO("a: %p, %08x", a, *((uint32_t *)a));
  CT_INFO("b: %p, %f", b, b->val);
  CT_INFO("c: %p, [%f, %f, %f, %f]", c, c->x, c->y, c->z, c->w);  
  CT_INFO("d: %p, %s", d, d->val);  
  CT_INFO("%08x", *((uint32_t *)a - 1));
  CT_INFO("%08x", *((uint32_t *)b - 1));
  CT_INFO("%08x", *((uint32_t *)c - 1));
  CT_INFO("%08x", *((uint32_t *)d - 1));
  ct_dealloc(a);
  ct_dealloc(b);
  ct_dealloc(c);
  ct_dealloc(d);
  return 0;
}
