/**
 * test_debug_code.cpp
 * This file exercises all of the error statements inside
 *   #ifdef DEBUG_INTMATH
 *     // Program Code
 *   #endif
 * blocks for c style and c++ style
 * divround,
 * shiftround run type,
 * multshiftround run type, and
 * multshiftround comp type
 * routines. These are all of the routines with such
 * debug code.
 * 
 * Written in 2019 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cinttypes>
#include <cstdio>
#include <limits>

#include "divround.hpp"
#include "multshiftround_comp.hpp"
#include "multshiftround_run.hpp"
#include "shiftround_run.hpp"

#ifdef __cplusplus
  extern "C"
  {
#endif
    #include "divround.h"
    #include "multshiftround_comp.h"
    #include "multshiftround_run.h"
    #include "shiftround_run.h"
#ifdef __cplusplus
  }
#endif

int main() {
  std::printf("\nEach printed function call below should result in an error.\n\n");

  std::printf("divround<int8_t>(1, 0):\n  ");
  divround<int8_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int8_t>(%i, -1):\n  ", std::numeric_limits<int8_t>::min());
  divround<int8_t>(std::numeric_limits<int8_t>::min(), -1);
  std::printf("\n");

  std::printf("divround<uint8_t>(1, 0):\n  ");
  divround<uint8_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int16_t>(1, 0):\n  ");
  divround<int16_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int16_t>(%i, -1):\n  ", std::numeric_limits<int16_t>::min());
  divround<int16_t>(std::numeric_limits<int16_t>::min(), -1);
  std::printf("\n");

  std::printf("divround<uint16_t>(1, 0):\n  ");
  divround<uint16_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int32_t>(1, 0):\n  ");
  divround<int32_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int32_t>(%i, -1):\n  ", std::numeric_limits<int32_t>::min());
  divround<int32_t>(std::numeric_limits<int32_t>::min(), -1);
  std::printf("\n");

  std::printf("divround<uint32_t>(1, 0):\n  ");
  divround<uint32_t>(1, 0);
  std::printf("\n");

  std::printf("divround<int64_t>(1, 0):\n  ");
  divround<int64_t>(1ll, 0ll);
  std::printf("\n");

  std::printf("divround<int64_t>(%" PRIi64 ", -1):\n  ", std::numeric_limits<int64_t>::min());
  divround<int64_t>(std::numeric_limits<int64_t>::min(), -1ll);
  std::printf("\n");

  std::printf("divround<uint64_t>(1, 0):\n  ");
  divround<uint64_t>(1ull, 0ull);
  std::printf("\n");

  std::printf("divround_i8(1, 0):\n  ");
  divround_i8(1, 0);
  std::printf("\n");

  std::printf("divround_i8(%i, -1):\n  ", std::numeric_limits<int8_t>::min());
  divround_i8(std::numeric_limits<int8_t>::min(), -1);
  std::printf("\n");

  std::printf("divround_u8(1, 0):\n  ");
  divround_u8(1, 0);
  std::printf("\n");

  std::printf("divround_i16(1, 0):\n  ");
  divround_i16(1, 0);
  std::printf("\n");

  std::printf("divround_i16(%i, -1):\n  ", std::numeric_limits<int16_t>::min());
  divround_i16(std::numeric_limits<int16_t>::min(), -1);
  std::printf("\n");

  std::printf("divround_u16(1, 0):\n  ");
  divround_u16(1, 0);
  std::printf("\n");

  std::printf("divround_i32(1, 0):\n  ");
  divround_i32(1, 0);
  std::printf("\n");

  std::printf("divround_i32(%i, -1):\n  ", std::numeric_limits<int32_t>::min());
  divround_i32(std::numeric_limits<int32_t>::min(), -1);
  std::printf("\n");

  std::printf("divround_u32(1, 0):\n  ");
  divround_u32(1, 0);
  std::printf("\n");

  std::printf("divround_i64(1, 0):\n  ");
  divround_i64(1ll, 0ll);
  std::printf("\n");

  std::printf("divround_i64(%" PRIi64 ", -1):\n  ", std::numeric_limits<int64_t>::min());
  divround_i64(std::numeric_limits<int64_t>::min(), -1ll);
  std::printf("\n");

  std::printf("divround_u64(1, 0):\n  ");
  divround_u64(1ull, 0ull);
  std::printf("\n");

  std::printf("shiftround<int8_t>(0, 7):\n  ");
  shiftround<int8_t>(0,7);
  std::printf("\n");

  std::printf("shiftround<uint8_t>(0, 8):\n  ");
  shiftround<uint8_t>(0,8);
  std::printf("\n");

  std::printf("shiftround<int16_t>(0, 15):\n  ");
  shiftround<int16_t>(0,15);
  std::printf("\n");

  std::printf("shiftround<uint16_t>(0, 16):\n  ");
  shiftround<uint16_t>(0,16);
  std::printf("\n");

  std::printf("shiftround<int32_t>(0, 31):\n  ");
  shiftround<int32_t>(0,31);
  std::printf("\n");

  std::printf("shiftround<uint32_t>(0, 32):\n  ");
  shiftround<uint32_t>(0,32);
  std::printf("\n");

  std::printf("shiftround<int64_t>(0, 63):\n  ");
  shiftround<int64_t>(0,63);
  std::printf("\n");

  std::printf("shiftround<uint64_t>(0, 64):\n  ");
  shiftround<uint64_t>(0,64);
  std::printf("\n");

  std::printf("shiftround_i8(0, 7):\n  ");
  shiftround_i8(0,7);
  std::printf("\n");

  std::printf("shiftround_u8(0, 8):\n  ");
  shiftround_u8(0,8);
  std::printf("\n");

  std::printf("shiftround_i16(0, 15):\n  ");
  shiftround_i16(0,15);
  std::printf("\n");

  std::printf("shiftround_u16(0, 16):\n  ");
  shiftround_u16(0,16);
  std::printf("\n");

  std::printf("shiftround_i32(0, 31):\n  ");
  shiftround_i32(0,31);
  std::printf("\n");

  std::printf("shiftround_u32(0, 32):\n  ");
  shiftround_u32(0,32);
  std::printf("\n");

  std::printf("shiftround_i64(0, 63):\n  ");
  shiftround_i64(0,63);
  std::printf("\n");

  std::printf("shiftround_u64(0, 64):\n  ");
  shiftround_u64(0,64);
  std::printf("\n");

  std::printf("multshiftround<int8_t>(0, 0, 7):\n  ");
  multshiftround<int8_t>(0,0,7);
  std::printf("\n");

  std::printf("multshiftround<int8_t>(64, 2, 6):\n  ");
  multshiftround<int8_t>(64,2,6);
  std::printf("\n");

  std::printf("multshiftround<int8_t>(-3, 43, 6):\n  ");
  multshiftround<int8_t>(-3,43,6);
  std::printf("\n");

  std::printf("multshiftround<uint8_t>(0, 0, 8):\n  ");
  multshiftround<uint8_t>(0,0,8);
  std::printf("\n");

  std::printf("multshiftround<uint8_t>(128, 2, 6):\n  ");
  multshiftround<uint8_t>(128,2,6);
  std::printf("\n");

  std::printf("multshiftround<int16_t>(0, 0, 15):\n  ");
  multshiftround<int16_t>(0,0,15);
  std::printf("\n");

  std::printf("multshiftround<int16_t>(16384, 2, 14):\n  ");
  multshiftround<int16_t>(16384,2,14);
  std::printf("\n");

  std::printf("multshiftround<int16_t>(-99, 331, 14):\n  ");
  multshiftround<int16_t>(-99,331,14);
  std::printf("\n");

  std::printf("multshiftround<uint16_t>(0, 0, 16):\n  ");
  multshiftround<uint16_t>(0,0,16);
  std::printf("\n");

  std::printf("multshiftround<uint16_t>(32768, 2, 15):\n  ");
  multshiftround<uint16_t>(32768,2,15);
  std::printf("\n");

  std::printf("multshiftround<int32_t>(0, 0, 31):\n  ");
  multshiftround<int32_t>(0,0,31);
  std::printf("\n");

  std::printf("multshiftround<int32_t>(1073741824, 2, 30):\n  ");
  multshiftround<int32_t>(1073741824,2,30);
  std::printf("\n");

  std::printf("multshiftround<int32_t>(-3, 715827883, 30):\n  ");
  multshiftround<int32_t>(-3,715827883,30);
  std::printf("\n");

  std::printf("multshiftround<uint32_t>(0, 0, 32):\n  ");
  multshiftround<uint32_t>(0u,0u,32);
  std::printf("\n");

  std::printf("multshiftround<uint32_t>(2147483648, 2, 31):\n  ");
  multshiftround<uint32_t>(2147483648u,2u,31);
  std::printf("\n");

  std::printf("multshiftround<int64_t>(0, 0, 63):\n  ");
  multshiftround<int64_t>(0,0,63);
  std::printf("\n");

  std::printf("multshiftround<int64_t>(4611686018427387904, 2, 62):\n  ");
  multshiftround<int64_t>(4611686018427387904ll,2ll,62);
  std::printf("\n");

  std::printf("multshiftround<int64_t>(-119537721, 77158673929, 62):\n  ");
  multshiftround<int64_t>(-119537721ll,77158673929ll,62);
  std::printf("\n");

  std::printf("multshiftround<uint64_t>(0, 0, 64):\n  ");
  multshiftround<uint64_t>(0ull,0ull,64);
  std::printf("\n");

  std::printf("multshiftround<uint64_t>(9223372036854775808, 2, 63):\n  ");
  multshiftround<uint64_t>(9223372036854775808ull,2ull,63);
  std::printf("\n");

  std::printf("multshiftround_i8(0, 0, 7):\n  ");
  multshiftround_i8(0,0,7);
  std::printf("\n");

  std::printf("multshiftround_i8(64, 2, 6):\n  ");
  multshiftround_i8(64,2,6);
  std::printf("\n");

  std::printf("multshiftround_i8(-3, 43, 6):\n  ");
  multshiftround_i8(-3,43,6);
  std::printf("\n");

  std::printf("multshiftround_u8(0, 0, 8):\n  ");
  multshiftround_u8(0,0,8);
  std::printf("\n");

  std::printf("multshiftround_u8(128, 2, 6):\n  ");
  multshiftround_u8(128,2,6);
  std::printf("\n");

  std::printf("multshiftround_i16(0, 0, 15):\n  ");
  multshiftround_i16(0,0,15);
  std::printf("\n");

  std::printf("multshiftround_i16(16384, 2, 14):\n  ");
  multshiftround_i16(16384,2,14);
  std::printf("\n");

  std::printf("multshiftround_i16(-99, 331, 14):\n  ");
  multshiftround_i16(-99,331,14);
  std::printf("\n");

  std::printf("multshiftround_u16(0, 0, 16):\n  ");
  multshiftround_u16(0,0,16);
  std::printf("\n");

  std::printf("multshiftround_u16(32768, 2, 15):\n  ");
  multshiftround_u16(32768,2,15);
  std::printf("\n");

  std::printf("multshiftround_i32(0, 0, 31):\n  ");
  multshiftround_i32(0,0,31);
  std::printf("\n");

  std::printf("multshiftround_i32(1073741824, 2, 30):\n  ");
  multshiftround_i32(1073741824,2,30);
  std::printf("\n");

  std::printf("multshiftround_i32(-3, 715827883, 30):\n  ");
  multshiftround_i32(-3,715827883,30);
  std::printf("\n");

  std::printf("multshiftround_u32(0, 0, 32):\n  ");
  multshiftround_u32(0u,0u,32);
  std::printf("\n");

  std::printf("multshiftround_u32(2147483648, 2, 31):\n  ");
  multshiftround_u32(2147483648u,2u,31);
  std::printf("\n");

  std::printf("multshiftround_i64(0, 0, 63):\n  ");
  multshiftround_i64(0,0,63);
  std::printf("\n");

  std::printf("multshiftround_i64(4611686018427387904, 2, 62):\n  ");
  multshiftround_i64(4611686018427387904ll,2ll,62);
  std::printf("\n");

  std::printf("multshiftround_i64(-119537721, 77158673929, 62):\n  ");
  multshiftround_i64(-119537721ll,77158673929ll,62);
  std::printf("\n");

  std::printf("multshiftround_u64(0, 0, 64):\n  ");
  multshiftround_u64(0ull,0ull,64);
  std::printf("\n");

  std::printf("multshiftround_u64(9223372036854775808, 2, 63):\n  ");
  multshiftround_u64(9223372036854775808ull,2ull,63);
  std::printf("\n");

  /**
   * The rest of this was generated by a program.
   */
    std::printf("multshiftround<int8_t, 1>(64, 2):\n  ");
  multshiftround<int8_t,1>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 1>(-3, 43):\n  ");
  multshiftround<int8_t,1>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 2>(64, 2):\n  ");
  multshiftround<int8_t,2>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 2>(-3, 43):\n  ");
  multshiftround<int8_t,2>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 3>(64, 2):\n  ");
  multshiftround<int8_t,3>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 3>(-3, 43):\n  ");
  multshiftround<int8_t,3>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 4>(64, 2):\n  ");
  multshiftround<int8_t,4>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 4>(-3, 43):\n  ");
  multshiftround<int8_t,4>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 5>(64, 2):\n  ");
  multshiftround<int8_t,5>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 5>(-3, 43):\n  ");
  multshiftround<int8_t,5>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 6>(64, 2):\n  ");
  multshiftround<int8_t,6>(64,2);
  std::printf("\n");

  std::printf("multshiftround<int8_t, 6>(-3, 43):\n  ");
  multshiftround<int8_t,6>(-3,43);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 1>(128, 2):\n  ");
  multshiftround<uint8_t,1>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 2>(128, 2):\n  ");
  multshiftround<uint8_t,2>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 3>(128, 2):\n  ");
  multshiftround<uint8_t,3>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 4>(128, 2):\n  ");
  multshiftround<uint8_t,4>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 5>(128, 2):\n  ");
  multshiftround<uint8_t,5>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 6>(128, 2):\n  ");
  multshiftround<uint8_t,6>(128,2);
  std::printf("\n");

  std::printf("multshiftround<uint8_t, 7>(128, 2):\n  ");
  multshiftround<uint8_t,7>(128,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 1>(16384, 2):\n  ");
  multshiftround<int16_t,1>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 1>(-99, 331):\n  ");
  multshiftround<int16_t,1>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 2>(16384, 2):\n  ");
  multshiftround<int16_t,2>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 2>(-99, 331):\n  ");
  multshiftround<int16_t,2>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 3>(16384, 2):\n  ");
  multshiftround<int16_t,3>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 3>(-99, 331):\n  ");
  multshiftround<int16_t,3>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 4>(16384, 2):\n  ");
  multshiftround<int16_t,4>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 4>(-99, 331):\n  ");
  multshiftround<int16_t,4>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 5>(16384, 2):\n  ");
  multshiftround<int16_t,5>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 5>(-99, 331):\n  ");
  multshiftround<int16_t,5>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 6>(16384, 2):\n  ");
  multshiftround<int16_t,6>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 6>(-99, 331):\n  ");
  multshiftround<int16_t,6>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 7>(16384, 2):\n  ");
  multshiftround<int16_t,7>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 7>(-99, 331):\n  ");
  multshiftround<int16_t,7>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 8>(16384, 2):\n  ");
  multshiftround<int16_t,8>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 8>(-99, 331):\n  ");
  multshiftround<int16_t,8>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 9>(16384, 2):\n  ");
  multshiftround<int16_t,9>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 9>(-99, 331):\n  ");
  multshiftround<int16_t,9>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 10>(16384, 2):\n  ");
  multshiftround<int16_t,10>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 10>(-99, 331):\n  ");
  multshiftround<int16_t,10>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 11>(16384, 2):\n  ");
  multshiftround<int16_t,11>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 11>(-99, 331):\n  ");
  multshiftround<int16_t,11>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 12>(16384, 2):\n  ");
  multshiftround<int16_t,12>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 12>(-99, 331):\n  ");
  multshiftround<int16_t,12>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 13>(16384, 2):\n  ");
  multshiftround<int16_t,13>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 13>(-99, 331):\n  ");
  multshiftround<int16_t,13>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 14>(16384, 2):\n  ");
  multshiftround<int16_t,14>(16384,2);
  std::printf("\n");

  std::printf("multshiftround<int16_t, 14>(-99, 331):\n  ");
  multshiftround<int16_t,14>(-99,331);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 1>(32768, 2):\n  ");
  multshiftround<uint16_t,1>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 2>(32768, 2):\n  ");
  multshiftround<uint16_t,2>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 3>(32768, 2):\n  ");
  multshiftround<uint16_t,3>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 4>(32768, 2):\n  ");
  multshiftround<uint16_t,4>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 5>(32768, 2):\n  ");
  multshiftround<uint16_t,5>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 6>(32768, 2):\n  ");
  multshiftround<uint16_t,6>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 7>(32768, 2):\n  ");
  multshiftround<uint16_t,7>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 8>(32768, 2):\n  ");
  multshiftround<uint16_t,8>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 9>(32768, 2):\n  ");
  multshiftround<uint16_t,9>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 10>(32768, 2):\n  ");
  multshiftround<uint16_t,10>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 11>(32768, 2):\n  ");
  multshiftround<uint16_t,11>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 12>(32768, 2):\n  ");
  multshiftround<uint16_t,12>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 13>(32768, 2):\n  ");
  multshiftround<uint16_t,13>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 14>(32768, 2):\n  ");
  multshiftround<uint16_t,14>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<uint16_t, 15>(32768, 2):\n  ");
  multshiftround<uint16_t,15>(32768,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 1>(1073741824, 2):\n  ");
  multshiftround<int32_t,1>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 1>(-3, 715827883):\n  ");
  multshiftround<int32_t,1>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 2>(1073741824, 2):\n  ");
  multshiftround<int32_t,2>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 2>(-3, 715827883):\n  ");
  multshiftround<int32_t,2>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 3>(1073741824, 2):\n  ");
  multshiftround<int32_t,3>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 3>(-3, 715827883):\n  ");
  multshiftround<int32_t,3>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 4>(1073741824, 2):\n  ");
  multshiftround<int32_t,4>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 4>(-3, 715827883):\n  ");
  multshiftround<int32_t,4>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 5>(1073741824, 2):\n  ");
  multshiftround<int32_t,5>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 5>(-3, 715827883):\n  ");
  multshiftround<int32_t,5>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 6>(1073741824, 2):\n  ");
  multshiftround<int32_t,6>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 6>(-3, 715827883):\n  ");
  multshiftround<int32_t,6>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 7>(1073741824, 2):\n  ");
  multshiftround<int32_t,7>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 7>(-3, 715827883):\n  ");
  multshiftround<int32_t,7>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 8>(1073741824, 2):\n  ");
  multshiftround<int32_t,8>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 8>(-3, 715827883):\n  ");
  multshiftround<int32_t,8>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 9>(1073741824, 2):\n  ");
  multshiftround<int32_t,9>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 9>(-3, 715827883):\n  ");
  multshiftround<int32_t,9>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 10>(1073741824, 2):\n  ");
  multshiftround<int32_t,10>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 10>(-3, 715827883):\n  ");
  multshiftround<int32_t,10>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 11>(1073741824, 2):\n  ");
  multshiftround<int32_t,11>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 11>(-3, 715827883):\n  ");
  multshiftround<int32_t,11>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 12>(1073741824, 2):\n  ");
  multshiftround<int32_t,12>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 12>(-3, 715827883):\n  ");
  multshiftround<int32_t,12>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 13>(1073741824, 2):\n  ");
  multshiftround<int32_t,13>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 13>(-3, 715827883):\n  ");
  multshiftround<int32_t,13>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 14>(1073741824, 2):\n  ");
  multshiftround<int32_t,14>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 14>(-3, 715827883):\n  ");
  multshiftround<int32_t,14>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 15>(1073741824, 2):\n  ");
  multshiftround<int32_t,15>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 15>(-3, 715827883):\n  ");
  multshiftround<int32_t,15>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 16>(1073741824, 2):\n  ");
  multshiftround<int32_t,16>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 16>(-3, 715827883):\n  ");
  multshiftround<int32_t,16>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 17>(1073741824, 2):\n  ");
  multshiftround<int32_t,17>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 17>(-3, 715827883):\n  ");
  multshiftround<int32_t,17>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 18>(1073741824, 2):\n  ");
  multshiftround<int32_t,18>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 18>(-3, 715827883):\n  ");
  multshiftround<int32_t,18>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 19>(1073741824, 2):\n  ");
  multshiftround<int32_t,19>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 19>(-3, 715827883):\n  ");
  multshiftround<int32_t,19>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 20>(1073741824, 2):\n  ");
  multshiftround<int32_t,20>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 20>(-3, 715827883):\n  ");
  multshiftround<int32_t,20>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 21>(1073741824, 2):\n  ");
  multshiftround<int32_t,21>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 21>(-3, 715827883):\n  ");
  multshiftround<int32_t,21>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 22>(1073741824, 2):\n  ");
  multshiftround<int32_t,22>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 22>(-3, 715827883):\n  ");
  multshiftround<int32_t,22>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 23>(1073741824, 2):\n  ");
  multshiftround<int32_t,23>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 23>(-3, 715827883):\n  ");
  multshiftround<int32_t,23>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 24>(1073741824, 2):\n  ");
  multshiftround<int32_t,24>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 24>(-3, 715827883):\n  ");
  multshiftround<int32_t,24>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 25>(1073741824, 2):\n  ");
  multshiftround<int32_t,25>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 25>(-3, 715827883):\n  ");
  multshiftround<int32_t,25>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 26>(1073741824, 2):\n  ");
  multshiftround<int32_t,26>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 26>(-3, 715827883):\n  ");
  multshiftround<int32_t,26>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 27>(1073741824, 2):\n  ");
  multshiftround<int32_t,27>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 27>(-3, 715827883):\n  ");
  multshiftround<int32_t,27>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 28>(1073741824, 2):\n  ");
  multshiftround<int32_t,28>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 28>(-3, 715827883):\n  ");
  multshiftround<int32_t,28>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 29>(1073741824, 2):\n  ");
  multshiftround<int32_t,29>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 29>(-3, 715827883):\n  ");
  multshiftround<int32_t,29>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 30>(1073741824, 2):\n  ");
  multshiftround<int32_t,30>(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround<int32_t, 30>(-3, 715827883):\n  ");
  multshiftround<int32_t,30>(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 1>(2147483648, 2):\n  ");
  multshiftround<uint32_t,1>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 2>(2147483648, 2):\n  ");
  multshiftround<uint32_t,2>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 3>(2147483648, 2):\n  ");
  multshiftround<uint32_t,3>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 4>(2147483648, 2):\n  ");
  multshiftround<uint32_t,4>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 5>(2147483648, 2):\n  ");
  multshiftround<uint32_t,5>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 6>(2147483648, 2):\n  ");
  multshiftround<uint32_t,6>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 7>(2147483648, 2):\n  ");
  multshiftround<uint32_t,7>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 8>(2147483648, 2):\n  ");
  multshiftround<uint32_t,8>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 9>(2147483648, 2):\n  ");
  multshiftround<uint32_t,9>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 10>(2147483648, 2):\n  ");
  multshiftround<uint32_t,10>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 11>(2147483648, 2):\n  ");
  multshiftround<uint32_t,11>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 12>(2147483648, 2):\n  ");
  multshiftround<uint32_t,12>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 13>(2147483648, 2):\n  ");
  multshiftround<uint32_t,13>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 14>(2147483648, 2):\n  ");
  multshiftround<uint32_t,14>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 15>(2147483648, 2):\n  ");
  multshiftround<uint32_t,15>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 16>(2147483648, 2):\n  ");
  multshiftround<uint32_t,16>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 17>(2147483648, 2):\n  ");
  multshiftround<uint32_t,17>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 18>(2147483648, 2):\n  ");
  multshiftround<uint32_t,18>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 19>(2147483648, 2):\n  ");
  multshiftround<uint32_t,19>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 20>(2147483648, 2):\n  ");
  multshiftround<uint32_t,20>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 21>(2147483648, 2):\n  ");
  multshiftround<uint32_t,21>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 22>(2147483648, 2):\n  ");
  multshiftround<uint32_t,22>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 23>(2147483648, 2):\n  ");
  multshiftround<uint32_t,23>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 24>(2147483648, 2):\n  ");
  multshiftround<uint32_t,24>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 25>(2147483648, 2):\n  ");
  multshiftround<uint32_t,25>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 26>(2147483648, 2):\n  ");
  multshiftround<uint32_t,26>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 27>(2147483648, 2):\n  ");
  multshiftround<uint32_t,27>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 28>(2147483648, 2):\n  ");
  multshiftround<uint32_t,28>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 29>(2147483648, 2):\n  ");
  multshiftround<uint32_t,29>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 30>(2147483648, 2):\n  ");
  multshiftround<uint32_t,30>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<uint32_t, 31>(2147483648, 2):\n  ");
  multshiftround<uint32_t,31>(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 1>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,1>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 1>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,1>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 2>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,2>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 2>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,2>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 3>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,3>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 3>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,3>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 4>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,4>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 4>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,4>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 5>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,5>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 5>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,5>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 6>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,6>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 6>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,6>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 7>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,7>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 7>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,7>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 8>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,8>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 8>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,8>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 9>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,9>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 9>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,9>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 10>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,10>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 10>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,10>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 11>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,11>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 11>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,11>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 12>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,12>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 12>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,12>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 13>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,13>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 13>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,13>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 14>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,14>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 14>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,14>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 15>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,15>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 15>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,15>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 16>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,16>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 16>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,16>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 17>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,17>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 17>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,17>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 18>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,18>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 18>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,18>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 19>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,19>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 19>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,19>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 20>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,20>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 20>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,20>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 21>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,21>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 21>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,21>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 22>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,22>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 22>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,22>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 23>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,23>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 23>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,23>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 24>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,24>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 24>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,24>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 25>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,25>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 25>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,25>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 26>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,26>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 26>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,26>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 27>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,27>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 27>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,27>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 28>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,28>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 28>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,28>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 29>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,29>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 29>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,29>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 30>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,30>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 30>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,30>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 31>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,31>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 31>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,31>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 32>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,32>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 32>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,32>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 33>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,33>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 33>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,33>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 34>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,34>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 34>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,34>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 35>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,35>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 35>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,35>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 36>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,36>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 36>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,36>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 37>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,37>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 37>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,37>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 38>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,38>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 38>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,38>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 39>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,39>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 39>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,39>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 40>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,40>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 40>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,40>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 41>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,41>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 41>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,41>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 42>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,42>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 42>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,42>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 43>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,43>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 43>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,43>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 44>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,44>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 44>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,44>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 45>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,45>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 45>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,45>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 46>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,46>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 46>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,46>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 47>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,47>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 47>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,47>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 48>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,48>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 48>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,48>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 49>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,49>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 49>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,49>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 50>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,50>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 50>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,50>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 51>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,51>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 51>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,51>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 52>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,52>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 52>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,52>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 53>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,53>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 53>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,53>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 54>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,54>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 54>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,54>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 55>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,55>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 55>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,55>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 56>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,56>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 56>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,56>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 57>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,57>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 57>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,57>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 58>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,58>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 58>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,58>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 59>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,59>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 59>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,59>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 60>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,60>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 60>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,60>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 61>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,61>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 61>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,61>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 62>(4611686018427387904, 2):\n  ");
  multshiftround<int64_t,62>(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround<int64_t, 62>(-119537721, 77158673929):\n  ");
  multshiftround<int64_t,62>(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 1>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,1>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 2>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,2>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 3>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,3>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 4>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,4>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 5>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,5>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 6>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,6>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 7>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,7>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 8>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,8>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 9>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,9>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 10>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,10>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 11>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,11>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 12>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,12>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 13>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,13>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 14>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,14>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 15>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,15>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 16>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,16>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 17>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,17>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 18>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,18>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 19>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,19>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 20>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,20>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 21>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,21>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 22>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,22>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 23>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,23>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 24>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,24>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 25>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,25>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 26>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,26>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 27>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,27>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 28>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,28>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 29>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,29>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 30>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,30>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 31>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,31>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 32>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,32>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 33>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,33>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 34>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,34>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 35>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,35>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 36>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,36>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 37>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,37>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 38>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,38>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 39>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,39>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 40>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,40>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 41>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,41>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 42>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,42>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 43>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,43>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 44>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,44>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 45>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,45>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 46>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,46>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 47>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,47>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 48>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,48>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 49>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,49>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 50>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,50>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 51>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,51>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 52>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,52>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 53>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,53>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 54>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,54>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 55>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,55>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 56>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,56>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 57>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,57>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 58>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,58>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 59>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,59>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 60>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,60>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 61>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,61>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 62>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,62>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround<uint64_t, 63>(9223372036854775808, 2):\n  ");
  multshiftround<uint64_t,63>(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_i8_1(64, 2):\n  ");
  multshiftround_i8_1(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_1(-3, 43):\n  ");
  multshiftround_i8_1(-3,43);
  std::printf("\n");

  std::printf("multshiftround_i8_2(64, 2):\n  ");
  multshiftround_i8_2(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_2(-3, 43):\n  ");
  multshiftround_i8_2(-3,43);
  std::printf("\n");

  std::printf("multshiftround_i8_3(64, 2):\n  ");
  multshiftround_i8_3(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_3(-3, 43):\n  ");
  multshiftround_i8_3(-3,43);
  std::printf("\n");

  std::printf("multshiftround_i8_4(64, 2):\n  ");
  multshiftround_i8_4(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_4(-3, 43):\n  ");
  multshiftround_i8_4(-3,43);
  std::printf("\n");

  std::printf("multshiftround_i8_5(64, 2):\n  ");
  multshiftround_i8_5(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_5(-3, 43):\n  ");
  multshiftround_i8_5(-3,43);
  std::printf("\n");

  std::printf("multshiftround_i8_6(64, 2):\n  ");
  multshiftround_i8_6(64,2);
  std::printf("\n");

  std::printf("multshiftround_i8_6(-3, 43):\n  ");
  multshiftround_i8_6(-3,43);
  std::printf("\n");

  std::printf("multshiftround_u8_1(128, 2):\n  ");
  multshiftround_u8_1(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_2(128, 2):\n  ");
  multshiftround_u8_2(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_3(128, 2):\n  ");
  multshiftround_u8_3(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_4(128, 2):\n  ");
  multshiftround_u8_4(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_5(128, 2):\n  ");
  multshiftround_u8_5(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_6(128, 2):\n  ");
  multshiftround_u8_6(128,2);
  std::printf("\n");

  std::printf("multshiftround_u8_7(128, 2):\n  ");
  multshiftround_u8_7(128,2);
  std::printf("\n");

  std::printf("multshiftround_i16_1(16384, 2):\n  ");
  multshiftround_i16_1(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_1(-99, 331):\n  ");
  multshiftround_i16_1(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_2(16384, 2):\n  ");
  multshiftround_i16_2(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_2(-99, 331):\n  ");
  multshiftround_i16_2(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_3(16384, 2):\n  ");
  multshiftround_i16_3(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_3(-99, 331):\n  ");
  multshiftround_i16_3(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_4(16384, 2):\n  ");
  multshiftround_i16_4(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_4(-99, 331):\n  ");
  multshiftround_i16_4(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_5(16384, 2):\n  ");
  multshiftround_i16_5(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_5(-99, 331):\n  ");
  multshiftround_i16_5(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_6(16384, 2):\n  ");
  multshiftround_i16_6(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_6(-99, 331):\n  ");
  multshiftround_i16_6(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_7(16384, 2):\n  ");
  multshiftround_i16_7(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_7(-99, 331):\n  ");
  multshiftround_i16_7(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_8(16384, 2):\n  ");
  multshiftround_i16_8(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_8(-99, 331):\n  ");
  multshiftround_i16_8(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_9(16384, 2):\n  ");
  multshiftround_i16_9(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_9(-99, 331):\n  ");
  multshiftround_i16_9(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_10(16384, 2):\n  ");
  multshiftround_i16_10(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_10(-99, 331):\n  ");
  multshiftround_i16_10(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_11(16384, 2):\n  ");
  multshiftround_i16_11(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_11(-99, 331):\n  ");
  multshiftround_i16_11(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_12(16384, 2):\n  ");
  multshiftround_i16_12(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_12(-99, 331):\n  ");
  multshiftround_i16_12(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_13(16384, 2):\n  ");
  multshiftround_i16_13(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_13(-99, 331):\n  ");
  multshiftround_i16_13(-99,331);
  std::printf("\n");

  std::printf("multshiftround_i16_14(16384, 2):\n  ");
  multshiftround_i16_14(16384,2);
  std::printf("\n");

  std::printf("multshiftround_i16_14(-99, 331):\n  ");
  multshiftround_i16_14(-99,331);
  std::printf("\n");

  std::printf("multshiftround_u16_1(32768, 2):\n  ");
  multshiftround_u16_1(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_2(32768, 2):\n  ");
  multshiftround_u16_2(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_3(32768, 2):\n  ");
  multshiftround_u16_3(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_4(32768, 2):\n  ");
  multshiftround_u16_4(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_5(32768, 2):\n  ");
  multshiftround_u16_5(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_6(32768, 2):\n  ");
  multshiftround_u16_6(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_7(32768, 2):\n  ");
  multshiftround_u16_7(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_8(32768, 2):\n  ");
  multshiftround_u16_8(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_9(32768, 2):\n  ");
  multshiftround_u16_9(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_10(32768, 2):\n  ");
  multshiftround_u16_10(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_11(32768, 2):\n  ");
  multshiftround_u16_11(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_12(32768, 2):\n  ");
  multshiftround_u16_12(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_13(32768, 2):\n  ");
  multshiftround_u16_13(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_14(32768, 2):\n  ");
  multshiftround_u16_14(32768,2);
  std::printf("\n");

  std::printf("multshiftround_u16_15(32768, 2):\n  ");
  multshiftround_u16_15(32768,2);
  std::printf("\n");

  std::printf("multshiftround_i32_1(1073741824, 2):\n  ");
  multshiftround_i32_1(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_1(-3, 715827883):\n  ");
  multshiftround_i32_1(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_2(1073741824, 2):\n  ");
  multshiftround_i32_2(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_2(-3, 715827883):\n  ");
  multshiftround_i32_2(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_3(1073741824, 2):\n  ");
  multshiftround_i32_3(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_3(-3, 715827883):\n  ");
  multshiftround_i32_3(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_4(1073741824, 2):\n  ");
  multshiftround_i32_4(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_4(-3, 715827883):\n  ");
  multshiftround_i32_4(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_5(1073741824, 2):\n  ");
  multshiftround_i32_5(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_5(-3, 715827883):\n  ");
  multshiftround_i32_5(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_6(1073741824, 2):\n  ");
  multshiftround_i32_6(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_6(-3, 715827883):\n  ");
  multshiftround_i32_6(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_7(1073741824, 2):\n  ");
  multshiftround_i32_7(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_7(-3, 715827883):\n  ");
  multshiftround_i32_7(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_8(1073741824, 2):\n  ");
  multshiftround_i32_8(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_8(-3, 715827883):\n  ");
  multshiftround_i32_8(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_9(1073741824, 2):\n  ");
  multshiftround_i32_9(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_9(-3, 715827883):\n  ");
  multshiftround_i32_9(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_10(1073741824, 2):\n  ");
  multshiftround_i32_10(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_10(-3, 715827883):\n  ");
  multshiftround_i32_10(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_11(1073741824, 2):\n  ");
  multshiftround_i32_11(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_11(-3, 715827883):\n  ");
  multshiftround_i32_11(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_12(1073741824, 2):\n  ");
  multshiftround_i32_12(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_12(-3, 715827883):\n  ");
  multshiftround_i32_12(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_13(1073741824, 2):\n  ");
  multshiftround_i32_13(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_13(-3, 715827883):\n  ");
  multshiftround_i32_13(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_14(1073741824, 2):\n  ");
  multshiftround_i32_14(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_14(-3, 715827883):\n  ");
  multshiftround_i32_14(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_15(1073741824, 2):\n  ");
  multshiftround_i32_15(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_15(-3, 715827883):\n  ");
  multshiftround_i32_15(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_16(1073741824, 2):\n  ");
  multshiftround_i32_16(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_16(-3, 715827883):\n  ");
  multshiftround_i32_16(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_17(1073741824, 2):\n  ");
  multshiftround_i32_17(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_17(-3, 715827883):\n  ");
  multshiftround_i32_17(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_18(1073741824, 2):\n  ");
  multshiftround_i32_18(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_18(-3, 715827883):\n  ");
  multshiftround_i32_18(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_19(1073741824, 2):\n  ");
  multshiftround_i32_19(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_19(-3, 715827883):\n  ");
  multshiftround_i32_19(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_20(1073741824, 2):\n  ");
  multshiftround_i32_20(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_20(-3, 715827883):\n  ");
  multshiftround_i32_20(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_21(1073741824, 2):\n  ");
  multshiftround_i32_21(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_21(-3, 715827883):\n  ");
  multshiftround_i32_21(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_22(1073741824, 2):\n  ");
  multshiftround_i32_22(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_22(-3, 715827883):\n  ");
  multshiftround_i32_22(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_23(1073741824, 2):\n  ");
  multshiftround_i32_23(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_23(-3, 715827883):\n  ");
  multshiftround_i32_23(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_24(1073741824, 2):\n  ");
  multshiftround_i32_24(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_24(-3, 715827883):\n  ");
  multshiftround_i32_24(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_25(1073741824, 2):\n  ");
  multshiftround_i32_25(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_25(-3, 715827883):\n  ");
  multshiftround_i32_25(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_26(1073741824, 2):\n  ");
  multshiftround_i32_26(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_26(-3, 715827883):\n  ");
  multshiftround_i32_26(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_27(1073741824, 2):\n  ");
  multshiftround_i32_27(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_27(-3, 715827883):\n  ");
  multshiftround_i32_27(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_28(1073741824, 2):\n  ");
  multshiftround_i32_28(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_28(-3, 715827883):\n  ");
  multshiftround_i32_28(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_29(1073741824, 2):\n  ");
  multshiftround_i32_29(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_29(-3, 715827883):\n  ");
  multshiftround_i32_29(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_i32_30(1073741824, 2):\n  ");
  multshiftround_i32_30(1073741824,2);
  std::printf("\n");

  std::printf("multshiftround_i32_30(-3, 715827883):\n  ");
  multshiftround_i32_30(-3,715827883);
  std::printf("\n");

  std::printf("multshiftround_u32_1(2147483648, 2):\n  ");
  multshiftround_u32_1(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_2(2147483648, 2):\n  ");
  multshiftround_u32_2(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_3(2147483648, 2):\n  ");
  multshiftround_u32_3(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_4(2147483648, 2):\n  ");
  multshiftround_u32_4(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_5(2147483648, 2):\n  ");
  multshiftround_u32_5(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_6(2147483648, 2):\n  ");
  multshiftround_u32_6(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_7(2147483648, 2):\n  ");
  multshiftround_u32_7(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_8(2147483648, 2):\n  ");
  multshiftround_u32_8(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_9(2147483648, 2):\n  ");
  multshiftround_u32_9(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_10(2147483648, 2):\n  ");
  multshiftround_u32_10(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_11(2147483648, 2):\n  ");
  multshiftround_u32_11(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_12(2147483648, 2):\n  ");
  multshiftround_u32_12(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_13(2147483648, 2):\n  ");
  multshiftround_u32_13(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_14(2147483648, 2):\n  ");
  multshiftround_u32_14(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_15(2147483648, 2):\n  ");
  multshiftround_u32_15(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_16(2147483648, 2):\n  ");
  multshiftround_u32_16(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_17(2147483648, 2):\n  ");
  multshiftround_u32_17(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_18(2147483648, 2):\n  ");
  multshiftround_u32_18(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_19(2147483648, 2):\n  ");
  multshiftround_u32_19(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_20(2147483648, 2):\n  ");
  multshiftround_u32_20(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_21(2147483648, 2):\n  ");
  multshiftround_u32_21(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_22(2147483648, 2):\n  ");
  multshiftround_u32_22(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_23(2147483648, 2):\n  ");
  multshiftround_u32_23(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_24(2147483648, 2):\n  ");
  multshiftround_u32_24(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_25(2147483648, 2):\n  ");
  multshiftround_u32_25(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_26(2147483648, 2):\n  ");
  multshiftround_u32_26(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_27(2147483648, 2):\n  ");
  multshiftround_u32_27(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_28(2147483648, 2):\n  ");
  multshiftround_u32_28(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_29(2147483648, 2):\n  ");
  multshiftround_u32_29(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_30(2147483648, 2):\n  ");
  multshiftround_u32_30(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_u32_31(2147483648, 2):\n  ");
  multshiftround_u32_31(2147483648u,2u);
  std::printf("\n");

  std::printf("multshiftround_i64_1(4611686018427387904, 2):\n  ");
  multshiftround_i64_1(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_1(-119537721, 77158673929):\n  ");
  multshiftround_i64_1(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_2(4611686018427387904, 2):\n  ");
  multshiftround_i64_2(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_2(-119537721, 77158673929):\n  ");
  multshiftround_i64_2(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_3(4611686018427387904, 2):\n  ");
  multshiftround_i64_3(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_3(-119537721, 77158673929):\n  ");
  multshiftround_i64_3(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_4(4611686018427387904, 2):\n  ");
  multshiftround_i64_4(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_4(-119537721, 77158673929):\n  ");
  multshiftround_i64_4(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_5(4611686018427387904, 2):\n  ");
  multshiftround_i64_5(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_5(-119537721, 77158673929):\n  ");
  multshiftround_i64_5(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_6(4611686018427387904, 2):\n  ");
  multshiftround_i64_6(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_6(-119537721, 77158673929):\n  ");
  multshiftround_i64_6(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_7(4611686018427387904, 2):\n  ");
  multshiftround_i64_7(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_7(-119537721, 77158673929):\n  ");
  multshiftround_i64_7(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_8(4611686018427387904, 2):\n  ");
  multshiftround_i64_8(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_8(-119537721, 77158673929):\n  ");
  multshiftround_i64_8(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_9(4611686018427387904, 2):\n  ");
  multshiftround_i64_9(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_9(-119537721, 77158673929):\n  ");
  multshiftround_i64_9(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_10(4611686018427387904, 2):\n  ");
  multshiftround_i64_10(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_10(-119537721, 77158673929):\n  ");
  multshiftround_i64_10(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_11(4611686018427387904, 2):\n  ");
  multshiftround_i64_11(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_11(-119537721, 77158673929):\n  ");
  multshiftround_i64_11(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_12(4611686018427387904, 2):\n  ");
  multshiftround_i64_12(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_12(-119537721, 77158673929):\n  ");
  multshiftround_i64_12(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_13(4611686018427387904, 2):\n  ");
  multshiftround_i64_13(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_13(-119537721, 77158673929):\n  ");
  multshiftround_i64_13(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_14(4611686018427387904, 2):\n  ");
  multshiftround_i64_14(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_14(-119537721, 77158673929):\n  ");
  multshiftround_i64_14(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_15(4611686018427387904, 2):\n  ");
  multshiftround_i64_15(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_15(-119537721, 77158673929):\n  ");
  multshiftround_i64_15(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_16(4611686018427387904, 2):\n  ");
  multshiftround_i64_16(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_16(-119537721, 77158673929):\n  ");
  multshiftround_i64_16(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_17(4611686018427387904, 2):\n  ");
  multshiftround_i64_17(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_17(-119537721, 77158673929):\n  ");
  multshiftround_i64_17(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_18(4611686018427387904, 2):\n  ");
  multshiftround_i64_18(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_18(-119537721, 77158673929):\n  ");
  multshiftround_i64_18(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_19(4611686018427387904, 2):\n  ");
  multshiftround_i64_19(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_19(-119537721, 77158673929):\n  ");
  multshiftround_i64_19(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_20(4611686018427387904, 2):\n  ");
  multshiftround_i64_20(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_20(-119537721, 77158673929):\n  ");
  multshiftround_i64_20(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_21(4611686018427387904, 2):\n  ");
  multshiftround_i64_21(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_21(-119537721, 77158673929):\n  ");
  multshiftround_i64_21(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_22(4611686018427387904, 2):\n  ");
  multshiftround_i64_22(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_22(-119537721, 77158673929):\n  ");
  multshiftround_i64_22(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_23(4611686018427387904, 2):\n  ");
  multshiftround_i64_23(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_23(-119537721, 77158673929):\n  ");
  multshiftround_i64_23(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_24(4611686018427387904, 2):\n  ");
  multshiftround_i64_24(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_24(-119537721, 77158673929):\n  ");
  multshiftround_i64_24(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_25(4611686018427387904, 2):\n  ");
  multshiftround_i64_25(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_25(-119537721, 77158673929):\n  ");
  multshiftround_i64_25(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_26(4611686018427387904, 2):\n  ");
  multshiftround_i64_26(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_26(-119537721, 77158673929):\n  ");
  multshiftround_i64_26(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_27(4611686018427387904, 2):\n  ");
  multshiftround_i64_27(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_27(-119537721, 77158673929):\n  ");
  multshiftround_i64_27(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_28(4611686018427387904, 2):\n  ");
  multshiftround_i64_28(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_28(-119537721, 77158673929):\n  ");
  multshiftround_i64_28(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_29(4611686018427387904, 2):\n  ");
  multshiftround_i64_29(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_29(-119537721, 77158673929):\n  ");
  multshiftround_i64_29(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_30(4611686018427387904, 2):\n  ");
  multshiftround_i64_30(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_30(-119537721, 77158673929):\n  ");
  multshiftround_i64_30(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_31(4611686018427387904, 2):\n  ");
  multshiftround_i64_31(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_31(-119537721, 77158673929):\n  ");
  multshiftround_i64_31(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_32(4611686018427387904, 2):\n  ");
  multshiftround_i64_32(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_32(-119537721, 77158673929):\n  ");
  multshiftround_i64_32(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_33(4611686018427387904, 2):\n  ");
  multshiftround_i64_33(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_33(-119537721, 77158673929):\n  ");
  multshiftround_i64_33(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_34(4611686018427387904, 2):\n  ");
  multshiftround_i64_34(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_34(-119537721, 77158673929):\n  ");
  multshiftround_i64_34(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_35(4611686018427387904, 2):\n  ");
  multshiftround_i64_35(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_35(-119537721, 77158673929):\n  ");
  multshiftround_i64_35(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_36(4611686018427387904, 2):\n  ");
  multshiftround_i64_36(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_36(-119537721, 77158673929):\n  ");
  multshiftround_i64_36(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_37(4611686018427387904, 2):\n  ");
  multshiftround_i64_37(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_37(-119537721, 77158673929):\n  ");
  multshiftround_i64_37(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_38(4611686018427387904, 2):\n  ");
  multshiftround_i64_38(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_38(-119537721, 77158673929):\n  ");
  multshiftround_i64_38(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_39(4611686018427387904, 2):\n  ");
  multshiftround_i64_39(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_39(-119537721, 77158673929):\n  ");
  multshiftround_i64_39(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_40(4611686018427387904, 2):\n  ");
  multshiftround_i64_40(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_40(-119537721, 77158673929):\n  ");
  multshiftround_i64_40(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_41(4611686018427387904, 2):\n  ");
  multshiftround_i64_41(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_41(-119537721, 77158673929):\n  ");
  multshiftround_i64_41(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_42(4611686018427387904, 2):\n  ");
  multshiftround_i64_42(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_42(-119537721, 77158673929):\n  ");
  multshiftround_i64_42(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_43(4611686018427387904, 2):\n  ");
  multshiftround_i64_43(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_43(-119537721, 77158673929):\n  ");
  multshiftround_i64_43(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_44(4611686018427387904, 2):\n  ");
  multshiftround_i64_44(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_44(-119537721, 77158673929):\n  ");
  multshiftround_i64_44(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_45(4611686018427387904, 2):\n  ");
  multshiftround_i64_45(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_45(-119537721, 77158673929):\n  ");
  multshiftround_i64_45(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_46(4611686018427387904, 2):\n  ");
  multshiftround_i64_46(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_46(-119537721, 77158673929):\n  ");
  multshiftround_i64_46(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_47(4611686018427387904, 2):\n  ");
  multshiftround_i64_47(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_47(-119537721, 77158673929):\n  ");
  multshiftround_i64_47(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_48(4611686018427387904, 2):\n  ");
  multshiftround_i64_48(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_48(-119537721, 77158673929):\n  ");
  multshiftround_i64_48(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_49(4611686018427387904, 2):\n  ");
  multshiftround_i64_49(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_49(-119537721, 77158673929):\n  ");
  multshiftround_i64_49(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_50(4611686018427387904, 2):\n  ");
  multshiftround_i64_50(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_50(-119537721, 77158673929):\n  ");
  multshiftround_i64_50(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_51(4611686018427387904, 2):\n  ");
  multshiftround_i64_51(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_51(-119537721, 77158673929):\n  ");
  multshiftround_i64_51(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_52(4611686018427387904, 2):\n  ");
  multshiftround_i64_52(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_52(-119537721, 77158673929):\n  ");
  multshiftround_i64_52(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_53(4611686018427387904, 2):\n  ");
  multshiftround_i64_53(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_53(-119537721, 77158673929):\n  ");
  multshiftround_i64_53(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_54(4611686018427387904, 2):\n  ");
  multshiftround_i64_54(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_54(-119537721, 77158673929):\n  ");
  multshiftround_i64_54(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_55(4611686018427387904, 2):\n  ");
  multshiftround_i64_55(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_55(-119537721, 77158673929):\n  ");
  multshiftround_i64_55(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_56(4611686018427387904, 2):\n  ");
  multshiftround_i64_56(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_56(-119537721, 77158673929):\n  ");
  multshiftround_i64_56(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_57(4611686018427387904, 2):\n  ");
  multshiftround_i64_57(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_57(-119537721, 77158673929):\n  ");
  multshiftround_i64_57(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_58(4611686018427387904, 2):\n  ");
  multshiftround_i64_58(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_58(-119537721, 77158673929):\n  ");
  multshiftround_i64_58(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_59(4611686018427387904, 2):\n  ");
  multshiftround_i64_59(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_59(-119537721, 77158673929):\n  ");
  multshiftround_i64_59(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_60(4611686018427387904, 2):\n  ");
  multshiftround_i64_60(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_60(-119537721, 77158673929):\n  ");
  multshiftround_i64_60(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_61(4611686018427387904, 2):\n  ");
  multshiftround_i64_61(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_61(-119537721, 77158673929):\n  ");
  multshiftround_i64_61(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_i64_62(4611686018427387904, 2):\n  ");
  multshiftround_i64_62(4611686018427387904ll,2ll);
  std::printf("\n");

  std::printf("multshiftround_i64_62(-119537721, 77158673929):\n  ");
  multshiftround_i64_62(-119537721ll,77158673929ll);
  std::printf("\n");

  std::printf("multshiftround_u64_1(9223372036854775808, 2):\n  ");
  multshiftround_u64_1(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_2(9223372036854775808, 2):\n  ");
  multshiftround_u64_2(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_3(9223372036854775808, 2):\n  ");
  multshiftround_u64_3(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_4(9223372036854775808, 2):\n  ");
  multshiftround_u64_4(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_5(9223372036854775808, 2):\n  ");
  multshiftround_u64_5(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_6(9223372036854775808, 2):\n  ");
  multshiftround_u64_6(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_7(9223372036854775808, 2):\n  ");
  multshiftround_u64_7(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_8(9223372036854775808, 2):\n  ");
  multshiftround_u64_8(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_9(9223372036854775808, 2):\n  ");
  multshiftround_u64_9(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_10(9223372036854775808, 2):\n  ");
  multshiftround_u64_10(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_11(9223372036854775808, 2):\n  ");
  multshiftround_u64_11(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_12(9223372036854775808, 2):\n  ");
  multshiftround_u64_12(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_13(9223372036854775808, 2):\n  ");
  multshiftround_u64_13(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_14(9223372036854775808, 2):\n  ");
  multshiftround_u64_14(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_15(9223372036854775808, 2):\n  ");
  multshiftround_u64_15(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_16(9223372036854775808, 2):\n  ");
  multshiftround_u64_16(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_17(9223372036854775808, 2):\n  ");
  multshiftround_u64_17(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_18(9223372036854775808, 2):\n  ");
  multshiftround_u64_18(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_19(9223372036854775808, 2):\n  ");
  multshiftround_u64_19(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_20(9223372036854775808, 2):\n  ");
  multshiftround_u64_20(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_21(9223372036854775808, 2):\n  ");
  multshiftround_u64_21(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_22(9223372036854775808, 2):\n  ");
  multshiftround_u64_22(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_23(9223372036854775808, 2):\n  ");
  multshiftround_u64_23(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_24(9223372036854775808, 2):\n  ");
  multshiftround_u64_24(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_25(9223372036854775808, 2):\n  ");
  multshiftround_u64_25(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_26(9223372036854775808, 2):\n  ");
  multshiftround_u64_26(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_27(9223372036854775808, 2):\n  ");
  multshiftround_u64_27(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_28(9223372036854775808, 2):\n  ");
  multshiftround_u64_28(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_29(9223372036854775808, 2):\n  ");
  multshiftround_u64_29(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_30(9223372036854775808, 2):\n  ");
  multshiftround_u64_30(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_31(9223372036854775808, 2):\n  ");
  multshiftround_u64_31(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_32(9223372036854775808, 2):\n  ");
  multshiftround_u64_32(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_33(9223372036854775808, 2):\n  ");
  multshiftround_u64_33(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_34(9223372036854775808, 2):\n  ");
  multshiftround_u64_34(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_35(9223372036854775808, 2):\n  ");
  multshiftround_u64_35(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_36(9223372036854775808, 2):\n  ");
  multshiftround_u64_36(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_37(9223372036854775808, 2):\n  ");
  multshiftround_u64_37(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_38(9223372036854775808, 2):\n  ");
  multshiftround_u64_38(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_39(9223372036854775808, 2):\n  ");
  multshiftround_u64_39(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_40(9223372036854775808, 2):\n  ");
  multshiftround_u64_40(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_41(9223372036854775808, 2):\n  ");
  multshiftround_u64_41(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_42(9223372036854775808, 2):\n  ");
  multshiftround_u64_42(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_43(9223372036854775808, 2):\n  ");
  multshiftround_u64_43(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_44(9223372036854775808, 2):\n  ");
  multshiftround_u64_44(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_45(9223372036854775808, 2):\n  ");
  multshiftround_u64_45(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_46(9223372036854775808, 2):\n  ");
  multshiftround_u64_46(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_47(9223372036854775808, 2):\n  ");
  multshiftround_u64_47(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_48(9223372036854775808, 2):\n  ");
  multshiftround_u64_48(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_49(9223372036854775808, 2):\n  ");
  multshiftround_u64_49(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_50(9223372036854775808, 2):\n  ");
  multshiftround_u64_50(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_51(9223372036854775808, 2):\n  ");
  multshiftround_u64_51(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_52(9223372036854775808, 2):\n  ");
  multshiftround_u64_52(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_53(9223372036854775808, 2):\n  ");
  multshiftround_u64_53(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_54(9223372036854775808, 2):\n  ");
  multshiftround_u64_54(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_55(9223372036854775808, 2):\n  ");
  multshiftround_u64_55(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_56(9223372036854775808, 2):\n  ");
  multshiftround_u64_56(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_57(9223372036854775808, 2):\n  ");
  multshiftround_u64_57(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_58(9223372036854775808, 2):\n  ");
  multshiftround_u64_58(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_59(9223372036854775808, 2):\n  ");
  multshiftround_u64_59(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_60(9223372036854775808, 2):\n  ");
  multshiftround_u64_60(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_61(9223372036854775808, 2):\n  ");
  multshiftround_u64_61(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_62(9223372036854775808, 2):\n  ");
  multshiftround_u64_62(9223372036854775808ull,2ull);
  std::printf("\n");

  std::printf("multshiftround_u64_63(9223372036854775808, 2):\n  ");
  multshiftround_u64_63(9223372036854775808ull,2ull);
  std::printf("\n");

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
