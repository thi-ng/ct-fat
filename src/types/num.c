#include <inttypes.h>

#include "protos/math.h"
#include "protos/print.h"
#include "types/num.h"

ct_deftype(Type_I32, CT_I32, "I32", CT_ALIGN4);
ct_deftype(Type_I64, CT_I64, "I64", CT_ALIGN8);
ct_deftype(Type_U32, CT_U32, "U32", CT_ALIGN4);
ct_deftype(Type_U64, CT_U64, "U64", CT_ALIGN8);
ct_deftype(Type_F32, CT_F32, "F32", CT_ALIGN4);
ct_deftype(Type_F64, CT_F64, "F64", CT_ALIGN8);

static void print_i32(FILE *out, const CT_Var x) {
  fprintf(out, "i32: %" PRId32 "\n", *((CT_I32 *)x));
}

static void print_i64(FILE *out, const CT_Var x) {
  fprintf(out, "i64: %" PRId64 "\n", *((CT_I64 *)x));
}

static void print_u32(FILE *out, const CT_Var x) {
  fprintf(out, "u32: 0x%" PRIx32 "\n", *((CT_U32 *)x));
}

static void print_u64(FILE *out, const CT_Var x) {
  fprintf(out, "u64: 0x%" PRIx64 "\n", *((CT_U64 *)x));
}

static void print_f32(FILE *out, const CT_Var x) {
  fprintf(out, "f32: %f\n", *((CT_F32 *)x));
}

static void print_f64(FILE *out, const CT_Var x) {
  fprintf(out, "f64: %f\n", *((CT_F64 *)x));
}

static CT_Var add_u32(CT_Var a, CT_Var b) {
  const CT_Typedef *t = ct_typeof(b);
  if (t == &Type_U32) {
    CT_U32 *c = ct_new_u32();
    *c        = *((CT_U32 *)a) + *((CT_U32 *)b);
    return c;
  }
  if (t == &Type_I32) {
    CT_U32 *c = ct_new_u32();
    *c        = *((CT_U32 *)a) + *((CT_I32 *)b);
    return c;
  }
  if (t == &Type_F32) {
    CT_F32 *c = ct_new_f32();
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
    ct_register_type(&Type_I64);
    ct_register_type(&Type_U32);
    ct_register_type(&Type_U64);
    ct_register_type(&Type_F32);
    ct_register_type(&Type_F64);

    ct_extend_type(Type_I32, ct_type_impl(Type_Print, CT_Print, print_i32));

    ct_extend_type(Type_I64, ct_type_impl(Type_Print, CT_Print, print_i64));

    ct_extend_type(Type_U32, ct_type_impl(Type_Print, CT_Print, print_u32),
                   ct_type_impl(Type_Math, CT_Math, add_u32));

    ct_extend_type(Type_U64, ct_type_impl(Type_Print, CT_Print, print_u64));

    ct_extend_type(Type_F32, ct_type_impl(Type_Print, CT_Print, print_f32));

    ct_extend_type(Type_F64, ct_type_impl(Type_Print, CT_Print, print_f64));
  }
}
