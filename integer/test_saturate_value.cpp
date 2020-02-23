/**
 * test_saturate_value.cpp
 * Performs basic unit tests on all versions of
 *   void saturate_value_X(type *value, const type lower_bound, const type upper_bound);
 * and
 *   void saturate_value<typename type>(type &value, const type lower_bound, const type upper_bound);
 * where X is a type abbreviation.
 * 
 * lower_bound is set to upper_bound - 2.
 *
 * Input values less than lower_bound, equal to lower_bound, between
 * lower_bound and uppper_bound, equal to upper_bound, and greater than
 * upper_bound are tested for all functions.
 * 
 * Written in 2018 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */
#include <cstdio>
#include "saturate_value.hpp"
#ifdef __cplusplus
  extern "C"
  {
#endif
    #include "saturate_value.h"
#ifdef __cplusplus
  }
#endif

int main () {
  int8_t res_i8;
  std::printf("Testing saturate_value_i8\n");
  res_i8 = -2;
  saturate_value_i8(&res_i8, -1, 1);
  if (res_i8 != -1) std::printf("\nERROR: res_i8 = -2; saturate_value_i8(&res_i8, -1, 1); res_i8: %i, expected -1.\n\n", res_i8);
  res_i8 = -1;
  saturate_value_i8(&res_i8, -1, 1);
  if (res_i8 != -1) std::printf("\nERROR: res_i8 = -1; saturate_value_i8(&res_i8, -1, 1); res_i8: %i, expected -1.\n\n", res_i8);
  res_i8 = 0;
  saturate_value_i8(&res_i8, -1, 1);
  if (res_i8 != 0) std::printf("\nERROR: res_i8 = 0; saturate_value_i8(&res_i8, -1, 1); res_i8: %i, expected 0.\n\n", res_i8);
  res_i8 = 1;
  saturate_value_i8(&res_i8, -1, 1);
  if (res_i8 != 1) std::printf("\nERROR: res_i8 = 1; saturate_value_i8(&res_i8, -1, 1); res_i8: %i, expected 1.\n\n", res_i8);
  res_i8 = 2;
  saturate_value_i8(&res_i8, -1, 1);
  if (res_i8 != 1) std::printf("\nERROR: res_i8 = 2; saturate_value_i8(&res_i8, -1, 1); res_i8: %i, expected 1.\n\n", res_i8);

  std::printf("Testing saturate_value<int8_t>\n");
  res_i8 = -2;
  saturate_value<int8_t>(res_i8, -1, 1);
  if (res_i8 != -1) std::printf("\nERROR: res_i8 = -2; saturate_value<int8_t>(res_i8, -1, 1); res_i8: %i, expected -1.\n\n", res_i8);
  res_i8 = -1;
  saturate_value<int8_t>(res_i8, -1, 1);
  if (res_i8 != -1) std::printf("\nERROR: res_i8 = -1; saturate_value<int8_t>(res_i8, -1, 1); res_i8: %i, expected -1.\n\n", res_i8);
  res_i8 = 0;
  saturate_value<int8_t>(res_i8, -1, 1);
  if (res_i8 != 0) std::printf("\nERROR: res_i8 = 0; saturate_value<int8_t>(res_i8, -1, 1); res_i8: %i, expected 0.\n\n", res_i8);
  res_i8 = 1;
  saturate_value<int8_t>(res_i8, -1, 1);
  if (res_i8 != 1) std::printf("\nERROR: res_i8 = 1; saturate_value<int8_t>(res_i8, -1, 1); res_i8: %i, expected 1.\n\n", res_i8);
  res_i8 = 2;
  saturate_value<int8_t>(res_i8, -1, 1);
  if (res_i8 != 1) std::printf("\nERROR: res_i8 = 2; saturate_value<int8_t>(res_i8, -1, 1); res_i8: %i, expected 1.\n\n", res_i8);

  int16_t res_i16;
  std::printf("Testing saturate_value_i16\n");
  res_i16 = -2;
  saturate_value_i16(&res_i16, -1, 1);
  if (res_i16 != -1) std::printf("\nERROR: res_i16 = -2; saturate_value_i16(&res_i16, -1, 1); res_i16: %i, expected -1.\n\n", res_i16);
  res_i16 = -1;
  saturate_value_i16(&res_i16, -1, 1);
  if (res_i16 != -1) std::printf("\nERROR: res_i16 = -1; saturate_value_i16(&res_i16, -1, 1); res_i16: %i, expected -1.\n\n", res_i16);
  res_i16 = 0;
  saturate_value_i16(&res_i16, -1, 1);
  if (res_i16 != 0) std::printf("\nERROR: res_i16 = 0; saturate_value_i16(&res_i16, -1, 1); res_i16: %i, expected 0.\n\n", res_i16);
  res_i16 = 1;
  saturate_value_i16(&res_i16, -1, 1);
  if (res_i16 != 1) std::printf("\nERROR: res_i16 = 1; saturate_value_i16(&res_i16, -1, 1); res_i16: %i, expected 1.\n\n", res_i16);
  res_i16 = 2;
  saturate_value_i16(&res_i16, -1, 1);
  if (res_i16 != 1) std::printf("\nERROR: res_i16 = 2; saturate_value_i16(&res_i16, -1, 1); res_i16: %i, expected 1.\n\n", res_i16);

  std::printf("Testing saturate_value<int16_t>\n");
  res_i16 = -2;
  saturate_value<int16_t>(res_i16, -1, 1);
  if (res_i16 != -1) std::printf("\nERROR: res_i16 = -2; saturate_value<int16_t>(res_i16, -1, 1); res_i16: %i, expected -1.\n\n", res_i16);
  res_i16 = -1;
  saturate_value<int16_t>(res_i16, -1, 1);
  if (res_i16 != -1) std::printf("\nERROR: res_i16 = -1; saturate_value<int16_t>(res_i16, -1, 1); res_i16: %i, expected -1.\n\n", res_i16);
  res_i16 = 0;
  saturate_value<int16_t>(res_i16, -1, 1);
  if (res_i16 != 0) std::printf("\nERROR: res_i16 = 0; saturate_value<int16_t>(res_i16, -1, 1); res_i16: %i, expected 0.\n\n", res_i16);
  res_i16 = 1;
  saturate_value<int16_t>(res_i16, -1, 1);
  if (res_i16 != 1) std::printf("\nERROR: res_i16 = 1; saturate_value<int16_t>(res_i16, -1, 1); res_i16: %i, expected 1.\n\n", res_i16);
  res_i16 = 2;
  saturate_value<int16_t>(res_i16, -1, 1);
  if (res_i16 != 1) std::printf("\nERROR: res_i16 = 2; saturate_value<int16_t>(res_i16, -1, 1); res_i16: %i, expected 1.\n\n", res_i16);

  int32_t res_i32;
  std::printf("Testing saturate_value_i32\n");
  res_i32 = -2;
  saturate_value_i32(&res_i32, -1, 1);
  if (res_i32 != -1) std::printf("\nERROR: res_i32 = -2; saturate_value_i32(&res_i32, -1, 1); res_i32: %i, expected -1.\n\n", res_i32);
  res_i32 = -1;
  saturate_value_i32(&res_i32, -1, 1);
  if (res_i32 != -1) std::printf("\nERROR: res_i32 = -1; saturate_value_i32(&res_i32, -1, 1); res_i32: %i, expected -1.\n\n", res_i32);
  res_i32 = 0;
  saturate_value_i32(&res_i32, -1, 1);
  if (res_i32 != 0) std::printf("\nERROR: res_i32 = 0; saturate_value_i32(&res_i32, -1, 1); res_i32: %i, expected 0.\n\n", res_i32);
  res_i32 = 1;
  saturate_value_i32(&res_i32, -1, 1);
  if (res_i32 != 1) std::printf("\nERROR: res_i32 = 1; saturate_value_i32(&res_i32, -1, 1); res_i32: %i, expected 1.\n\n", res_i32);
  res_i32 = 2;
  saturate_value_i32(&res_i32, -1, 1);
  if (res_i32 != 1) std::printf("\nERROR: res_i32 = 2; saturate_value_i32(&res_i32, -1, 1); res_i32: %i, expected 1.\n\n", res_i32);

  std::printf("Testing saturate_value<int32_t>\n");
  res_i32 = -2;
  saturate_value<int32_t>(res_i32, -1, 1);
  if (res_i32 != -1) std::printf("\nERROR: res_i32 = -2; saturate_value<int32_t>(res_i32, -1, 1); res_i32: %i, expected -1.\n\n", res_i32);
  res_i32 = -1;
  saturate_value<int32_t>(res_i32, -1, 1);
  if (res_i32 != -1) std::printf("\nERROR: res_i32 = -1; saturate_value<int32_t>(res_i32, -1, 1); res_i32: %i, expected -1.\n\n", res_i32);
  res_i32 = 0;
  saturate_value<int32_t>(res_i32, -1, 1);
  if (res_i32 != 0) std::printf("\nERROR: res_i32 = 0; saturate_value<int32_t>(res_i32, -1, 1); res_i32: %i, expected 0.\n\n", res_i32);
  res_i32 = 1;
  saturate_value<int32_t>(res_i32, -1, 1);
  if (res_i32 != 1) std::printf("\nERROR: res_i32 = 1; saturate_value<int32_t>(res_i32, -1, 1); res_i32: %i, expected 1.\n\n", res_i32);
  res_i32 = 2;
  saturate_value<int32_t>(res_i32, -1, 1);
  if (res_i32 != 1) std::printf("\nERROR: res_i32 = 2; saturate_value<int32_t>(res_i32, -1, 1); res_i32: %i, expected 1.\n\n", res_i32);

  int64_t res_i64;
  std::printf("Testing saturate_value_i64\n");
  res_i64 = -2ll;
  saturate_value_i64(&res_i64, -1ll, 1ll);
  if (res_i64 != -1ll) std::printf("\nERROR: res_i64 = -2; saturate_value_i64(&res_i64, -1, 1); res_i64: %" PRIi64 ", expected -1.\n\n", res_i64);
  res_i64 = -1ll;
  saturate_value_i64(&res_i64, -1ll, 1ll);
  if (res_i64 != -1ll) std::printf("\nERROR: res_i64 = -1; saturate_value_i64(&res_i64, -1, 1); res_i64: %" PRIi64 ", expected -1.\n\n", res_i64);
  res_i64 = 0ll;
  saturate_value_i64(&res_i64, -1ll, 1ll);
  if (res_i64 != 0ll) std::printf("\nERROR: res_i64 = 0; saturate_value_i64(&res_i64, -1, 1); res_i64: %" PRIi64 ", expected 0.\n\n", res_i64);
  res_i64 = 1ll;
  saturate_value_i64(&res_i64, -1ll, 1ll);
  if (res_i64 != 1ll) std::printf("\nERROR: res_i64 = 1; saturate_value_i64(&res_i64, -1, 1); res_i64: %" PRIi64 ", expected 1.\n\n", res_i64);
  res_i64 = 2ll;
  saturate_value_i64(&res_i64, -1ll, 1ll);
  if (res_i64 != 1ll) std::printf("\nERROR: res_i64 = 2; saturate_value_i64(&res_i64, -1, 1); res_i64: %" PRIi64 ", expected 1.\n\n", res_i64);

  std::printf("Testing saturate_value<int64_t>\n");
  res_i64 = -2ll;
  saturate_value<int64_t>(res_i64, -1ll, 1ll);
  if (res_i64 != -1ll) std::printf("\nERROR: res_i64 = -2; saturate_value<int64_t>(res_i64, -1, 1); res_i64: %" PRIi64 ", expected -1.\n\n", res_i64);
  res_i64 = -1ll;
  saturate_value<int64_t>(res_i64, -1ll, 1ll);
  if (res_i64 != -1ll) std::printf("\nERROR: res_i64 = -1; saturate_value<int64_t>(res_i64, -1, 1); res_i64: %" PRIi64 ", expected -1.\n\n", res_i64);
  res_i64 = 0ll;
  saturate_value<int64_t>(res_i64, -1ll, 1ll);
  if (res_i64 != 0ll) std::printf("\nERROR: res_i64 = 0; saturate_value<int64_t>(res_i64, -1, 1); res_i64: %" PRIi64 ", expected 0.\n\n", res_i64);
  res_i64 = 1ll;
  saturate_value<int64_t>(res_i64, -1ll, 1ll);
  if (res_i64 != 1ll) std::printf("\nERROR: res_i64 = 1; saturate_value<int64_t>(res_i64, -1, 1); res_i64: %" PRIi64 ", expected 1.\n\n", res_i64);
  res_i64 = 2ll;
  saturate_value<int64_t>(res_i64, -1ll, 1ll);
  if (res_i64 != 1ll) std::printf("\nERROR: res_i64 = 2; saturate_value<int64_t>(res_i64, -1, 1); res_i64: %" PRIi64 ", expected 1.\n\n", res_i64);

  uint8_t res_u8;
  std::printf("Testing saturate_value_u8\n");
  res_u8 = 0;
  saturate_value_u8(&res_u8, 1, 3);
  if (res_u8 != 1) std::printf("\nERROR: res_u8 = 0; saturate_value_u8(&res_u8, 1, 3); res_u8: %u, expected 1.\n\n", res_u8);
  res_u8 = 1;
  saturate_value_u8(&res_u8, 1, 3);
  if (res_u8 != 1) std::printf("\nERROR: res_u8 = 1; saturate_value_u8(&res_u8, 1, 3); res_u8: %u, expected 1.\n\n", res_u8);
  res_u8 = 2;
  saturate_value_u8(&res_u8, 1, 3);
  if (res_u8 != 2) std::printf("\nERROR: res_u8 = 2; saturate_value_u8(&res_u8, 1, 3); res_u8: %u, expected 2.\n\n", res_u8);
  res_u8 = 3;
  saturate_value_u8(&res_u8, 1, 3);
  if (res_u8 != 3) std::printf("\nERROR: res_u8 = 3; saturate_value_u8(&res_u8, 1, 3); res_u8: %u, expected 3.\n\n", res_u8);
  res_u8 = 4;
  saturate_value_u8(&res_u8, 1, 3);
  if (res_u8 != 3) std::printf("\nERROR: res_u8 = 4; saturate_value_u8(&res_u8, 1, 3); res_u8: %u, expected 3.\n\n", res_u8);

  std::printf("Testing saturate_value<uint8_t>\n");
  res_u8 = 0;
  saturate_value<uint8_t>(res_u8, 1, 3);
  if (res_u8 != 1) std::printf("\nERROR: res_u8 = 0; saturate_value<uint8_t>(res_u8, 1, 3); res_u8: %u, expected 1.\n\n", res_u8);
  res_u8 = 1;
  saturate_value<uint8_t>(res_u8, 1, 3);
  if (res_u8 != 1) std::printf("\nERROR: res_u8 = 1; saturate_value<uint8_t>(res_u8, 1, 3); res_u8: %u, expected 1.\n\n", res_u8);
  res_u8 = 2;
  saturate_value<uint8_t>(res_u8, 1, 3);
  if (res_u8 != 2) std::printf("\nERROR: res_u8 = 2; saturate_value<uint8_t>(res_u8, 1, 3); res_u8: %u, expected 2.\n\n", res_u8);
  res_u8 = 3;
  saturate_value<uint8_t>(res_u8, 1, 3);
  if (res_u8 != 3) std::printf("\nERROR: res_u8 = 3; saturate_value<uint8_t>(res_u8, 1, 3); res_u8: %u, expected 3.\n\n", res_u8);
  res_u8 = 4;
  saturate_value<uint8_t>(res_u8, 1, 3);
  if (res_u8 != 3) std::printf("\nERROR: res_u8 = 4; saturate_value<uint8_t>(res_u8, 1, 3); res_u8: %u, expected 3.\n\n", res_u8);

  uint16_t res_u16;
  std::printf("Testing saturate_value_u16\n");
  res_u16 = 0;
  saturate_value_u16(&res_u16, 1, 3);
  if (res_u16 != 1) std::printf("\nERROR: res_u16 = 0; saturate_value_u16(&res_u16, 1, 3); res_u16: %u, expected 1.\n\n", res_u16);
  res_u16 = 1;
  saturate_value_u16(&res_u16, 1, 3);
  if (res_u16 != 1) std::printf("\nERROR: res_u16 = 1; saturate_value_u16(&res_u16, 1, 3); res_u16: %u, expected 1.\n\n", res_u16);
  res_u16 = 2;
  saturate_value_u16(&res_u16, 1, 3);
  if (res_u16 != 2) std::printf("\nERROR: res_u16 = 2; saturate_value_u16(&res_u16, 1, 3); res_u16: %u, expected 2.\n\n", res_u16);
  res_u16 = 3;
  saturate_value_u16(&res_u16, 1, 3);
  if (res_u16 != 3) std::printf("\nERROR: res_u16 = 3; saturate_value_u16(&res_u16, 1, 3); res_u16: %u, expected 3.\n\n", res_u16);
  res_u16 = 4;
  saturate_value_u16(&res_u16, 1, 3);
  if (res_u16 != 3) std::printf("\nERROR: res_u16 = 4; saturate_value_u16(&res_u16, 1, 3); res_u16: %u, expected 3.\n\n", res_u16);

  std::printf("Testing saturate_value<uint16_t>\n");
  res_u16 = 0;
  saturate_value<uint16_t>(res_u16, 1, 3);
  if (res_u16 != 1) std::printf("\nERROR: res_u16 = 0; saturate_value<uint16_t>(res_u16, 1, 3); res_u16: %u, expected 1.\n\n", res_u16);
  res_u16 = 1;
  saturate_value<uint16_t>(res_u16, 1, 3);
  if (res_u16 != 1) std::printf("\nERROR: res_u16 = 1; saturate_value<uint16_t>(res_u16, 1, 3); res_u16: %u, expected 1.\n\n", res_u16);
  res_u16 = 2;
  saturate_value<uint16_t>(res_u16, 1, 3);
  if (res_u16 != 2) std::printf("\nERROR: res_u16 = 2; saturate_value<uint16_t>(res_u16, 1, 3); res_u16: %u, expected 2.\n\n", res_u16);
  res_u16 = 3;
  saturate_value<uint16_t>(res_u16, 1, 3);
  if (res_u16 != 3) std::printf("\nERROR: res_u16 = 3; saturate_value<uint16_t>(res_u16, 1, 3); res_u16: %u, expected 3.\n\n", res_u16);
  res_u16 = 4;
  saturate_value<uint16_t>(res_u16, 1, 3);
  if (res_u16 != 3) std::printf("\nERROR: res_u16 = 4; saturate_value<uint16_t>(res_u16, 1, 3); res_u16: %u, expected 3.\n\n", res_u16);

  uint32_t res_u32;
  std::printf("Testing saturate_value_u32\n");
  res_u32 = 0u;
  saturate_value_u32(&res_u32, 1u, 3u);
  if (res_u32 != 1u) std::printf("\nERROR: res_u32 = 0; saturate_value_u32(&res_u32, 1, 3); res_u32: %u, expected 1.\n\n", res_u32);
  res_u32 = 1u;
  saturate_value_u32(&res_u32, 1u, 3u);
  if (res_u32 != 1u) std::printf("\nERROR: res_u32 = 1; saturate_value_u32(&res_u32, 1, 3); res_u32: %u, expected 1.\n\n", res_u32);
  res_u32 = 2u;
  saturate_value_u32(&res_u32, 1u, 3u);
  if (res_u32 != 2u) std::printf("\nERROR: res_u32 = 2; saturate_value_u32(&res_u32, 1, 3); res_u32: %u, expected 2.\n\n", res_u32);
  res_u32 = 3u;
  saturate_value_u32(&res_u32, 1u, 3u);
  if (res_u32 != 3u) std::printf("\nERROR: res_u32 = 3; saturate_value_u32(&res_u32, 1, 3); res_u32: %u, expected 3.\n\n", res_u32);
  res_u32 = 4u;
  saturate_value_u32(&res_u32, 1u, 3u);
  if (res_u32 != 3u) std::printf("\nERROR: res_u32 = 4; saturate_value_u32(&res_u32, 1, 3); res_u32: %u, expected 3.\n\n", res_u32);

  std::printf("Testing saturate_value<uint32_t>\n");
  res_u32 = 0u;
  saturate_value<uint32_t>(res_u32, 1u, 3u);
  if (res_u32 != 1u) std::printf("\nERROR: res_u32 = 0; saturate_value<uint32_t>(res_u32, 1, 3); res_u32: %u, expected 1.\n\n", res_u32);
  res_u32 = 1u;
  saturate_value<uint32_t>(res_u32, 1u, 3u);
  if (res_u32 != 1u) std::printf("\nERROR: res_u32 = 1; saturate_value<uint32_t>(res_u32, 1, 3); res_u32: %u, expected 1.\n\n", res_u32);
  res_u32 = 2u;
  saturate_value<uint32_t>(res_u32, 1u, 3u);
  if (res_u32 != 2u) std::printf("\nERROR: res_u32 = 2; saturate_value<uint32_t>(res_u32, 1, 3); res_u32: %u, expected 2.\n\n", res_u32);
  res_u32 = 3u;
  saturate_value<uint32_t>(res_u32, 1u, 3u);
  if (res_u32 != 3u) std::printf("\nERROR: res_u32 = 3; saturate_value<uint32_t>(res_u32, 1, 3); res_u32: %u, expected 3.\n\n", res_u32);
  res_u32 = 4u;
  saturate_value<uint32_t>(res_u32, 1u, 3u);
  if (res_u32 != 3u) std::printf("\nERROR: res_u32 = 4; saturate_value<uint32_t>(res_u32, 1, 3); res_u32: %u, expected 3.\n\n", res_u32);

  uint64_t res_u64;
  std::printf("Testing saturate_value_u64\n");
  res_u64 = 0ull;
  saturate_value_u64(&res_u64, 1ull, 3ull);
  if (res_u64 != 1ull) std::printf("\nERROR: res_u64 = 0; saturate_value_u64(&res_u64, 1, 3); res_u64: %" PRIu64 ", expected 1.\n\n", res_u64);
  res_u64 = 1ull;
  saturate_value_u64(&res_u64, 1ull, 3ull);
  if (res_u64 != 1ull) std::printf("\nERROR: res_u64 = 1; saturate_value_u64(&res_u64, 1, 3); res_u64: %" PRIu64 ", expected 1.\n\n", res_u64);
  res_u64 = 2ull;
  saturate_value_u64(&res_u64, 1ull, 3ull);
  if (res_u64 != 2ull) std::printf("\nERROR: res_u64 = 2; saturate_value_u64(&res_u64, 1, 3); res_u64: %" PRIu64 ", expected 2.\n\n", res_u64);
  res_u64 = 3ull;
  saturate_value_u64(&res_u64, 1ull, 3ull);
  if (res_u64 != 3ull) std::printf("\nERROR: res_u64 = 3; saturate_value_u64(&res_u64, 1, 3); res_u64: %" PRIu64 ", expected 3.\n\n", res_u64);
  res_u64 = 4ull;
  saturate_value_u64(&res_u64, 1ull, 3ull);
  if (res_u64 != 3ull) std::printf("\nERROR: res_u64 = 4; saturate_value_u64(&res_u64, 1, 3); res_u64: %" PRIu64 ", expected 3.\n\n", res_u64);

  std::printf("Testing saturate_value<uint64_t>\n");
  res_u64 = 0ull;
  saturate_value<uint64_t>(res_u64, 1ull, 3ull);
  if (res_u64 != 1ull) std::printf("\nERROR: res_u64 = 0; saturate_value<uint64_t>(res_u64, 1, 3); res_u64: %" PRIu64 ", expected 1.\n\n", res_u64);
  res_u64 = 1ull;
  saturate_value<uint64_t>(res_u64, 1ull, 3ull);
  if (res_u64 != 1ull) std::printf("\nERROR: res_u64 = 1; saturate_value<uint64_t>(res_u64, 1, 3); res_u64: %" PRIu64 ", expected 1.\n\n", res_u64);
  res_u64 = 2ull;
  saturate_value<uint64_t>(res_u64, 1ull, 3ull);
  if (res_u64 != 2ull) std::printf("\nERROR: res_u64 = 2; saturate_value<uint64_t>(res_u64, 1, 3); res_u64: %" PRIu64 ", expected 2.\n\n", res_u64);
  res_u64 = 3ull;
  saturate_value<uint64_t>(res_u64, 1ull, 3ull);
  if (res_u64 != 3ull) std::printf("\nERROR: res_u64 = 3; saturate_value<uint64_t>(res_u64, 1, 3); res_u64: %" PRIu64 ", expected 3.\n\n", res_u64);
  res_u64 = 4ull;
  saturate_value<uint64_t>(res_u64, 1ull, 3ull);
  if (res_u64 != 3ull) std::printf("\nERROR: res_u64 = 4; saturate_value<uint64_t>(res_u64, 1, 3); res_u64: %" PRIu64 ", expected 3.\n\n", res_u64);

  std::printf("\nIf there are no errors above, the tests succeeded.\n\n");
  return 0;
}

