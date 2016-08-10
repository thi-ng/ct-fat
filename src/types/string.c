#include "types/string.h"

ct_deftype(Type_String, CT_String, "String", CT_ALIGN4);

static void print_str(FILE *out, const CT_Var x) {
  fprintf(out, "str: %s\n", ((CT_String *)x)->val);
}

static CT_Var cast_str(const CT_Var x, const CT_Typedef *type) {
  CT_INFO("cast target type: %zu", type->id);
  if (type->id == Type_String.id) {
    return x;
  }
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

static uint32_t hash_str(const CT_Var x) {
  char *ptr = ((CT_String *)x)->val;
  return ct_murmur3_32(ptr, strlen(ptr));
}

static int compare_str(const CT_Var a, const CT_Var b) {
  const CT_Typedef *tb = ct_typeof(b);
  if (tb == &Type_String) {
    return strcmp(((CT_String *)a)->val, ((CT_String *)b)->val);
  }
  return -100;  // FIXME throw
}

static CT_Var assign_str(CT_Var dest, CT_Var src) {
  if (ct_typeof(src) != &Type_String) {
    CT_String *cast = ct_cast(src, &Type_String);
  }
  return NULL;
}

void init_type_string() {
  if (!Type_String.id) {
    init_type_cast();
    init_type_print();
    ct_register_type(&Type_String);
    ct_extend_type(Type_String, ct_type_impl(Type_Print, CT_Print, print_str),
                   ct_type_impl(Type_Cast, CT_Cast, cast_str),
                   ct_type_impl(Type_Compare, CT_Compare, compare_str),
                   ct_type_impl(Type_Hash, CT_Hash, hash_str));
  }
}
