/**
 * test_input_protections.cpp
 * Runs tests to verify that multshiftround and shiftround functions which
 * evaluate the shift argument at runtime return 0 for out of range shift
 * values.
 *
 * Also runs tests to verify that all divround functions return the dividend
 * when the divisor is 0 and that divround functions operating on signed
 * types return the maximum representable positive value when the maximum
 * representable negative value is divided by -1.
 * 
 * All compilation units required for the tests in this file should be
 * compiled without DEBUG_INTMATH defined, as the debug code emits 
 * error messages for the inputs under test.
 * 
 * Written in 2019 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cstdio>
#include <limits>

#include "multshiftround_run.hpp"
#include "shiftround_run.hpp"
#include "divround.hpp"

#ifdef __cplusplus
  extern "C"
  {
#endif
    #include "multshiftround_run.h"
    #include "shiftround_run.h"
    #include "divround.h"
#ifdef __cplusplus
  }
#endif

int main() {

  std::printf("\nVerifying that invalid function input arguments return sane results.\n\n");

  uint8_t shift = 7u;
  while (true) {
    int8_t result = multshiftround_i8(-16, 8, shift);
    if (result != 0) std::printf("ERROR: multshiftround_i8(-16, 8, %u) returned %i, expected 0.\n\n", shift, result);

    result = multshiftround<int8_t>(-16, 8, shift);
    if (result != 0) std::printf("ERROR: multshiftround<int8_t>(-16, 8, %u) returned %i, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 8u;
  while (true) {
    uint8_t result = multshiftround_u8(15u, 17u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround_u8(15, 17, %u) returned %u, expected 0.\n\n", shift, result);

    result = multshiftround<uint8_t>(15u, 17u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround<uint8_t>(15, 17, %u) returned %u, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 15u;
  while (true) {
    int16_t result = multshiftround_i16(-128, 256, shift);
    if (result != 0) std::printf("ERROR: multshiftround_i16(-128, 256, %u) returned %i, expected 0.\n\n", shift, result);

    result = multshiftround<int16_t>(-128, 256, shift);
    if (result != 0) std::printf("ERROR: multshiftround<int16_t>(-128, 256, %u) returned %i, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 16u;
  while (true) {
    uint16_t result = multshiftround_u16(255u, 257u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround_u16(255, 257, %u) returned %u, expected 0.\n\n", shift, result);

    result = multshiftround<uint16_t>(255u, 257u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround<uint16_t>(255, 257, %u) returned %u, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 31u;
  while (true) {
    int32_t result = multshiftround_i32(-32768, 65536, shift);
    if (result != 0) std::printf("ERROR: multshiftround_i32(-32768, 65536, %u) returned %i, expected 0.\n\n", shift, result);

    result = multshiftround<int32_t>(-32768, 65536, shift);
    if (result != 0) std::printf("ERROR: multshiftround<int32_t>(-32768, 65536, %u) returned %i, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 32u;
  while (true) {
    uint32_t result = multshiftround_u32(65535u, 65537u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround_u32(65535, 65537, %u) returned %u, expected 0.\n\n", shift, result);

    result = multshiftround<uint32_t>(65535u, 65537u, shift);
    if (result != 0u) std::printf("ERROR: multshiftround<uint32_t>(65535, 65537, %u) returned %u, expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 63u;
  while (true) {
    int64_t result = multshiftround_i64(-2147483648ll, 4294967296ll, shift);
    if (result != 0ll) std::printf("ERROR: multshiftround_i64(-2147483648, 4294967296, %u) returned %" PRIi64 ", expected 0.\n\n", shift, result);

    result = multshiftround<int64_t>(-2147483648ll, 4294967296ll, shift);
    if (result != 0ll) std::printf("ERROR: multshiftround<int64_t>(-2147483648, 4294967296, %u) returned %" PRIi64 ", expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 64u;
  while (true) {
    uint64_t result = multshiftround_u64(4294967295ull, 4294967297ull, shift);
    if (result != 0ull) std::printf("ERROR: multshiftround_u64(4294967295, 4294967297, %u) returned %" PRIu64 ", expected 0.\n\n", shift, result);

    result = multshiftround<uint64_t>(4294967295ull, 4294967297ull, shift);
    if (result != 0ull) std::printf("ERROR: multshiftround<uint64_t>(4294967295, 4294967297, %u) returned %" PRIu64 ", expected 0.\n\n", shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 7u;
  while (true) {
    int8_t result = shiftround_i8(std::numeric_limits<int8_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround_i8(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int8_t>::min(), shift, result);

    result = shiftround<int8_t>(std::numeric_limits<int8_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround<int8_t>(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int8_t>::min(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 8u;
  while (true) {
    uint8_t result = shiftround_u8(std::numeric_limits<uint8_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround_u8(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint8_t>::max(), shift, result);

    result = shiftround<uint8_t>(std::numeric_limits<uint8_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround<uint8_t>(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint8_t>::max(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 15u;
  while (true) {
    int16_t result = shiftround_i16(std::numeric_limits<int16_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround_i16(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int16_t>::min(), shift, result);

    result = shiftround<int16_t>(std::numeric_limits<int16_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround<int16_t>(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int16_t>::min(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 16u;
  while (true) {
    uint16_t result = shiftround_u16(std::numeric_limits<uint16_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround_u16(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint16_t>::max(), shift, result);

    result = shiftround<uint16_t>(std::numeric_limits<uint16_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround<uint16_t>(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint16_t>::max(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 31u;
  while (true) {
    int32_t result = shiftround_i32(std::numeric_limits<int32_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround_i32(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int32_t>::min(), shift, result);

    result = shiftround<int32_t>(std::numeric_limits<int32_t>::min(), shift);
    if (result != 0) std::printf("ERROR: shiftround<int32_t>(%i, %u) returned %i, expected 0.\n\n", std::numeric_limits<int32_t>::min(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 32u;
  while (true) {
    uint32_t result = shiftround_u32(std::numeric_limits<uint32_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround_u32(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint32_t>::max(), shift, result);

    result = shiftround<uint32_t>(std::numeric_limits<uint32_t>::max(), shift);
    if (result != 0u) std::printf("ERROR: shiftround<uint32_t>(%u, %u) returned %u, expected 0.\n\n", std::numeric_limits<uint32_t>::max(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 63u;
  while (true) {
    int64_t result = shiftround_i64(std::numeric_limits<int64_t>::min(), shift);
    if (result != 0ll) std::printf("ERROR: shiftround_i64(%" PRIi64 ", %u) returned %" PRIi64 ", expected 0.\n\n", std::numeric_limits<int64_t>::min(), shift, result);

    result = shiftround<int64_t>(std::numeric_limits<int64_t>::min(), shift);
    if (result != 0ll) std::printf("ERROR: shiftround<int64_t>(%" PRIi64 ", %u) returned %" PRIi64 ", expected 0.\n\n", std::numeric_limits<int64_t>::min(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  shift = 64u;
  while (true) {
    uint64_t result = shiftround_u64(std::numeric_limits<uint64_t>::max(), shift);
    if (result != 0ull) std::printf("ERROR: shiftround_u64(%" PRIu64 ", %u) returned %" PRIu64 ", expected 0.\n\n", std::numeric_limits<uint64_t>::max(), shift, result);

    result = shiftround<uint64_t>(std::numeric_limits<uint64_t>::max(), shift);
    if (result != 0ull) std::printf("ERROR: shiftround<uint64_t>(%" PRIu64 ", %u) returned %" PRIu64 ", expected 0.\n\n", std::numeric_limits<uint64_t>::max(), shift, result);

    if (shift == std::numeric_limits<uint8_t>::max()) break;
    shift++;
  }

  int8_t divr_i8 = divround_i8(0, 0);
  if (divr_i8 != 0) std::printf("ERROR: divround_i8(0, 0) returned %i, expected 0.\n\n", divr_i8);
  
  divr_i8 = divround<int8_t>(0, 0);
  if (divr_i8 != 0) std::printf("ERROR: divround<int8_t>(0, 0) returned %i, expected 0.\n\n", divr_i8);

  divr_i8 = divround_i8(std::numeric_limits<int8_t>::min(), 0);
  if (divr_i8 != std::numeric_limits<int8_t>::min()) std::printf("ERROR: divround_i8(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::min(), divr_i8, std::numeric_limits<int8_t>::min());

  divr_i8 = divround<int8_t>(std::numeric_limits<int8_t>::min(), 0);
  if (divr_i8 != std::numeric_limits<int8_t>::min()) std::printf("ERROR: divround<int8_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::min(), divr_i8, std::numeric_limits<int8_t>::min());

  divr_i8 = divround_i8(std::numeric_limits<int8_t>::max(), 0);
  if (divr_i8 != std::numeric_limits<int8_t>::max()) std::printf("ERROR: divround_i8(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::max(), divr_i8, std::numeric_limits<int8_t>::max());

  divr_i8 = divround<int8_t>(std::numeric_limits<int8_t>::max(), 0);
  if (divr_i8 != std::numeric_limits<int8_t>::max()) std::printf("ERROR: divround<int8_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::max(), divr_i8, std::numeric_limits<int8_t>::max());

  divr_i8 = divround_i8(std::numeric_limits<int8_t>::min(), -1);
  if (divr_i8 != std::numeric_limits<int8_t>::max()) std::printf("ERROR: divround_i8(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::min(), divr_i8, std::numeric_limits<int8_t>::max());

  divr_i8 = divround<int8_t>(std::numeric_limits<int8_t>::min(), -1);
  if (divr_i8 != std::numeric_limits<int8_t>::max()) std::printf("ERROR: divround<int8_t>(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int8_t>::min(), divr_i8, std::numeric_limits<int8_t>::max());

  int16_t divr_i16 = divround_i16(0, 0);
  if (divr_i16 != 0) std::printf("ERROR: divround_i16(0, 0) returned %i, expected 0.\n\n", divr_i16);
  
  divr_i16 = divround<int16_t>(0, 0);
  if (divr_i16 != 0) std::printf("ERROR: divround<int16_t>(0, 0) returned %i, expected 0.\n\n", divr_i16);

  divr_i16 = divround_i16(std::numeric_limits<int16_t>::min(), 0);
  if (divr_i16 != std::numeric_limits<int16_t>::min()) std::printf("ERROR: divround_i16(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::min(), divr_i16, std::numeric_limits<int16_t>::min());

  divr_i16 = divround<int16_t>(std::numeric_limits<int16_t>::min(), 0);
  if (divr_i16 != std::numeric_limits<int16_t>::min()) std::printf("ERROR: divround<int16_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::min(), divr_i16, std::numeric_limits<int16_t>::min());

  divr_i16 = divround_i16(std::numeric_limits<int16_t>::max(), 0);
  if (divr_i16 != std::numeric_limits<int16_t>::max()) std::printf("ERROR: divround_i16(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::max(), divr_i16, std::numeric_limits<int16_t>::max());

  divr_i16 = divround<int16_t>(std::numeric_limits<int16_t>::max(), 0);
  if (divr_i16 != std::numeric_limits<int16_t>::max()) std::printf("ERROR: divround<int16_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::max(), divr_i16, std::numeric_limits<int16_t>::max());

  divr_i16 = divround_i16(std::numeric_limits<int16_t>::min(), -1);
  if (divr_i16 != std::numeric_limits<int16_t>::max()) std::printf("ERROR: divround_i16(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::min(), divr_i16, std::numeric_limits<int16_t>::max());

  divr_i16 = divround<int16_t>(std::numeric_limits<int16_t>::min(), -1);
  if (divr_i16 != std::numeric_limits<int16_t>::max()) std::printf("ERROR: divround<int16_t>(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int16_t>::min(), divr_i16, std::numeric_limits<int16_t>::max());

  int32_t divr_i32 = divround_i32(0, 0);
  if (divr_i32 != 0) std::printf("ERROR: divround_i32(0, 0) returned %i, expected 0.\n\n", divr_i32);
  
  divr_i32 = divround<int32_t>(0, 0);
  if (divr_i32 != 0) std::printf("ERROR: divround<int32_t>(0, 0) returned %i, expected 0.\n\n", divr_i32);

  divr_i32 = divround_i32(std::numeric_limits<int32_t>::min(), 0);
  if (divr_i32 != std::numeric_limits<int32_t>::min()) std::printf("ERROR: divround_i32(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::min(), divr_i32, std::numeric_limits<int32_t>::min());

  divr_i32 = divround<int32_t>(std::numeric_limits<int32_t>::min(), 0);
  if (divr_i32 != std::numeric_limits<int32_t>::min()) std::printf("ERROR: divround<int32_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::min(), divr_i32, std::numeric_limits<int32_t>::min());

  divr_i32 = divround_i32(std::numeric_limits<int32_t>::max(), 0);
  if (divr_i32 != std::numeric_limits<int32_t>::max()) std::printf("ERROR: divround_i32(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::max(), divr_i32, std::numeric_limits<int32_t>::max());

  divr_i32 = divround<int32_t>(std::numeric_limits<int32_t>::max(), 0);
  if (divr_i32 != std::numeric_limits<int32_t>::max()) std::printf("ERROR: divround<int32_t>(%i, 0) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::max(), divr_i32, std::numeric_limits<int32_t>::max());

  divr_i32 = divround_i32(std::numeric_limits<int32_t>::min(), -1);
  if (divr_i32 != std::numeric_limits<int32_t>::max()) std::printf("ERROR: divround_i32(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::min(), divr_i32, std::numeric_limits<int32_t>::max());

  divr_i32 = divround<int32_t>(std::numeric_limits<int32_t>::min(), -1);
  if (divr_i32 != std::numeric_limits<int32_t>::max()) std::printf("ERROR: divround<int32_t>(%i, -1) returned %i, expected %i.\n\n", std::numeric_limits<int32_t>::min(), divr_i32, std::numeric_limits<int32_t>::max());

  int64_t divr_i64 = divround_i64(0ll, 0ll);
  if (divr_i64 != 0ll) std::printf("ERROR: divround_i64(0, 0) returned %" PRIi64 ", expected 0.\n\n", divr_i64);
  
  divr_i64 = divround<int64_t>(0ll, 0ll);
  if (divr_i64 != 0ll) std::printf("ERROR: divround<int64_t>(0, 0) returned %" PRIi64 ", expected 0.\n\n", divr_i64);

  divr_i64 = divround_i64(std::numeric_limits<int64_t>::min(), 0ll);
  if (divr_i64 != std::numeric_limits<int64_t>::min()) std::printf("ERROR: divround_i64(%" PRIi64 ", 0) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::min(), divr_i64, std::numeric_limits<int64_t>::min());

  divr_i64 = divround<int64_t>(std::numeric_limits<int64_t>::min(), 0ll);
  if (divr_i64 != std::numeric_limits<int64_t>::min()) std::printf("ERROR: divround<int64_t>(%" PRIi64 ", 0) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::min(), divr_i64, std::numeric_limits<int64_t>::min());

  divr_i64 = divround_i64(std::numeric_limits<int64_t>::max(), 0ll);
  if (divr_i64 != std::numeric_limits<int64_t>::max()) std::printf("ERROR: divround_i64(%" PRIi64 ", 0) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::max(), divr_i64, std::numeric_limits<int64_t>::max());

  divr_i64 = divround<int64_t>(std::numeric_limits<int64_t>::max(), 0ll);
  if (divr_i64 != std::numeric_limits<int64_t>::max()) std::printf("ERROR: divround<int64_t>(%" PRIi64 ", 0) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::max(), divr_i64, std::numeric_limits<int64_t>::max());

  divr_i64 = divround_i64(std::numeric_limits<int64_t>::min(), -1ll);
  if (divr_i64 != std::numeric_limits<int64_t>::max()) std::printf("ERROR: divround_i64(%" PRIi64 ", -1) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::min(), divr_i64, std::numeric_limits<int64_t>::max());

  divr_i64 = divround<int64_t>(std::numeric_limits<int64_t>::min(), -1ll);
  if (divr_i64 != std::numeric_limits<int64_t>::max()) std::printf("ERROR: divround<int64_t>(%" PRIi64 ", -1) returned %" PRIi64 ", expected %" PRIi64 ".\n\n", std::numeric_limits<int64_t>::min(), divr_i64, std::numeric_limits<int64_t>::max());

  uint8_t divr_u8 = divround_u8(std::numeric_limits<uint8_t>::min(), 0u);
  if (divr_u8 != std::numeric_limits<uint8_t>::min()) std::printf("ERROR: divround_u8(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint8_t>::min(), divr_u8, std::numeric_limits<uint8_t>::min());

  divr_u8 = divround<uint8_t>(std::numeric_limits<uint8_t>::min(), 0u);
  if (divr_u8 != std::numeric_limits<uint8_t>::min()) std::printf("ERROR: divround<uint8_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint8_t>::min(), divr_u8, std::numeric_limits<uint8_t>::min());

  divr_u8 = divround_u8(std::numeric_limits<uint8_t>::max(), 0u);
  if (divr_u8 != std::numeric_limits<uint8_t>::max()) std::printf("ERROR: divround_u8(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint8_t>::max(), divr_u8, std::numeric_limits<uint8_t>::max());

  divr_u8 = divround<uint8_t>(std::numeric_limits<uint8_t>::max(), 0u);
  if (divr_u8 != std::numeric_limits<uint8_t>::max()) std::printf("ERROR: divround<uint8_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint8_t>::max(), divr_u8, std::numeric_limits<uint8_t>::max());

  uint16_t divr_u16 = divround_u16(std::numeric_limits<uint16_t>::min(), 0u);
  if (divr_u16 != std::numeric_limits<uint16_t>::min()) std::printf("ERROR: divround_u16(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint16_t>::min(), divr_u16, std::numeric_limits<uint16_t>::min());

  divr_u16 = divround<uint16_t>(std::numeric_limits<uint16_t>::min(), 0u);
  if (divr_u16 != std::numeric_limits<uint16_t>::min()) std::printf("ERROR: divround<uint16_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint16_t>::min(), divr_u16, std::numeric_limits<uint16_t>::min());

  divr_u16 = divround_u16(std::numeric_limits<uint16_t>::max(), 0u);
  if (divr_u16 != std::numeric_limits<uint16_t>::max()) std::printf("ERROR: divround_u16(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint16_t>::max(), divr_u16, std::numeric_limits<uint16_t>::max());

  divr_u16 = divround<uint16_t>(std::numeric_limits<uint16_t>::max(), 0u);
  if (divr_u16 != std::numeric_limits<uint16_t>::max()) std::printf("ERROR: divround<uint16_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint16_t>::max(), divr_u16, std::numeric_limits<uint16_t>::max());

  uint32_t divr_u32 = divround_u32(std::numeric_limits<uint32_t>::min(), 0u);
  if (divr_u32 != std::numeric_limits<uint32_t>::min()) std::printf("ERROR: divround_u32(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint32_t>::min(), divr_u32, std::numeric_limits<uint32_t>::min());

  divr_u32 = divround<uint32_t>(std::numeric_limits<uint32_t>::min(), 0u);
  if (divr_u32 != std::numeric_limits<uint32_t>::min()) std::printf("ERROR: divround<uint32_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint32_t>::min(), divr_u32, std::numeric_limits<uint32_t>::min());

  divr_u32 = divround_u32(std::numeric_limits<uint32_t>::max(), 0u);
  if (divr_u32 != std::numeric_limits<uint32_t>::max()) std::printf("ERROR: divround_u32(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint32_t>::max(), divr_u32, std::numeric_limits<uint32_t>::max());

  divr_u32 = divround<uint32_t>(std::numeric_limits<uint32_t>::max(), 0u);
  if (divr_u32 != std::numeric_limits<uint32_t>::max()) std::printf("ERROR: divround<uint32_t>(%u, 0) returned %u, expected %u.\n\n", std::numeric_limits<uint32_t>::max(), divr_u32, std::numeric_limits<uint32_t>::max());

  uint64_t divr_u64 = divround_u64(std::numeric_limits<uint64_t>::min(), 0ull);
  if (divr_u64 != std::numeric_limits<uint64_t>::min()) std::printf("ERROR: divround_u64(%" PRIu64 ", 0) returned %" PRIu64 ", expected %" PRIu64 ".\n\n", std::numeric_limits<uint64_t>::min(), divr_u64, std::numeric_limits<uint64_t>::min());

  divr_u64 = divround<uint64_t>(std::numeric_limits<uint64_t>::min(), 0ull);
  if (divr_u64 != std::numeric_limits<uint64_t>::min()) std::printf("ERROR: divround<uint64_t>(%" PRIu64 ", 0) returned %" PRIu64 ", expected %" PRIu64 ".\n\n", std::numeric_limits<uint64_t>::min(), divr_u64, std::numeric_limits<uint64_t>::min());

  divr_u64 = divround_u64(std::numeric_limits<uint64_t>::max(), 0ull);
  if (divr_u64 != std::numeric_limits<uint64_t>::max()) std::printf("ERROR: divround_u64(%" PRIu64 ", 0) returned %" PRIu64 ", expected %" PRIu64 ".\n\n", std::numeric_limits<uint64_t>::max(), divr_u64, std::numeric_limits<uint64_t>::max());

  divr_u64 = divround<uint64_t>(std::numeric_limits<uint64_t>::max(), 0ull);
  if (divr_u64 != std::numeric_limits<uint64_t>::max()) std::printf("ERROR: divround<uint64_t>(%" PRIu64 ", 0) returned %" PRIu64 ", expected %" PRIu64 ".\n\n", std::numeric_limits<uint64_t>::max(), divr_u64, std::numeric_limits<uint64_t>::max());

  std::printf("Test finished. It succeeded if there are no errors above.\n\n");

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