/*
Creative Commons Legal Code

CC0 1.0 Universal

    CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE
    LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN
    ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS
    INFORMATION ON AN "AS-IS" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES
    REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS
    PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM
    THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED
    HEREUNDER.

Statement of Purpose

The laws of most jurisdictions throughout the world automatically confer
exclusive Copyright and Related Rights (defined below) upon the creator
and subsequent owner(s) (each and all, an "owner") of an original work of
authorship and/or a database (each, a "Work").

Certain owners wish to permanently relinquish those rights to a Work for
the purpose of contributing to a commons of creative, cultural and
scientific works ("Commons") that the public can reliably and without fear
of later claims of infringement build upon, modify, incorporate in other
works, reuse and redistribute as freely as possible in any form whatsoever
and for any purposes, including without limitation commercial purposes.
These owners may contribute to the Commons to promote the ideal of a free
culture and the further production of creative, cultural and scientific
works, or to gain reputation or greater distribution for their Work in
part through the use and efforts of others.

For these and/or other purposes and motivations, and without any
expectation of additional consideration or compensation, the person
associating CC0 with a Work (the "Affirmer"), to the extent that he or she
is an owner of Copyright and Related Rights in the Work, voluntarily
elects to apply CC0 to the Work and publicly distribute the Work under its
terms, with knowledge of his or her Copyright and Related Rights in the
Work and the meaning and intended legal effect of CC0 on those rights.

1. Copyright and Related Rights. A Work made available under CC0 may be
protected by copyright and related or neighboring rights ("Copyright and
Related Rights"). Copyright and Related Rights include, but are not
limited to, the following:

  i. the right to reproduce, adapt, distribute, perform, display,
     communicate, and translate a Work;
 ii. moral rights retained by the original author(s) and/or performer(s);
iii. publicity and privacy rights pertaining to a person's image or
     likeness depicted in a Work;
 iv. rights protecting against unfair competition in regards to a Work,
     subject to the limitations in paragraph 4(a), below;
  v. rights protecting the extraction, dissemination, use and reuse of data
     in a Work;
 vi. database rights (such as those arising under Directive 96/9/EC of the
     European Parliament and of the Council of 11 March 1996 on the legal
     protection of databases, and under any national implementation
     thereof, including any amended or successor version of such
     directive); and
vii. other similar, equivalent or corresponding rights throughout the
     world based on applicable law or treaty, and any national
     implementations thereof.

2. Waiver. To the greatest extent permitted by, but not in contravention
of, applicable law, Affirmer hereby overtly, fully, permanently,
irrevocably and unconditionally waives, abandons, and surrenders all of
Affirmer's Copyright and Related Rights and associated claims and causes
of action, whether now known or unknown (including existing as well as
future claims and causes of action), in the Work (i) in all territories
worldwide, (ii) for the maximum duration provided by applicable law or
treaty (including future time extensions), (iii) in any current or future
medium and for any number of copies, and (iv) for any purpose whatsoever,
including without limitation commercial, advertising or promotional
purposes (the "Waiver"). Affirmer makes the Waiver for the benefit of each
member of the public at large and to the detriment of Affirmer's heirs and
successors, fully intending that such Waiver shall not be subject to
revocation, rescission, cancellation, termination, or any other legal or
equitable action to disrupt the quiet enjoyment of the Work by the public
as contemplated by Affirmer's express Statement of Purpose.

3. Public License Fallback. Should any part of the Waiver for any reason
be judged legally invalid or ineffective under applicable law, then the
Waiver shall be preserved to the maximum extent permitted taking into
account Affirmer's express Statement of Purpose. In addition, to the
extent the Waiver is so judged Affirmer hereby grants to each affected
person a royalty-free, non transferable, non sublicensable, non exclusive,
irrevocable and unconditional license to exercise Affirmer's Copyright and
Related Rights in the Work (i) in all territories worldwide, (ii) for the
maximum duration provided by applicable law or treaty (including future
time extensions), (iii) in any current or future medium and for any number
of copies, and (iv) for any purpose whatsoever, including without
limitation commercial, advertising or promotional purposes (the
"License"). The License shall be deemed effective as of the date CC0 was
applied by Affirmer to the Work. Should any part of the License for any
reason be judged legally invalid or ineffective under applicable law, such
partial invalidity or ineffectiveness shall not invalidate the remainder
of the License, and in such case Affirmer hereby affirms that he or she
will not (i) exercise any of his or her remaining Copyright and Related
Rights in the Work or (ii) assert any associated claims and causes of
action with respect to the Work, in either case contrary to Affirmer's
express Statement of Purpose.

4. Limitations and Disclaimers.

 a. No trademark or patent rights held by Affirmer are waived, abandoned,
    surrendered, licensed or otherwise affected by this document.
 b. Affirmer offers the Work as-is and makes no representations or
    warranties of any kind concerning the Work, express, implied,
    statutory or otherwise, including without limitation warranties of
    title, merchantability, fitness for a particular purpose, non
    infringement, or the absence of latent or other defects, accuracy, or
    the present or absence of errors, whether or not discoverable, all to
    the greatest extent permissible under applicable law.
 c. Affirmer disclaims responsibility for clearing rights of other persons
    that may apply to the Work or any use thereof, including without
    limitation any person's Copyright and Related Rights in the Work.
    Further, Affirmer disclaims responsibility for obtaining any necessary
    consents, permissions or other rights required for any use of the
    Work.
 d. Affirmer understands and acknowledges that Creative Commons is not a
    party to this document and has no duty or obligation with respect to
    this CC0 or use of the Work.
*/