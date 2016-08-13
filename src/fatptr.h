#pragma once

#include <alloca.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "ct-head/features.h"
#include "ct-head/log.h"
#include "ct-head/vargs.h"

#define CT_TYPE_BITS 8
#define CT_ALLOC_BITS 2
#define CT_MARK_BITS 1
#define CT_REF_BITS 18
#define CT_OFFSET_BITS 3
#define CT_OFFSET_MASK (size_t)((1 << CT_OFFSET_BITS) - 1)

#define ct_decode_align(a) (2 << ((a) + 1))

// clang-format off

#define ct_declare_type(T, suffix)                                   \
  extern CT_Typedef T;                                               \
  void ct_init_type_##suffix() __attribute__((constructor))

#define ct_deftype(T, instance, name_, align_, ...) \
  CT_Typedef T = {                                  \
      .size = sizeof(instance),                     \
      .align = align_,                              \
      .name = name_,                                \
      __VA_ARGS__}

#define ct_defproto(T, name_, ...) \
  CT_Typedef T = {                 \
    .abstract = 1,                 \
    .name = name_,                 \
    __VA_ARGS__}

// clang-format on

#define ct_extend_type(T, ...)                                            \
  do {                                                                    \
    CT_TypeImpl impls[] = {__VA_ARGS__};                                  \
    ct_typedef_set_impls(&T, impls, sizeof(impls) / sizeof(CT_TypeImpl)); \
  } while (0);

#define ct_type_impl(T, proto, ...) \
  { .type = &T, .size = sizeof(proto), .impls = &((proto){__VA_ARGS__}) }

#define ct_protocol_initializer(T, suffix) \
  void ct_init_type_##suffix() {           \
    if (!T.id) {                           \
      ct_register_type(&T);                \
    }                                      \
  }

#define ct_protocol_lookup(T, proto, instance) \
  ((proto *)(ct_typeof(instance)->impls[(T).id]))

#define ct_protocol_call(T, P, method, inst, ...)  \
  const P *proto = ct_protocol_lookup(T, P, inst); \
  if (proto && proto->method) {                    \
    return proto->method(__VA_ARGS__);             \
  }

#define ct_protocol_call_fast(T, P, method, inst, ...) \
  return ct_protocol_lookup(T, P, inst)->method(__VA_ARGS__);

#define ct_alloc_stack(type, align_)                                      \
  ct_header_init##align_(alloca(ct_decode_align(type.align) + type.size), \
                         &type, CT_ALLOC_STACK)

#define $(type, instance, align, ...)                                         \
  (instance *)memcpy(ct_alloc_stack(type, align), &((instance){__VA_ARGS__}), \
                     sizeof(instance))

#define _ct_require1(a) ct_init_type_##a()
#define _ct_require2(a, b) \
  _ct_require1(a);         \
  _ct_require1(b)
#define _ct_require3(a, b, c) \
  _ct_require2(a, b);         \
  _ct_require1(c)
#define _ct_require4(a, b, c, d) \
  _ct_require2(a, b);            \
  _ct_require2(c, d)
#define _ct_require5(a, b, c, d, e) \
  _ct_require3(a, b, c);            \
  _ct_require2(d, e)
#define _ct_require6(a, b, c, d, e, f) \
  _ct_require3(a, b, c);               \
  _ct_require3(d, e, f)
#define _ct_require7(a, b, c, d, e, f, g) \
  _ct_require4(a, b, c, d);               \
  _ct_require3(e, f, g)
#define _ct_require8(a, b, c, d, e, f, g, h) \
  _ct_require4(a, b, c, d);                  \
  _ct_require4(e, f, g, h)
#define _ct_require9(a, b, c, d, e, f, g, h, i) \
  _ct_require5(a, b, c, d, e);                  \
  _ct_require4(f, g, h, i)
#define _ct_require10(a, b, c, d, e, f, g, h, i, j) \
  _ct_require5(a, b, c, d, e);                      \
  _ct_require5(f, g, h, i, j)

#define ct_require(...) CT_VA_DISPATCH(_ct_require, __VA_ARGS__)(__VA_ARGS__)

// -------------------- internal type definitions

enum { CT_ALLOC_STACK = 0, CT_ALLOC_HEAP, CT_ALLOC_RAW };
enum { CT_ALIGN4 = 0, CT_ALIGN8, CT_ALIGN16, CT_ALIGN32 };

typedef void *CT_Var;

typedef struct {
  void *impls[1 << CT_TYPE_BITS];
  size_t size;
  size_t id : CT_TYPE_BITS;
  size_t parent_id : CT_TYPE_BITS;
  size_t align : CT_OFFSET_BITS;
  size_t abstract : 1;
  char *name;
} CT_Typedef;

typedef struct {
  CT_Typedef *type;
  size_t size;
  void *impls;
} CT_TypeImpl;

typedef struct {
  CT_Typedef *types[1 << CT_TYPE_BITS];
  size_t next_id;
} CT_TypeRegistry;

typedef struct {
  uint32_t type_id : CT_TYPE_BITS;
  uint32_t alloc : CT_ALLOC_BITS;
  uint32_t mark : CT_MARK_BITS;
  uint32_t refs : CT_REF_BITS;
  uint32_t align : CT_OFFSET_BITS;
} CT_Header;

extern CT_TypeRegistry __ctfat_registry;

void ct_register_type(CT_Typedef *type);
void ct_typedef_set_impls(CT_Typedef *type, CT_TypeImpl *impls, size_t num);

CT_Var ct_header_init4(CT_Var head, const CT_Typedef *type, size_t alloc);
CT_Var ct_header_init8(CT_Var head, const CT_Typedef *type, size_t alloc);
CT_Var ct_header_init16(CT_Var head, const CT_Typedef *type, size_t alloc);
CT_Var ct_header_init32(CT_Var head, const CT_Typedef *type, size_t alloc);
void ct_header_trace(CT_Header *hd);

ct_inline CT_Header *ct_get_header(const CT_Var self) {
  return (CT_Header *)((uint8_t *)self - sizeof(CT_Header));
}

ct_inline CT_Typedef *ct_typeof(const CT_Var self) {
  return __ctfat_registry.types[ct_get_header(self)->type_id];
}

ct_inline bool ct_instance_of(const CT_Var self, const CT_Typedef *type) {
  return ct_get_header(self)->type_id == type->id;
}

ct_inline bool ct_implements(const CT_Var self, const CT_Typedef *proto) {
  return ct_typeof(self)->impls[proto->id] != NULL;
}

// -------------------- builtin type definitions

#include "types/num.h"
#include "types/string.h"
#include "types/vec.h"

// -------------------- builtin type definitions

#include "protos/alloc.h"
#include "protos/cast.h"
#include "protos/compare.h"
#include "protos/hash.h"
#include "protos/len.h"
#include "protos/math.h"
#include "protos/print.h"
