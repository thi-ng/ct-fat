#pragma once

#include "fatptr.h"

typedef struct {
  uint32_t (*hash32)(const CT_Var);
  uint64_t (*hash64)(const CT_Var);
} CT_Hash;

extern CT_Typedef Type_Hash;

void init_type_hash() __attribute__((constructor));

uint32_t ct_murmur3_32(const CT_Var data, size_t len);
uint64_t ct_murmur3_64(const CT_Var data, size_t len);

uint32_t ct_hash32(CT_Var x);
uint64_t ct_hash64(CT_Var x);
