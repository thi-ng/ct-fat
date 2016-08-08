#pragma once

#include <stdbool.h>
#include <string.h>

#include "cthing.h"
#include "init.h"

#define CT_TYPE_BITS 8
#define CT_ALLOC_BITS 2
#define CT_MARK_BITS 1
#define CT_REF_BITS 18
#define CT_OFFSET_BITS 3

#define ct_decode_align(a) (2 << (a + 1))

#define ct_extend_type(T, ...)                                            \
  do {                                                                    \
    CT_TypeImpl impls[] = {__VA_ARGS__};                                  \
    ct_typedef_set_impls(&T, impls, sizeof(impls) / sizeof(CT_TypeImpl)); \
  } while (0);

#define ct_type_impl(T, proto, ...) \
  { .type = &T, .size = sizeof(proto), .impls = &((proto){__VA_ARGS__}) }

#define protocol_lookup(T, proto, instance) \
  ((proto *)(ct_typeof(instance)->impls[(T).id]))

#define ct_alloc_stack(type)                                             \
  ct_header_init(alloca(ct_decode_align(type.align) + type.size), &type, \
                 CT_ALLOC_STACK)

#define $(type, instance, ...)                                         \
  (instance *)memcpy(ct_alloc_stack(type), &((instance){__VA_ARGS__}), \
                     sizeof(instance))

#define ct_new(type, instance)                                              \
  (instance *)ct_header_init(ct_alloc(&type) - ct_decode_align(type.align), \
                             &type, CT_ALLOC_HEAP)

// -------------------- internal type definitions

enum { CT_ALLOC_STACK = 0, CT_ALLOC_HEAP = 1, CT_ALLOC_RAW = 2 };
enum { CT_ALIGN4 = 0, CT_ALIGN8, CT_ALIGN16, CT_ALIGN32 };

typedef void *CT_Var;

typedef struct {
  void *impls[1 << CT_TYPE_BITS];
  size_t size;
  size_t id : CT_TYPE_BITS;
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
} CT_TypeCache;

typedef struct {
  uint32_t type_id : CT_TYPE_BITS;
  uint32_t alloc : CT_ALLOC_BITS;
  uint32_t mark : CT_MARK_BITS;
  uint32_t refs : CT_REF_BITS;
  uint32_t align : CT_OFFSET_BITS;
} CT_Header;

extern CT_TypeCache __ctfatptr_types;

void ct_register_type(CT_Typedef *type);
void ct_typedef_set_impls(CT_Typedef *type, CT_TypeImpl *impls, size_t num);

CT_Var ct_alloc(const CT_Typedef *type);
void ct_free(CT_Var self);

CT_Var ct_header_init(CT_Var head, const CT_Typedef *type, size_t alloc);
void ct_header_trace(CT_Header *hd);

ct_inline CT_Header *ct_get_header(const CT_Var self) {
  return (CT_Header *)((uint8_t *)self - sizeof(CT_Header));
}

ct_inline CT_Typedef *ct_typeof(const CT_Var self) {
  return __ctfatptr_types.types[ct_get_header(self)->type_id];
}

ct_inline bool ct_is_instance_of(const CT_Var self, const CT_Typedef *type) {
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

#include "protos/cast.h"
#include "protos/math.h"
#include "protos/print.h"
