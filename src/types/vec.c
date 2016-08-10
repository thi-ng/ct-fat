#include "types/vec.h"

ct_deftype(Type_Vec4, CT_Vec4, "Vec4", CT_ALIGN16);

static void print_vec4(FILE *out, const CT_Var x) {
  const CT_Vec4 *v = x;
  fprintf(out, "vec4: [%f, %f, %f, %f]\n", v->x, v->y, v->z, v->w);
}

static int compare_vec4(CT_Var a, CT_Var b) {
  const CT_Typedef *tb = ct_typeof(b);
  if (tb == &Type_Vec4) {
    CT_Vec4 *va = (CT_Vec4 *)a;
    CT_Vec4 *vb = (CT_Vec4 *)b;
    if (va->x < vb->x)
      return -1;
    if (va->x > vb->x)
      return 1;
    if (va->y < vb->y)
      return -1;
    if (va->y > vb->y)
      return 1;
    if (va->z < vb->z)
      return -1;
    if (va->z > vb->z)
      return 1;
    if (va->w < vb->w)
      return -1;
    if (va->w > vb->w)
      return 1;
    return 0;
  }
  return -100;  // FIXME
}

void ct_init_type_vec() {
  if (!Type_Vec4.id) {
    ct_register_type(&Type_Vec4);
    ct_extend_type(Type_Vec4, ct_type_impl(Type_Print, CT_Print, print_vec4),
                   ct_type_impl(Type_Compare, CT_Compare, compare_vec4));
  }
}
