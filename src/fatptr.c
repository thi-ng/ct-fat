#include <stdio.h>
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

#define method_lookup(T, proto, instance) \
  ((proto *)(ct_typeof(instance)->impls[(T).id]))

typedef void *CT_Var;

enum { CT_ALLOC_STACK = 0, CT_ALLOC_HEAP = 1, CT_ALLOC_RAW = 2 };
enum { CT_ALIGN4 = 0, CT_ALIGN8, CT_ALIGN16, CT_ALIGN32 };

typedef struct {
  void *impls[1 << CT_TYPE_BITS];
  size_t size;
  size_t id : CT_TYPE_BITS;
  size_t align : CT_OFFSET_BITS;
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

typedef struct {
  char *val;
  size_t len;
} CT_String;

typedef struct { void (*print)(const CT_Var); } CT_Print;
typedef struct { CT_Var (*add)(CT_Var, const CT_Var); } CT_Math;

typedef struct { int32_t val; } CT_I32;

typedef struct { uint32_t val; } CT_U32;

typedef struct { float val; } CT_F32;

typedef struct { double val; } CT_F64;

typedef struct { float x, y, z, w; } CT_Vec4;

static CT_TypeCache __types = {.types = {0}, .next_id = 0};

void ct_register_type(CT_Typedef *type) {
  type->id                       = __types.next_id;
  __types.types[__types.next_id] = type;
  __types.next_id++;
}

static void print_i32(const CT_Var x) {
  CT_INFO("i32: %d", ((CT_I32 *)x)->val);
}

static void print_u32(const CT_Var x) {
  CT_INFO("u32: 0x%x", ((CT_U32 *)x)->val);
}

static void print_f32(const CT_Var x) {
  CT_INFO("f32: %f", ((CT_F32 *)x)->val);
}

static void print_f64(const CT_Var x) {
  CT_INFO("f64: %f", ((CT_F64 *)x)->val);
}

static void print_str(const CT_Var x) {
  CT_INFO("str: %s", ((CT_String *)x)->val);
}

static void print_vec4(const CT_Var x) {
  const CT_Vec4 *v = x;
  CT_INFO("vec4: [%f, %f, %f, %f]", v->x, v->y, v->z, v->w);
}

CT_Typedef Type_Print = {.name = "Print"};

CT_Typedef Type_Math = {.name = "Math"};

CT_Typedef Type_I32 = {.size  = sizeof(CT_I32),
                       .align = CT_ALIGN4,
                       .name  = "I32"};

CT_Typedef Type_U32 = {.size  = sizeof(CT_U32),
                       .align = CT_ALIGN4,
                       .name  = "U32"};

CT_Typedef Type_F32 = {.size  = sizeof(CT_F32),
                       .align = CT_ALIGN4,
                       .name  = "F32"};

CT_Typedef Type_F64 = {.size  = sizeof(CT_F64),
                       .align = CT_ALIGN8,
                       .name  = "F64"};

CT_Typedef Type_String = {.size  = sizeof(CT_String),
                          .align = CT_ALIGN4,
                          .name  = "String"};

CT_Typedef Type_Vec4 = {.size  = sizeof(CT_Vec4),
                        .align = CT_ALIGN16,
                        .name  = "Vec4"};

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

ct_inline CT_Header *ct_get_header(CT_Var self) {
  return (CT_Header *)((uint8_t *)self - sizeof(CT_Header));
}

void ct_header_trace(CT_Header *hd) {
  CT_INFO("hd: %p, alloc: %u, refs: %u, align: %u", hd, hd->alloc, hd->refs,
          ct_decode_align(hd->align));
}

ct_inline CT_Typedef *ct_typeof(CT_Var self) {
  return __types.types[ct_get_header(self)->type_id];
}

CT_Var ct_alloc(CT_Typedef *type) {
  size_t align  = ct_decode_align(type->align);
  uint8_t *head = calloc(1, align + type->size);
  return (uint8_t *)head + align;
}

void ct_dealloc(CT_Var self) {
  CT_Header *hd  = ct_get_header(self);
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

CT_Var init_header(CT_Var block,
                   size_t type_id,
                   size_t alloc,
                   size_t align,
                   size_t offset) {
  CT_Header *hd = block + offset;
  CT_DEBUG("init: %p -> %p (offset: %zu, align: %zu)", block, hd, offset,
           align);
  hd->type_id = type_id;
  hd->alloc   = alloc;
  hd->align   = align;
  hd->refs    = 0;
  return (uint8_t *)hd + sizeof(CT_Header);
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
  return init_header(head, type->id, alloc, type->align, offset << 2);
}

#define ct_alloc_stack(type)                                             \
  ct_header_init(alloca(ct_decode_align(type.align) + type.size), &type, \
                 CT_ALLOC_STACK)

#define $(type, instance, ...)                                         \
  (instance *)memcpy(ct_alloc_stack(type), &((instance){__VA_ARGS__}), \
                     sizeof(instance))

#define $str(x) $(Type_String, CT_String, x)
#define $i32(x) $(Type_I32, CT_I32, x)
#define $u32(x) $(Type_U32, CT_U32, x)
#define $f32(x) $(Type_F32, CT_F32, x)
#define $f64(x) $(Type_F64, CT_F64, x)
#define $vec4(x, y, z, w) $(Type_Vec4, CT_Vec4, x, y, z, w)

#define ct_new(type, instance)                                         \
  ct_header_init(ct_alloc(&type) - ct_decode_align(type.align), &type, \
                 CT_ALLOC_HEAP)

#define new_i32() ct_new(Type_I32, CT_I32)
#define new_u32() ct_new(Type_U32, CT_U32)
#define new_f32() ct_new(Type_F32, CT_F32)
#define new_f64() ct_new(Type_F64, CT_F64)

void print(CT_Var x) {
  method_lookup(Type_Print, CT_Print, x)->print(x);
}

static CT_Var add_u32(CT_Var a, CT_Var b) {
  CT_U32 *c = new_u32();
  c->val    = ((CT_U32 *)a)->val + ((CT_U32 *)b)->val;
  return c;
}

CT_Var add(CT_Var a, CT_Var b) {
  return method_lookup(Type_Math, CT_Math, a)->add(a, b);
}

INITIALIZER(init_types) {
  CT_INFO("==== init ct-fatptr ====");
  CT_Typedef *builtins[] = {&Type_I32,  &Type_U32,    &Type_F32,   &Type_F64,
                            &Type_Vec4, &Type_String, &Type_Print, &Type_Math};
  for (size_t i = 0, num = sizeof(builtins) / sizeof(CT_Typedef *); i < num;
       i++) {
    ct_register_type(builtins[i]);
  }

  ct_extend_type(Type_I32, ct_type_impl(Type_Print, CT_Print, print_i32));

  ct_extend_type(Type_U32, ct_type_impl(Type_Print, CT_Print, print_u32),
                 ct_type_impl(Type_Math, CT_Math, add_u32));

  ct_extend_type(Type_F32, ct_type_impl(Type_Print, CT_Print, print_f32));

  ct_extend_type(Type_F64, ct_type_impl(Type_Print, CT_Print, print_f64));

  ct_extend_type(Type_String, ct_type_impl(Type_Print, CT_Print, print_str));

  ct_extend_type(Type_Vec4, ct_type_impl(Type_Print, CT_Print, print_vec4));
}

int main() {
  CT_INFO("hd size: %zu", sizeof(CT_Header));
  //CT_U32 *a = $u32(0xdeadbeef);
  CT_U32 *a = $u32(0x40302010);
  //CT_Var *b = $u32(0xcafebabe);
  CT_F64 *b    = new_f64();
  b->val       = 3.1415926;
  CT_Vec4 *c   = $vec4(1, 2, 3, 4);
  CT_String *d = $str("hello world!");
  //CT_INFO("a: %p, %08x", a, a->val);
  //CT_INFO("b: %p, %f", b, b->val);
  //CT_INFO("c: %p, [%f, %f, %f, %f]", c, c->x, c->y, c->z, c->w);
  //CT_INFO("d: %p, %s", d, d->val);
  CT_INFO("%08x", *((uint32_t *)a - 1));
  CT_INFO("%08x", *((uint32_t *)b - 1));
  CT_INFO("%08x", *((uint32_t *)c - 1));
  CT_INFO("%08x", *((uint32_t *)d - 1));
  print(a);
  print(b);
  print(c);
  print(d);
  print(add(a, a));
  ct_dealloc(a);
  ct_dealloc(b);
  ct_dealloc(c);
  ct_dealloc(d);
  return 0;
}
