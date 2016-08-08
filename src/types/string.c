#include "types/string.h"
#include "protos/print.h"

CT_Typedef Type_String = {.size  = sizeof(CT_String),
                          .align = CT_ALIGN4,
                          .name  = "String"};

static void print_str(const CT_Var x) {
  CT_INFO("str: %s", ((CT_String *)x)->val);
}

static void init() __attribute__((constructor(102)));

static void init() {
  ct_register_type(&Type_String);
  ct_extend_type(Type_String, ct_type_impl(Type_Print, CT_Print, print_str));
}
