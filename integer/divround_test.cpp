/**
 * divround_test.cpp
 * Tests the following functions for all valid inputs:
 * divround_i8, divround<int8_t>, divround_u8, divround<uint8_t>,
 * divround_i16, divround<int16_t>, divround_u16, and divround<uint16_t>.
 *
 * Tests divround_i32 and divround<int32_t> for all valid combinations
 * of dividend and divisor on the ranges
 * [-2147483648, -2147418112],
 * [-65536, 65536], and
 * [2147418111, 2147483647].
 * Approximately 2^36 tests are conducted on each function.
 *
 * Tests divround_u32 and divround<uint32_t> for all valid combinations
 * of dividend and divisor on the ranges
 * [0, 131072] and
 * [4294836223, 4294967295].
 * Approximately 2^36 tests are conducted on each function.
 *
 * Tests divround_i64 and divround<int64_t> for all valid combinations
 * of dividend and divisor on the ranges
 * [-9223372036854775808, -9223372036854710272],
 * [-65536, 65536], and
 * [9223372036854710271, 9223372036854775807].
 * Approximately 2^36 tests are conducted on each function.
 *
 * Tests divround_u64 and divround<uint64_t> for all valid combinations
 * of dividend and divisor on the ranges
 * [0, 131072] and
 * [18446744073709420543, 18446744073709551615].
 * Approximately 2^36 tests are conducted on each function.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file.If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <limits>
extern "C" 
{
  #include "divround.h"
}
#include "divround.hpp"
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/round.hpp>

typedef boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<80, boost::multiprecision::backends::digit_base_2, void, boost::int16_t, -16382, 16383>, boost::multiprecision::et_off> cpp_bin_float_80;

int main() {
  std::printf("Testing divround_i8.\n");
  int8_t dividend_i8 = std::numeric_limits<int8_t>::lowest();
  while (true) {
    int8_t divisor = std::numeric_limits<int8_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i8 == std::numeric_limits<int8_t>::lowest()) & (divisor == static_cast<int8_t>(-1))) divisor = static_cast<int8_t>(1);
      /* Don't divide by 0. */
      if (divisor == static_cast<int8_t>(0)) divisor = static_cast<int8_t>(1);
      int8_t dr = divround_i8(dividend_i8, divisor);
      int8_t dbl_dr = static_cast<int8_t>(std::round(static_cast<double>(dividend_i8) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int8_t quotient = dividend_i8 / divisor;
        int8_t remainder = dividend_i8 - (quotient * divisor);
        int8_t div_half = divisor >> 1;
        if ((divisor & static_cast<uint8_t>(0x81)) == static_cast<int8_t>(0x01)) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround_i8 returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i8, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int8_t>::max()) break;
      divisor++;
    }
    if (dividend_i8 == std::numeric_limits<int8_t>::max()) break;
    dividend_i8++;
  }

  std::printf("Testing divround_<int8_t>.\n");
  dividend_i8 = std::numeric_limits<int8_t>::lowest();
  while (true) {
    int8_t divisor = std::numeric_limits<int8_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i8 == std::numeric_limits<int8_t>::lowest()) & (divisor == static_cast<int8_t>(-1))) divisor = static_cast<int8_t>(1);
      /* Don't divide by 0. */
      if (divisor == static_cast<int8_t>(0)) divisor = static_cast<int8_t>(1);
      int8_t dr = divround<int8_t>(dividend_i8, divisor);
      int8_t dbl_dr = static_cast<int8_t>(std::round(static_cast<double>(dividend_i8) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int8_t quotient = dividend_i8 / divisor;
        int8_t remainder = dividend_i8 - (quotient * divisor);
        int8_t div_half = divisor >> 1;
        if ((divisor & static_cast<uint8_t>(0x81)) == static_cast<int8_t>(0x01)) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround<int8_t> returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i8, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int8_t>::max()) break;
      divisor++;
    }
    if (dividend_i8 == std::numeric_limits<int8_t>::max()) break;
    dividend_i8++;
  }

  std::printf("Testing divround_u8.\n");
  uint8_t dividend_u8 = std::numeric_limits<uint8_t>::lowest();
  while (true) {
    uint8_t divisor = static_cast<uint8_t>(1);
    while (true) {
      uint8_t dr = divround_u8(dividend_u8, divisor);
      uint8_t dbl_dr = static_cast<uint8_t>(std::round(static_cast<double>(dividend_u8) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint8_t quotient = dividend_u8 / divisor;
        uint8_t remainder = dividend_u8 - (quotient * divisor);
        uint8_t div_half = divisor >> 1;
        if (divisor & static_cast<uint8_t>(0x01)) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround_u8 returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u8, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<uint8_t>::max()) break;
      divisor++;
    }
    if (dividend_u8 == std::numeric_limits<uint8_t>::max()) break;
    dividend_u8++;
  }

  std::printf("Testing divround<uint8_t>.\n");
  dividend_u8 = std::numeric_limits<uint8_t>::lowest();
  while (true) {
    uint8_t divisor = static_cast<uint8_t>(1);
    while (true) {
      uint8_t dr = divround<uint8_t>(dividend_u8, divisor);
      uint8_t dbl_dr = static_cast<uint8_t>(std::round(static_cast<double>(dividend_u8) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint8_t quotient = dividend_u8 / divisor;
        uint8_t remainder = dividend_u8 - (quotient * divisor);
        uint8_t div_half = divisor >> 1;
        if (divisor & static_cast<uint8_t>(0x01)) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround<uint8_t> returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u8, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<uint8_t>::max()) break;
      divisor++;
    }
    if (dividend_u8 == std::numeric_limits<uint8_t>::max()) break;
    dividend_u8++;
  }

  std::printf("Testing divround_i16.\n");
  int16_t dividend_i16 = std::numeric_limits<int16_t>::lowest();
  while (true) {
    int16_t divisor = std::numeric_limits<int16_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i16 == std::numeric_limits<int16_t>::lowest()) & (divisor == static_cast<int16_t>(-1))) divisor = static_cast<int16_t>(1);
      /* Don't divide by 0. */
      if (divisor == static_cast<int16_t>(0)) divisor = static_cast<int16_t>(1);
      int16_t dr = divround_i16(dividend_i16, divisor);
      int16_t dbl_dr = static_cast<int16_t>(std::round(static_cast<double>(dividend_i16) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int16_t quotient = dividend_i16 / divisor;
        int16_t remainder = dividend_i16 - (quotient * divisor);
        int16_t div_half = divisor >> 1;
        if ((divisor & static_cast<uint16_t>(0x8001)) == static_cast<int16_t>(1)) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround_i16 returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i16, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int16_t>::max()) break;
      divisor++;
    }
    if (dividend_i16 == std::numeric_limits<int16_t>::max()) break;
    dividend_i16++;
  }
  
  std::printf("Testing divround<int16_t>.\n");
  dividend_i16 = std::numeric_limits<int16_t>::lowest();
  while (true) {
    int16_t divisor = std::numeric_limits<int16_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i16 == std::numeric_limits<int16_t>::lowest()) & (divisor == static_cast<int16_t>(-1))) divisor = static_cast<int16_t>(1);
      /* Don't divide by 0. */
      if (divisor == static_cast<int16_t>(0)) divisor = static_cast<int16_t>(1);
      int16_t dr = divround<int16_t>(dividend_i16, divisor);
      int16_t dbl_dr = static_cast<int16_t>(std::round(static_cast<double>(dividend_i16) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int16_t quotient = dividend_i16 / divisor;
        int16_t remainder = dividend_i16 - (quotient * divisor);
        int16_t div_half = divisor >> 1;
        if ((divisor & static_cast<uint16_t>(0x8001)) == static_cast<int16_t>(1)) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround<int16_t> returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i16, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int16_t>::max()) break;
      divisor++;
    }
    if (dividend_i16 == std::numeric_limits<int16_t>::max()) break;
    dividend_i16++;
  }

  std::printf("Testing divround_u16.\n");
  uint16_t dividend_u16 = std::numeric_limits<uint16_t>::lowest();
  while (true) {
    uint16_t divisor = static_cast<uint16_t>(1);
    while (true) {
      uint16_t dr = divround_u16(dividend_u16, divisor);
      uint16_t dbl_dr = static_cast<uint16_t>(std::round(static_cast<double>(dividend_u16) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint16_t quotient = dividend_u16 / divisor;
        uint16_t remainder = dividend_u16 - (quotient * divisor);
        uint16_t div_half = divisor >> 1;
        if (divisor & static_cast<uint16_t>(0x0001)) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround_u16 returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u16, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<uint16_t>::max()) break;
      divisor++;
    }
    if (dividend_u16 == std::numeric_limits<uint16_t>::max()) break;
    dividend_u16++;
  }

  std::printf("Testing divround<uint16_t>.\n");
  dividend_u16 = std::numeric_limits<uint16_t>::lowest();
  while (true) {
    uint16_t divisor = static_cast<uint16_t>(1);
    while (true) {
      uint16_t dr = divround<uint16_t>(dividend_u16, divisor);
      uint16_t dbl_dr = static_cast<uint16_t>(std::round(static_cast<double>(dividend_u16) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint16_t quotient = dividend_u16 / divisor;
        uint16_t remainder = dividend_u16 - (quotient * divisor);
        uint16_t div_half = divisor >> 1;
        if (divisor & static_cast<uint16_t>(0x0001)) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround<uint16_t> returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u16, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<uint16_t>::max()) break;
      divisor++;
    }
    if (dividend_u16 == std::numeric_limits<uint16_t>::max()) break;
    dividend_u16++;
  }

  std::printf("Testing divround_i32.\n");
  int32_t dividend_i32 = std::numeric_limits<int32_t>::lowest();
  while (true) {
    int32_t divisor = std::numeric_limits<int32_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i32 == std::numeric_limits<int32_t>::lowest()) & (divisor == -1)) divisor = 1;
      /* Don't divide by 0. */
      if (divisor == 0) divisor = 1;
      int32_t dr = divround_i32(dividend_i32, divisor);
      int32_t dbl_dr = static_cast<int32_t>(std::round(static_cast<double>(dividend_i32) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int32_t quotient = dividend_i32 / divisor;
        int32_t remainder = dividend_i32 - (quotient * divisor);
        int32_t div_half = divisor >> 1;
        if ((divisor & 0x80000001u) == 1) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround_i32 returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i32, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int32_t>::lowest() + (1 << 16)) divisor = -(1 << 16);
      else if (divisor == (1 << 16)) divisor = std::numeric_limits<int32_t>::max() - (1 << 16);
      else if (divisor == std::numeric_limits<int32_t>::max()) break;
      else divisor++;
    }
    if (dividend_i32 == std::numeric_limits<int32_t>::lowest() + (1 << 16)) dividend_i32 = -(1 << 16);
    else if (dividend_i32 == (1 << 16)) dividend_i32 = std::numeric_limits<int32_t>::max() - (1 << 16);
    else if (dividend_i32 == std::numeric_limits<int32_t>::max()) break;
    else dividend_i32++;
  }

  std::printf("Testing divround<int32_t>.\n");
  dividend_i32 = std::numeric_limits<int32_t>::lowest();
  while (true) {
    int32_t divisor = std::numeric_limits<int32_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i32 == std::numeric_limits<int32_t>::lowest()) & (divisor == -1)) divisor = 1;
      /* Don't divide by 0. */
      if (divisor == 0) divisor = 1;
      int32_t dr = divround<int32_t>(dividend_i32, divisor);
      int32_t dbl_dr = static_cast<int32_t>(std::round(static_cast<double>(dividend_i32) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        int32_t quotient = dividend_i32 / divisor;
        int32_t remainder = dividend_i32 - (quotient * divisor);
        int32_t div_half = divisor >> 1;
        if ((divisor & 0x80000001u) == 1) div_half++;
        std::printf("ROUND(%i / %i) = %i, but divround<int32_t> returns %i\n  remainder = %i, div_half = %i\n\n", dividend_i32, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int32_t>::lowest() + (1 << 16)) divisor = -(1 << 16);
      else if (divisor == (1 << 16)) divisor = std::numeric_limits<int32_t>::max() - (1 << 16);
      else if (divisor == std::numeric_limits<int32_t>::max()) break;
      else divisor++;
    }
    if (dividend_i32 == std::numeric_limits<int32_t>::lowest() + (1 << 16)) dividend_i32 = -(1 << 16);
    else if (dividend_i32 == (1 << 16)) dividend_i32 = std::numeric_limits<int32_t>::max() - (1 << 16);
    else if (dividend_i32 == std::numeric_limits<int32_t>::max()) break;
    else dividend_i32++;
  }

  std::printf("Testing divround_u32.\n");
  uint32_t dividend_u32 = std::numeric_limits<uint32_t>::lowest();
  while (true) {
    uint32_t divisor = 1u;
    while (true) {
      uint32_t dr = divround_u32(dividend_u32, divisor);
      uint32_t dbl_dr = static_cast<uint32_t>(std::round(static_cast<double>(dividend_u32) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint32_t quotient = dividend_u32 / divisor;
        uint32_t remainder = dividend_u32 - (quotient * divisor);
        uint32_t div_half = divisor >> 1;
        if (divisor & 0x00000001u) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround_u32 returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u32, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == (1u << 17)) divisor = std::numeric_limits<uint32_t>::max() - (1u << 17);
      else if (divisor == std::numeric_limits<uint32_t>::max()) break;
      else divisor++;
    }
    if (dividend_u32 == (1u << 17)) dividend_u32 = std::numeric_limits<uint32_t>::max() - (1u << 17);
    else if (dividend_u32 == std::numeric_limits<uint32_t>::max()) break;
    else dividend_u32++;
  }

  std::printf("Testing divround<uint32_t>.\n");
  dividend_u32 = std::numeric_limits<uint32_t>::lowest();
  while (true) {
    uint32_t divisor = 1u;
    while (true) {
      uint32_t dr = divround<uint32_t>(dividend_u32, divisor);
      uint32_t dbl_dr = static_cast<uint32_t>(std::round(static_cast<double>(dividend_u32) / static_cast<double>(divisor)));
      if (dr != dbl_dr) {
        uint32_t quotient = dividend_u32 / divisor;
        uint32_t remainder = dividend_u32 - (quotient * divisor);
        uint32_t div_half = divisor >> 1;
        if (divisor & 0x00000001u) div_half++;
        std::printf("ROUND(%u / %u) = %u, but divround<uint32_t> returns %u\n  remainder = %u, div_half = %u\n\n", dividend_u32, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == (1u << 17)) divisor = std::numeric_limits<uint32_t>::max() - (1u << 17);
      else if (divisor == std::numeric_limits<uint32_t>::max()) break;
      else divisor++;
    }
    if (dividend_u32 == (1u << 17)) dividend_u32 = std::numeric_limits<uint32_t>::max() - (1u << 17);
    else if (dividend_u32 == std::numeric_limits<uint32_t>::max()) break;
    else dividend_u32++;
  }

  std::printf("Testing divround_i64.\n");
  int64_t dividend_i64 = std::numeric_limits<int64_t>::lowest();
  while (true) {
    int64_t divisor = std::numeric_limits<int64_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i64 == std::numeric_limits<int64_t>::lowest()) & (divisor == -1ll)) divisor = 1ll;
      /* Don't divide by 0. */
      if (divisor == 0ll) divisor = 1ll;
      int64_t dr = divround_i64(dividend_i64, divisor);
      int64_t dbl_dr = boost::math::round(cpp_bin_float_80(dividend_i64) / cpp_bin_float_80(divisor)).convert_to<int64_t>();
      if (dr != dbl_dr) {
        int64_t quotient = dividend_i64 / divisor;
        int64_t remainder = dividend_i64 - (quotient * divisor);
        int64_t div_half = divisor >> 1;
        if ((divisor & 0x8000000000000001ull) == 1ll) div_half++;
        std::printf("ROUND(%" PRIi64 " / %" PRIi64 ") = %" PRIi64 ", but divround_i64 returns %" PRIi64 "\n  remainder = %" PRIi64 ", div_half = %" PRIi64 "\n\n", dividend_i64, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int64_t>::lowest() + (1 << 16)) divisor = -(1 << 16);
      else if (divisor == (1 << 16)) divisor = std::numeric_limits<int64_t>::max() - (1 << 16);
      else if (divisor == std::numeric_limits<int64_t>::max()) break;
      else divisor++;
    }
    if (dividend_i64 == std::numeric_limits<int64_t>::lowest() + (1ll << 16)) dividend_i64 = -(1ll << 16);
    else if (dividend_i64 == (1ll << 16)) dividend_i64 = std::numeric_limits<int64_t>::max() - (1ll << 16);
    else if (dividend_i64 == std::numeric_limits<int64_t>::max()) break;
    else dividend_i64++;
  }

  std::printf("Testing divround<int64_t>.\n");
  dividend_i64 = std::numeric_limits<int64_t>::lowest();
  while (true) {
    int64_t divisor = std::numeric_limits<int64_t>::lowest();
    while (true) {
      /* This would cause overflow in the result. Skip it. */
      if ((dividend_i64 == std::numeric_limits<int64_t>::lowest()) & (divisor == -1ll)) divisor = 1ll;
      /* Don't divide by 0. */
      if (divisor == 0ll) divisor = 1ll;
      int64_t dr = divround<int64_t>(dividend_i64, divisor);
      int64_t dbl_dr = boost::math::round(cpp_bin_float_80(dividend_i64) / cpp_bin_float_80(divisor)).convert_to<int64_t>();
      if (dr != dbl_dr) {
        int64_t quotient = dividend_i64 / divisor;
        int64_t remainder = dividend_i64 - (quotient * divisor);
        int64_t div_half = divisor >> 1;
        if ((divisor & 0x8000000000000001ull) == 1ll) div_half++;
        std::printf("ROUND(%" PRIi64 " / %" PRIi64 ") = %" PRIi64 ", but divround<int64_t> returns %" PRIi64 "\n  remainder = %" PRIi64 ", div_half = %" PRIi64 "\n\n", dividend_i64, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == std::numeric_limits<int64_t>::lowest() + (1 << 16)) divisor = -(1 << 16);
      else if (divisor == (1 << 16)) divisor = std::numeric_limits<int64_t>::max() - (1 << 16);
      else if (divisor == std::numeric_limits<int64_t>::max()) break;
      else divisor++;
    }
    if (dividend_i64 == std::numeric_limits<int64_t>::lowest() + (1ll << 16)) dividend_i64 = -(1ll << 16);
    else if (dividend_i64 == (1ll << 16)) dividend_i64 = std::numeric_limits<int64_t>::max() - (1ll << 16);
    else if (dividend_i64 == std::numeric_limits<int64_t>::max()) break;
    else dividend_i64++;
  }

  std::printf("Testing divround_u64.\n");
  uint64_t dividend_u64 = std::numeric_limits<uint64_t>::lowest();
  while (true) {
    uint64_t divisor = 1ull;
    while (true) {
      uint64_t dr = divround_u64(dividend_u64, divisor);
      uint64_t dbl_dr = boost::math::round(cpp_bin_float_80(dividend_u32) / cpp_bin_float_80(divisor)).convert_to<uint64_t>();
      if (dr != dbl_dr) {
        uint64_t quotient = dividend_u64 / divisor;
        uint64_t remainder = dividend_u64 - (quotient * divisor);
        uint64_t div_half = divisor >> 1;
        if (divisor & 0x0000000000000001ull) div_half++;
        std::printf("ROUND(%" PRIu64 " / %" PRIu64 ") = %" PRIu64 ", but divround_u64 returns %" PRIu64 "\n  remainder = %" PRIu64 ", div_half = %" PRIu64 "\n\n", dividend_u64, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == (1ull << 17)) divisor = std::numeric_limits<uint64_t>::max() - (1ull << 17);
      else if (divisor == std::numeric_limits<uint64_t>::max()) break;
      else divisor++;
    }
    if (dividend_u64 == (1ull << 17)) dividend_u64 = std::numeric_limits<uint64_t>::max() - (1ull << 17);
    else if (dividend_u64 == std::numeric_limits<uint64_t>::max()) break;
    else dividend_u64++;
  }

  std::printf("Testing divround<uint64_t>.\n");
  dividend_u64 = std::numeric_limits<uint64_t>::lowest();
  while (true) {
    uint64_t divisor = 1ull;
    while (true) {
      uint64_t dr = divround<uint64_t>(dividend_u64, divisor);
      uint64_t dbl_dr = boost::math::round(cpp_bin_float_80(dividend_u32) / cpp_bin_float_80(divisor)).convert_to<uint64_t>();
      if (dr != dbl_dr) {
        uint64_t quotient = dividend_u64 / divisor;
        uint64_t remainder = dividend_u64 - (quotient * divisor);
        uint64_t div_half = divisor >> 1;
        if (divisor & 0x0000000000000001ull) div_half++;
        std::printf("ROUND(%" PRIu64 " / %" PRIu64 ") = %" PRIu64 ", but divround<uint64_t> returns %" PRIu64 "\n  remainder = %" PRIu64 ", div_half = %" PRIu64 "\n\n", dividend_u64, divisor, dbl_dr, dr, remainder, div_half);
      }
      if (divisor == (1ull << 17)) divisor = std::numeric_limits<uint64_t>::max() - (1ull << 17);
      else if (divisor == std::numeric_limits<uint64_t>::max()) break;
      else divisor++;
    }
    if (dividend_u64 == (1ull << 17)) dividend_u64 = std::numeric_limits<uint64_t>::max() - (1ull << 17);
    else if (dividend_u64 == std::numeric_limits<uint64_t>::max()) break;
    else dividend_u64++;
  }

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