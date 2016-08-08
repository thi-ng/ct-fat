#include "types/num.h"
#include "protos/math.h"
#include "protos/print.h"

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

static void print_i32(const CT_Var x) {
  CT_INFO("i32: %d", *((CT_I32 *)x));
}

static void print_u32(const CT_Var x) {
  CT_INFO("u32: 0x%x", *((CT_U32 *)x));
}

static void print_f32(const CT_Var x) {
  CT_INFO("f32: %f", *((CT_F32 *)x));
}

static void print_f64(const CT_Var x) {
  CT_INFO("f64: %f", *((CT_F64 *)x));
}

static CT_Var add_u32(CT_Var a, CT_Var b) {
  const CT_Typedef *t = ct_typeof(b);
  if (t == &Type_U32) {
    CT_U32 *c = new_u32();
    *c        = *((CT_U32 *)a) + *((CT_U32 *)b);
    return c;
  }
  if (t == &Type_I32) {
    CT_U32 *c = new_u32();
    *c        = *((CT_U32 *)a) + *((CT_I32 *)b);
    return c;
  }
  if (t == &Type_F32) {
    CT_F32 *c = new_f32();
    *c        = *((CT_U32 *)a) + *((CT_F32 *)b);
    return c;
  }
  return NULL;
}

void init_type_num() {
  if (!Type_I32.id) {
    init_type_cast();
    init_type_print();
    init_type_math();
    ct_register_type(&Type_I32);
    ct_register_type(&Type_U32);
    ct_register_type(&Type_F32);
    ct_register_type(&Type_F64);

    ct_extend_type(Type_I32, ct_type_impl(Type_Print, CT_Print, print_i32));

    ct_extend_type(Type_U32, ct_type_impl(Type_Print, CT_Print, print_u32),
                   ct_type_impl(Type_Math, CT_Math, add_u32));

    ct_extend_type(Type_F32, ct_type_impl(Type_Print, CT_Print, print_f32));

    ct_extend_type(Type_F64, ct_type_impl(Type_Print, CT_Print, print_f64));
  }
}
