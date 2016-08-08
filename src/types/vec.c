#include "types/vec.h"

CT_Typedef Type_Vec4 = {.size  = sizeof(CT_Vec4),
                        .align = CT_ALIGN16,
                        .name  = "Vec4"};

static void print_vec4(const CT_Var x) {
  const CT_Vec4 *v = x;
  CT_INFO("vec4: [%f, %f, %f, %f]", v->x, v->y, v->z, v->w);
}

static void init() __attribute__((constructor(101)));

static void init() {
  ct_register_type(&Type_Vec4);
  ct_extend_type(Type_Vec4, ct_type_impl(Type_Print, CT_Print, print_vec4));
}
