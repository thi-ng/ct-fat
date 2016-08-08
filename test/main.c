#include "fatptr.h"

int main() {
  CT_INFO("hd size: %zu", sizeof(CT_Header));
  //CT_U32 *a = $u32(0xdeadbeef);
  CT_U32 *a = $u32(0x40302010);
  //CT_Var *b = $u32(0xcafebabe);
  CT_F32 *b1   = $f32(3.1415926);
  CT_F64 *b2   = new_f64();
  *b2          = *b1 * 2;
  CT_Vec4 *c   = $vec4(1, 2, 3, 4);
  CT_String *d = $str("hello world!");
  //CT_INFO("a: %p, %08x", a, a->val);
  //CT_INFO("b: %p, %f", b, b->val);
  //CT_INFO("c: %p, [%f, %f, %f, %f]", c, c->x, c->y, c->z, c->w);
  //CT_INFO("d: %p, %s", d, d->val);
  CT_INFO("%08x", *((uint32_t *)a - 1));
  CT_INFO("%08x", *((uint32_t *)b1 - 1));
  CT_INFO("%08x", *((uint32_t *)b2 - 1));
  CT_INFO("%08x", *((uint32_t *)c - 1));
  CT_INFO("%08x", *((uint32_t *)d - 1));
  print(a);
  print(b1);
  print(b2);
  print(c);
  print(d);
  print(add(a, a));
  CT_Var (*add)(CT_Var, CT_Var) = protocol_lookup(Type_Math, CT_Math, a)->add;
  CT_INFO("adder: %p", add);
  print(add(a, b1));
  CT_INFO("a u32?: %d", ct_is_instance_of(a, &Type_U32));
  CT_INFO("impls Print?: %d", ct_implements(b1, &Type_Print));
  print(cast($str("257"), &Type_F32));
  ct_free(a);
  ct_free(b1);
  ct_free(b2);
  ct_free(c);
  ct_free(d);
  return 0;
}
