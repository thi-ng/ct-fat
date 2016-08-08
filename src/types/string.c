#include "types/string.h"
#include "protos/cast.h"
#include "protos/print.h"
#include "types/num.h"

CT_Typedef Type_String = {.size  = sizeof(CT_String),
                          .align = CT_ALIGN4,
                          .name  = "String"};

static void print_str(const CT_Var x) {
  CT_INFO("str: %s", ((CT_String *)x)->val);
}

static CT_Var cast_str(const CT_Var x, const CT_Typedef *type) {
  CT_INFO("cast target type: %zu", type->id);
  if (type->id == Type_U32.id) {
    CT_U32 *res = new_u32();
    *res        = atol(((CT_String *)x)->val);
    return res;
  }
  return NULL;
}

static void init() __attribute__((constructor(102)));

static void init() {
  ct_register_type(&Type_String);
  ct_extend_type(Type_String, ct_type_impl(Type_Print, CT_Print, print_str),
                 ct_type_impl(Type_Cast, CT_Cast, cast_str));
}
