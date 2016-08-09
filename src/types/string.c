#include "types/string.h"
#include "protos/cast.h"
#include "protos/hash.h"
#include "protos/print.h"
#include "types/num.h"

CT_Typedef Type_String = {.size  = sizeof(CT_String),
                          .align = CT_ALIGN4,
                          .name  = "String"};

static void print_str(FILE *out, const CT_Var x) {
  fprintf(out, "str: %s\n", ((CT_String *)x)->val);
}

static CT_Var cast_str(const CT_Var x, const CT_Typedef *type) {
  CT_INFO("cast target type: %zu", type->id);
  if (type->id == Type_I32.id) {
    CT_I32 *res = ct_new_i32();
    *res        = (CT_I32)atol(((CT_String *)x)->val);
    return res;
  }
  if (type->id == Type_U32.id) {
    CT_U32 *res = ct_new_u32();
    *res        = (CT_U32)atol(((CT_String *)x)->val);
    return res;
  }
  if (type->id == Type_F32.id) {
    CT_F32 *res = ct_new_f32();
    *res        = (CT_F32)atof(((CT_String *)x)->val);
    return res;
  }
  if (type->id == Type_F64.id) {
    CT_F64 *res = ct_new_f64();
    *res        = (CT_F64)atof(((CT_String *)x)->val);
    return res;
  }
  return NULL;
}

static CT_U32 hash_str(const CT_Var x) {
  char *ptr = ((CT_String *)x)->val;
  return ct_murmur3_32(ptr, strlen(ptr));
}

void init_type_string() {
  if (!Type_String.id) {
    init_type_cast();
    init_type_print();
    ct_register_type(&Type_String);
    ct_extend_type(Type_String, ct_type_impl(Type_Print, CT_Print, print_str),
                   ct_type_impl(Type_Cast, CT_Cast, cast_str),
                   ct_type_impl(Type_Hash, CT_Hash, hash_str));
  }
}
