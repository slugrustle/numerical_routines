/**
 * multshiftround_shiftround_test.cpp
 * Unit tests for the templated C++ functions
 *     type shiftround<typename type, int8_t shift>(const type num);
 * and
 *     type multshiftround<typename type, int8_t shift>(const type num, const type mul);
 * found in shiftround.hpp and multshiftround.hpp as well as the C
 * functions of the form
 *     type shiftround_X_Y(const type num);
 * and
 *     type multshiftround_X_Y(const type num, const type mul);
 * declared and defined in shiftround.h, multshiftround.h, shiftround.c,
 * and multshiftround.c.
 *
 * Full coverage is provided on the num and shift (or Y) inputs for the
 * int8_t, int16_t, int32_t, uint8_t, uint16_t, and uint32_t types.
 *
 * The coverage of the num input for 64-bit types is only partial in
 * order to acheive a reasonable test time, especially since the
 * extended precision floating point calculations required to test the
 * 64-bit routines are slow.
 * The int64_t type is tested for num on [-2^63, -2^63+2^22],
 * [-2^22, 2^22], and [2^63-2^22-1, 2^63-1] with full coverage of the
 * shift (or Y) input. The uint64_t type is tested for num on [0, 2^23]
 * and [2^64-2^23-1, 2^64-1] with full coverage of the shift (or Y) input.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */
#include <cstdio>
#include <cmath>
#include <limits>
#include "multshiftround.hpp"
#include "shiftround.hpp"
extern "C"
{
  #include "multshiftround.h"
  #include "shiftround.h"
}
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/round.hpp>

typedef boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<80, boost::multiprecision::backends::digit_base_2, void, boost::int16_t, -16382, 16383>, boost::multiprecision::et_off> cpp_bin_float_80;

/**
 * Setting the mul argument of multshiftround to 1 for various types.
 * Everything is tested except the multiplication statement
 *     type prod = num * mul;
 * in the various specializations of multshiftround<>().
 */
const int8_t   mul_i8 = 1;
const double   dbl_mul_i8 = static_cast<double>(mul_i8);
const int16_t  mul_i16 = 1;
const double   dbl_mul_i16 = static_cast<double>(mul_i16);
const int32_t  mul_i32 = 1;
const double   dbl_mul_i32 = static_cast<double>(mul_i32);
const int64_t  mul_i64 = 1ll;
const cpp_bin_float_80 ldbl_mul_i64(mul_i64);
const uint8_t  mul_u8 = 1u;
const double   dbl_mul_u8 = static_cast<double>(mul_u8);
const uint16_t mul_u16 = 1u;
const double   dbl_mul_u16 = static_cast<double>(mul_u16);
const uint32_t mul_u32 = 1u;
const double   dbl_mul_u32 = static_cast<double>(mul_u32);
const uint64_t mul_u64 = 1ull;
const cpp_bin_float_80 ldbl_mul_u64(mul_u64);

int main()
{
  /**
   * Test int8_t multshiftround for num on [-128, 127] and shift on [1, 6].
   */
  for (int8_t shift = 1; shift <= 6; shift++)
  {
    std::printf("testing multshiftround<int8_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    while (true)
    {
      int8_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<int8_t, 1>(num, mul_i8); break;
        case  2: ms_res = multshiftround<int8_t, 2>(num, mul_i8); break;
        case  3: ms_res = multshiftround<int8_t, 3>(num, mul_i8); break;
        case  4: ms_res = multshiftround<int8_t, 4>(num, mul_i8); break;
        case  5: ms_res = multshiftround<int8_t, 5>(num, mul_i8); break;
        case  6: ms_res = multshiftround<int8_t, 6>(num, mul_i8); break;
        default: std::printf("ERROR: invalid shift int8_t\n"); return 0; break;
      }

      int8_t dbl_res = static_cast<int8_t>(std::round((static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, %i>(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i8_Y for num on [-128, 127] and Y on [1, 6].
   */
  for (int8_t shift = 1; shift <= 6; shift++)
  {
    std::printf("testing multshiftround_i8_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    while (true)
    {
      int8_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_i8_1(num, mul_i8); break;
        case  2: ms_res = multshiftround_i8_2(num, mul_i8); break;
        case  3: ms_res = multshiftround_i8_3(num, mul_i8); break;
        case  4: ms_res = multshiftround_i8_4(num, mul_i8); break;
        case  5: ms_res = multshiftround_i8_5(num, mul_i8); break;
        case  6: ms_res = multshiftround_i8_6(num, mul_i8); break;
        default: std::printf("ERROR: invalid shift int8_t\n"); return 0; break;
      }

      int8_t dbl_res = static_cast<int8_t>(std::round((static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_%i(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int8_t shiftround for num on [-128, 127] and shift on [1, 6].
   */
  for (int8_t shift = 1; shift <= 6; shift++)
  {
    std::printf("testing shiftround<int8_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    while (true)
    {
      int8_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<int8_t, 1>(num); break;
        case  2: s_res = shiftround<int8_t, 2>(num); break;
        case  3: s_res = shiftround<int8_t, 3>(num); break;
        case  4: s_res = shiftround<int8_t, 4>(num); break;
        case  5: s_res = shiftround<int8_t, 5>(num); break;
        case  6: s_res = shiftround<int8_t, 6>(num); break;
        default: std::printf("ERROR: invalid shift int8_t\n"); return 0; break;
      }

      int8_t dbl_res = static_cast<int8_t>(std::round((static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<int8_t, %i>(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i8_Y for num on [-128, 127] and Y on [1, 6].
   */
  for (int8_t shift = 1; shift <= 6; shift++)
  {
    std::printf("testing shiftround_i8_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    while (true)
    {
      int8_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_i8_1(num); break;
        case  2: s_res = shiftround_i8_2(num); break;
        case  3: s_res = shiftround_i8_3(num); break;
        case  4: s_res = shiftround_i8_4(num); break;
        case  5: s_res = shiftround_i8_5(num); break;
        case  6: s_res = shiftround_i8_6(num); break;
        default: std::printf("ERROR: invalid shift int8_t\n"); return 0; break;
      }

      int8_t dbl_res = static_cast<int8_t>(std::round((static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_i8_%i(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i8) / dbl_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int16_t multshiftround for num on [-32768, 32767] and shift on [1, 14].
   */
  for (int8_t shift = 1; shift <= 14; shift++)
  {
    std::printf("testing multshiftround<int16_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    while (true)
    {
      int16_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<int16_t,  1>(num, mul_i16); break;
        case  2: ms_res = multshiftround<int16_t,  2>(num, mul_i16); break;
        case  3: ms_res = multshiftround<int16_t,  3>(num, mul_i16); break;
        case  4: ms_res = multshiftround<int16_t,  4>(num, mul_i16); break;
        case  5: ms_res = multshiftround<int16_t,  5>(num, mul_i16); break;
        case  6: ms_res = multshiftround<int16_t,  6>(num, mul_i16); break;
        case  7: ms_res = multshiftround<int16_t,  7>(num, mul_i16); break;
        case  8: ms_res = multshiftround<int16_t,  8>(num, mul_i16); break;
        case  9: ms_res = multshiftround<int16_t,  9>(num, mul_i16); break;
        case 10: ms_res = multshiftround<int16_t, 10>(num, mul_i16); break;
        case 11: ms_res = multshiftround<int16_t, 11>(num, mul_i16); break;
        case 12: ms_res = multshiftround<int16_t, 12>(num, mul_i16); break;
        case 13: ms_res = multshiftround<int16_t, 13>(num, mul_i16); break;
        case 14: ms_res = multshiftround<int16_t, 14>(num, mul_i16); break;
        default: std::printf("ERROR: invalid shift int16_t\n"); return 0; break;
      }

      int16_t dbl_res = static_cast<int16_t>(std::round((static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, %i>(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i16_Y for num on [-32768, 32767] and Y on [1, 14].
   */
  for (int8_t shift = 1; shift <= 14; shift++)
  {
    std::printf("testing multshiftround_i16_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    while (true)
    {
      int16_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_i16_1(num, mul_i16); break;
        case  2: ms_res = multshiftround_i16_2(num, mul_i16); break;
        case  3: ms_res = multshiftround_i16_3(num, mul_i16); break;
        case  4: ms_res = multshiftround_i16_4(num, mul_i16); break;
        case  5: ms_res = multshiftround_i16_5(num, mul_i16); break;
        case  6: ms_res = multshiftround_i16_6(num, mul_i16); break;
        case  7: ms_res = multshiftround_i16_7(num, mul_i16); break;
        case  8: ms_res = multshiftround_i16_8(num, mul_i16); break;
        case  9: ms_res = multshiftround_i16_9(num, mul_i16); break;
        case 10: ms_res = multshiftround_i16_10(num, mul_i16); break;
        case 11: ms_res = multshiftround_i16_11(num, mul_i16); break;
        case 12: ms_res = multshiftround_i16_12(num, mul_i16); break;
        case 13: ms_res = multshiftround_i16_13(num, mul_i16); break;
        case 14: ms_res = multshiftround_i16_14(num, mul_i16); break;
        default: std::printf("ERROR: invalid shift int16_t\n"); return 0; break;
      }

      int16_t dbl_res = static_cast<int16_t>(std::round((static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_%i(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int16_t shiftround for num on [-32768, 32767] and shift on [1, 14].
   */
  for (int8_t shift = 1; shift <= 14; shift++)
  {
    std::printf("testing shiftround<int16_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    while (true)
    {
      int16_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<int16_t,  1>(num); break;
        case  2: s_res = shiftround<int16_t,  2>(num); break;
        case  3: s_res = shiftround<int16_t,  3>(num); break;
        case  4: s_res = shiftround<int16_t,  4>(num); break;
        case  5: s_res = shiftround<int16_t,  5>(num); break;
        case  6: s_res = shiftround<int16_t,  6>(num); break;
        case  7: s_res = shiftround<int16_t,  7>(num); break;
        case  8: s_res = shiftround<int16_t,  8>(num); break;
        case  9: s_res = shiftround<int16_t,  9>(num); break;
        case 10: s_res = shiftround<int16_t, 10>(num); break;
        case 11: s_res = shiftround<int16_t, 11>(num); break;
        case 12: s_res = shiftround<int16_t, 12>(num); break;
        case 13: s_res = shiftround<int16_t, 13>(num); break;
        case 14: s_res = shiftround<int16_t, 14>(num); break;
        default: std::printf("ERROR: invalid shift int16_t\n"); return 0; break;
      }

      int16_t dbl_res = static_cast<int16_t>(std::round((static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<int16_t, %i>(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i16_Y for num on [-32768, 32767] and Y on [1, 14].
   */
  for (int8_t shift = 1; shift <= 14; shift++)
  {
    std::printf("testing shiftround_i16_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    while (true)
    {
      int16_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_i16_1(num); break;
        case  2: s_res = shiftround_i16_2(num); break;
        case  3: s_res = shiftround_i16_3(num); break;
        case  4: s_res = shiftround_i16_4(num); break;
        case  5: s_res = shiftround_i16_5(num); break;
        case  6: s_res = shiftround_i16_6(num); break;
        case  7: s_res = shiftround_i16_7(num); break;
        case  8: s_res = shiftround_i16_8(num); break;
        case  9: s_res = shiftround_i16_9(num); break;
        case 10: s_res = shiftround_i16_10(num); break;
        case 11: s_res = shiftround_i16_11(num); break;
        case 12: s_res = shiftround_i16_12(num); break;
        case 13: s_res = shiftround_i16_13(num); break;
        case 14: s_res = shiftround_i16_14(num); break;
        default: std::printf("ERROR: invalid shift int16_t\n"); return 0; break;
      }

      int16_t dbl_res = static_cast<int16_t>(std::round((static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_i16_%i(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i16) / dbl_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int32_t multshiftround for num on [-2147483648, 2147483647] and shift on [1, 30].
   */
  for (int8_t shift = 1; shift <= 30; shift++)
  {
    std::printf("testing multshiftround<int32_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    while (true)
    {
      int32_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<int32_t, 1>(num, mul_i32); break;
        case  2: ms_res = multshiftround<int32_t, 2>(num, mul_i32); break;
        case  3: ms_res = multshiftround<int32_t, 3>(num, mul_i32); break;
        case  4: ms_res = multshiftround<int32_t, 4>(num, mul_i32); break;
        case  5: ms_res = multshiftround<int32_t, 5>(num, mul_i32); break;
        case  6: ms_res = multshiftround<int32_t, 6>(num, mul_i32); break;
        case  7: ms_res = multshiftround<int32_t, 7>(num, mul_i32); break;
        case  8: ms_res = multshiftround<int32_t, 8>(num, mul_i32); break;
        case  9: ms_res = multshiftround<int32_t, 9>(num, mul_i32); break;
        case 10: ms_res = multshiftround<int32_t,10>(num, mul_i32); break;
        case 11: ms_res = multshiftround<int32_t,11>(num, mul_i32); break;
        case 12: ms_res = multshiftround<int32_t,12>(num, mul_i32); break;
        case 13: ms_res = multshiftround<int32_t,13>(num, mul_i32); break;
        case 14: ms_res = multshiftround<int32_t,14>(num, mul_i32); break;
        case 15: ms_res = multshiftround<int32_t,15>(num, mul_i32); break;
        case 16: ms_res = multshiftround<int32_t,16>(num, mul_i32); break;
        case 17: ms_res = multshiftround<int32_t,17>(num, mul_i32); break;
        case 18: ms_res = multshiftround<int32_t,18>(num, mul_i32); break;
        case 19: ms_res = multshiftround<int32_t,19>(num, mul_i32); break;
        case 20: ms_res = multshiftround<int32_t,20>(num, mul_i32); break;
        case 21: ms_res = multshiftround<int32_t,21>(num, mul_i32); break;
        case 22: ms_res = multshiftround<int32_t,22>(num, mul_i32); break;
        case 23: ms_res = multshiftround<int32_t,23>(num, mul_i32); break;
        case 24: ms_res = multshiftround<int32_t,24>(num, mul_i32); break;
        case 25: ms_res = multshiftround<int32_t,25>(num, mul_i32); break;
        case 26: ms_res = multshiftround<int32_t,26>(num, mul_i32); break;
        case 27: ms_res = multshiftround<int32_t,27>(num, mul_i32); break;
        case 28: ms_res = multshiftround<int32_t,28>(num, mul_i32); break;
        case 29: ms_res = multshiftround<int32_t,29>(num, mul_i32); break;
        case 30: ms_res = multshiftround<int32_t,30>(num, mul_i32); break;
        default: std::printf("ERROR: invalid shift int32_t\n"); return 0; break;
      }

      int32_t dbl_res = static_cast<int32_t>(std::round((static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int32_t, %i>(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp, num, mul_i32);
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i32_Y for num on [-2147483648, 2147483647] and Y on [1, 30].
   */
  for (int8_t shift = 1; shift <= 30; shift++)
  {
    std::printf("testing multshiftround_i32_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    while (true)
    {
      int32_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_i32_1(num, mul_i32); break;
        case  2: ms_res = multshiftround_i32_2(num, mul_i32); break;
        case  3: ms_res = multshiftround_i32_3(num, mul_i32); break;
        case  4: ms_res = multshiftround_i32_4(num, mul_i32); break;
        case  5: ms_res = multshiftround_i32_5(num, mul_i32); break;
        case  6: ms_res = multshiftround_i32_6(num, mul_i32); break;
        case  7: ms_res = multshiftround_i32_7(num, mul_i32); break;
        case  8: ms_res = multshiftround_i32_8(num, mul_i32); break;
        case  9: ms_res = multshiftround_i32_9(num, mul_i32); break;
        case 10: ms_res = multshiftround_i32_10(num, mul_i32); break;
        case 11: ms_res = multshiftround_i32_11(num, mul_i32); break;
        case 12: ms_res = multshiftround_i32_12(num, mul_i32); break;
        case 13: ms_res = multshiftround_i32_13(num, mul_i32); break;
        case 14: ms_res = multshiftround_i32_14(num, mul_i32); break;
        case 15: ms_res = multshiftround_i32_15(num, mul_i32); break;
        case 16: ms_res = multshiftround_i32_16(num, mul_i32); break;
        case 17: ms_res = multshiftround_i32_17(num, mul_i32); break;
        case 18: ms_res = multshiftround_i32_18(num, mul_i32); break;
        case 19: ms_res = multshiftround_i32_19(num, mul_i32); break;
        case 20: ms_res = multshiftround_i32_20(num, mul_i32); break;
        case 21: ms_res = multshiftround_i32_21(num, mul_i32); break;
        case 22: ms_res = multshiftround_i32_22(num, mul_i32); break;
        case 23: ms_res = multshiftround_i32_23(num, mul_i32); break;
        case 24: ms_res = multshiftround_i32_24(num, mul_i32); break;
        case 25: ms_res = multshiftround_i32_25(num, mul_i32); break;
        case 26: ms_res = multshiftround_i32_26(num, mul_i32); break;
        case 27: ms_res = multshiftround_i32_27(num, mul_i32); break;
        case 28: ms_res = multshiftround_i32_28(num, mul_i32); break;
        case 29: ms_res = multshiftround_i32_29(num, mul_i32); break;
        case 30: ms_res = multshiftround_i32_30(num, mul_i32); break;
        default: std::printf("ERROR: invalid shift int32_t\n"); return 0; break;
      }

      int32_t dbl_res = static_cast<int32_t>(std::round((static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i32_%i(): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp, num, mul_i32);
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int32_t shiftround for num on [-2147483648, 2147483647] and shift on [1, 30].
   */
  for (int8_t shift = 1; shift <= 30; shift++)
  {
    std::printf("testing shiftround<int32_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    while (true)
    {
      int32_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<int32_t,  1>(num); break;
        case  2: s_res = shiftround<int32_t,  2>(num); break;
        case  3: s_res = shiftround<int32_t,  3>(num); break;
        case  4: s_res = shiftround<int32_t,  4>(num); break;
        case  5: s_res = shiftround<int32_t,  5>(num); break;
        case  6: s_res = shiftround<int32_t,  6>(num); break;
        case  7: s_res = shiftround<int32_t,  7>(num); break;
        case  8: s_res = shiftround<int32_t,  8>(num); break;
        case  9: s_res = shiftround<int32_t,  9>(num); break;
        case 10: s_res = shiftround<int32_t, 10>(num); break;
        case 11: s_res = shiftround<int32_t, 11>(num); break;
        case 12: s_res = shiftround<int32_t, 12>(num); break;
        case 13: s_res = shiftround<int32_t, 13>(num); break;
        case 14: s_res = shiftround<int32_t, 14>(num); break;
        case 15: s_res = shiftround<int32_t, 15>(num); break;
        case 16: s_res = shiftround<int32_t, 16>(num); break;
        case 17: s_res = shiftround<int32_t, 17>(num); break;
        case 18: s_res = shiftround<int32_t, 18>(num); break;
        case 19: s_res = shiftround<int32_t, 19>(num); break;
        case 20: s_res = shiftround<int32_t, 20>(num); break;
        case 21: s_res = shiftround<int32_t, 21>(num); break;
        case 22: s_res = shiftround<int32_t, 22>(num); break;
        case 23: s_res = shiftround<int32_t, 23>(num); break;
        case 24: s_res = shiftround<int32_t, 24>(num); break;
        case 25: s_res = shiftround<int32_t, 25>(num); break;
        case 26: s_res = shiftround<int32_t, 26>(num); break;
        case 27: s_res = shiftround<int32_t, 27>(num); break;
        case 28: s_res = shiftround<int32_t, 28>(num); break;
        case 29: s_res = shiftround<int32_t, 29>(num); break;
        case 30: s_res = shiftround<int32_t, 30>(num); break;
        default: std::printf("ERROR: invalid shift int32_t\n"); return 0; break;
      }

      int32_t dbl_res = static_cast<int32_t>(std::round((static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<int32_t, %i>(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp, num, mul_i32);
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i32_Y for num on [-2147483648, 2147483647] and Y on [1, 30].
   */
  for (int8_t shift = 1; shift <= 30; shift++)
  {
    std::printf("testing shiftround_i32_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    while (true)
    {
      int32_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_i32_1(num); break;
        case  2: s_res = shiftround_i32_2(num); break;
        case  3: s_res = shiftround_i32_3(num); break;
        case  4: s_res = shiftround_i32_4(num); break;
        case  5: s_res = shiftround_i32_5(num); break;
        case  6: s_res = shiftround_i32_6(num); break;
        case  7: s_res = shiftround_i32_7(num); break;
        case  8: s_res = shiftround_i32_8(num); break;
        case  9: s_res = shiftround_i32_9(num); break;
        case 10: s_res = shiftround_i32_10(num); break;
        case 11: s_res = shiftround_i32_11(num); break;
        case 12: s_res = shiftround_i32_12(num); break;
        case 13: s_res = shiftround_i32_13(num); break;
        case 14: s_res = shiftround_i32_14(num); break;
        case 15: s_res = shiftround_i32_15(num); break;
        case 16: s_res = shiftround_i32_16(num); break;
        case 17: s_res = shiftround_i32_17(num); break;
        case 18: s_res = shiftround_i32_18(num); break;
        case 19: s_res = shiftround_i32_19(num); break;
        case 20: s_res = shiftround_i32_20(num); break;
        case 21: s_res = shiftround_i32_21(num); break;
        case 22: s_res = shiftround_i32_22(num); break;
        case 23: s_res = shiftround_i32_23(num); break;
        case 24: s_res = shiftround_i32_24(num); break;
        case 25: s_res = shiftround_i32_25(num); break;
        case 26: s_res = shiftround_i32_26(num); break;
        case 27: s_res = shiftround_i32_27(num); break;
        case 28: s_res = shiftround_i32_28(num); break;
        case 29: s_res = shiftround_i32_29(num); break;
        case 30: s_res = shiftround_i32_30(num); break;
        default: std::printf("ERROR: invalid shift int32_t\n"); return 0; break;
      }

      int32_t dbl_res = static_cast<int32_t>(std::round((static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_i32_%i(): s_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_i32) / dbl_twoexp, num, mul_i32);
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int64_t multshiftround for num on the ranges
   * [-9223372036854775808, -9223372036850581504]
   * [-4194304, 4194304]
   * [9223372036850581503, 9223372036854775807]
   * and shift on [1, 62].
   */
  for (int8_t shift = 1; shift <= 62; shift++)
  {
    std::printf("testing multshiftround<int64_t, %i>()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    int64_t num = std::numeric_limits<int64_t>::lowest();
    while (true)
    {
      int64_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<int64_t,  1>(num, mul_i64); break;
        case  2: ms_res = multshiftround<int64_t,  2>(num, mul_i64); break;
        case  3: ms_res = multshiftround<int64_t,  3>(num, mul_i64); break;
        case  4: ms_res = multshiftround<int64_t,  4>(num, mul_i64); break;
        case  5: ms_res = multshiftround<int64_t,  5>(num, mul_i64); break;
        case  6: ms_res = multshiftround<int64_t,  6>(num, mul_i64); break;
        case  7: ms_res = multshiftround<int64_t,  7>(num, mul_i64); break;
        case  8: ms_res = multshiftround<int64_t,  8>(num, mul_i64); break;
        case  9: ms_res = multshiftround<int64_t,  9>(num, mul_i64); break;
        case 10: ms_res = multshiftround<int64_t, 10>(num, mul_i64); break;
        case 11: ms_res = multshiftround<int64_t, 11>(num, mul_i64); break;
        case 12: ms_res = multshiftround<int64_t, 12>(num, mul_i64); break;
        case 13: ms_res = multshiftround<int64_t, 13>(num, mul_i64); break;
        case 14: ms_res = multshiftround<int64_t, 14>(num, mul_i64); break;
        case 15: ms_res = multshiftround<int64_t, 15>(num, mul_i64); break;
        case 16: ms_res = multshiftround<int64_t, 16>(num, mul_i64); break;
        case 17: ms_res = multshiftround<int64_t, 17>(num, mul_i64); break;
        case 18: ms_res = multshiftround<int64_t, 18>(num, mul_i64); break;
        case 19: ms_res = multshiftround<int64_t, 19>(num, mul_i64); break;
        case 20: ms_res = multshiftround<int64_t, 20>(num, mul_i64); break;
        case 21: ms_res = multshiftround<int64_t, 21>(num, mul_i64); break;
        case 22: ms_res = multshiftround<int64_t, 22>(num, mul_i64); break;
        case 23: ms_res = multshiftround<int64_t, 23>(num, mul_i64); break;
        case 24: ms_res = multshiftround<int64_t, 24>(num, mul_i64); break;
        case 25: ms_res = multshiftround<int64_t, 25>(num, mul_i64); break;
        case 26: ms_res = multshiftround<int64_t, 26>(num, mul_i64); break;
        case 27: ms_res = multshiftround<int64_t, 27>(num, mul_i64); break;
        case 28: ms_res = multshiftround<int64_t, 28>(num, mul_i64); break;
        case 29: ms_res = multshiftround<int64_t, 29>(num, mul_i64); break;
        case 30: ms_res = multshiftround<int64_t, 30>(num, mul_i64); break;
        case 31: ms_res = multshiftround<int64_t, 31>(num, mul_i64); break;
        case 32: ms_res = multshiftround<int64_t, 32>(num, mul_i64); break;
        case 33: ms_res = multshiftround<int64_t, 33>(num, mul_i64); break;
        case 34: ms_res = multshiftround<int64_t, 34>(num, mul_i64); break;
        case 35: ms_res = multshiftround<int64_t, 35>(num, mul_i64); break;
        case 36: ms_res = multshiftround<int64_t, 36>(num, mul_i64); break;
        case 37: ms_res = multshiftround<int64_t, 37>(num, mul_i64); break;
        case 38: ms_res = multshiftround<int64_t, 38>(num, mul_i64); break;
        case 39: ms_res = multshiftround<int64_t, 39>(num, mul_i64); break;
        case 40: ms_res = multshiftround<int64_t, 40>(num, mul_i64); break;
        case 41: ms_res = multshiftround<int64_t, 41>(num, mul_i64); break;
        case 42: ms_res = multshiftround<int64_t, 42>(num, mul_i64); break;
        case 43: ms_res = multshiftround<int64_t, 43>(num, mul_i64); break;
        case 44: ms_res = multshiftround<int64_t, 44>(num, mul_i64); break;
        case 45: ms_res = multshiftround<int64_t, 45>(num, mul_i64); break;
        case 46: ms_res = multshiftround<int64_t, 46>(num, mul_i64); break;
        case 47: ms_res = multshiftround<int64_t, 47>(num, mul_i64); break;
        case 48: ms_res = multshiftround<int64_t, 48>(num, mul_i64); break;
        case 49: ms_res = multshiftround<int64_t, 49>(num, mul_i64); break;
        case 50: ms_res = multshiftround<int64_t, 50>(num, mul_i64); break;
        case 51: ms_res = multshiftround<int64_t, 51>(num, mul_i64); break;
        case 52: ms_res = multshiftround<int64_t, 52>(num, mul_i64); break;
        case 53: ms_res = multshiftround<int64_t, 53>(num, mul_i64); break;
        case 54: ms_res = multshiftround<int64_t, 54>(num, mul_i64); break;
        case 55: ms_res = multshiftround<int64_t, 55>(num, mul_i64); break;
        case 56: ms_res = multshiftround<int64_t, 56>(num, mul_i64); break;
        case 57: ms_res = multshiftround<int64_t, 57>(num, mul_i64); break;
        case 58: ms_res = multshiftround<int64_t, 58>(num, mul_i64); break;
        case 59: ms_res = multshiftround<int64_t, 59>(num, mul_i64); break;
        case 60: ms_res = multshiftround<int64_t, 60>(num, mul_i64); break;
        case 61: ms_res = multshiftround<int64_t, 61>(num, mul_i64); break;
        case 62: ms_res = multshiftround<int64_t, 62>(num, mul_i64); break;
        default: std::printf("ERROR: invalid shift int64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      int64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<int64_t>();
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int64_t, %i>(): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, ms_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_i64);
      if (num == std::numeric_limits<int64_t>::lowest() + (1ll << 22)) num = -(1ll << 22) - 1ll;
      if (num == (1ll << 22)) num = std::numeric_limits<int64_t>::max() - (1ll << 22) - 1ll;
      if (num == std::numeric_limits<int64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i64_Y for num on the ranges
   * [-9223372036854775808, -9223372036850581504]
   * [-4194304, 4194304]
   * [9223372036850581503, 9223372036854775807]
   * and Y on [1, 62].
   */
  for (int8_t shift = 1; shift <= 62; shift++)
  {
    std::printf("testing multshiftround_i64_%i()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    int64_t num = std::numeric_limits<int64_t>::lowest();
    while (true)
    {
      int64_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_i64_1(num, mul_i64); break;
        case  2: ms_res = multshiftround_i64_2(num, mul_i64); break;
        case  3: ms_res = multshiftround_i64_3(num, mul_i64); break;
        case  4: ms_res = multshiftround_i64_4(num, mul_i64); break;
        case  5: ms_res = multshiftround_i64_5(num, mul_i64); break;
        case  6: ms_res = multshiftround_i64_6(num, mul_i64); break;
        case  7: ms_res = multshiftround_i64_7(num, mul_i64); break;
        case  8: ms_res = multshiftround_i64_8(num, mul_i64); break;
        case  9: ms_res = multshiftround_i64_9(num, mul_i64); break;
        case 10: ms_res = multshiftround_i64_10(num, mul_i64); break;
        case 11: ms_res = multshiftround_i64_11(num, mul_i64); break;
        case 12: ms_res = multshiftround_i64_12(num, mul_i64); break;
        case 13: ms_res = multshiftround_i64_13(num, mul_i64); break;
        case 14: ms_res = multshiftround_i64_14(num, mul_i64); break;
        case 15: ms_res = multshiftround_i64_15(num, mul_i64); break;
        case 16: ms_res = multshiftround_i64_16(num, mul_i64); break;
        case 17: ms_res = multshiftround_i64_17(num, mul_i64); break;
        case 18: ms_res = multshiftround_i64_18(num, mul_i64); break;
        case 19: ms_res = multshiftround_i64_19(num, mul_i64); break;
        case 20: ms_res = multshiftround_i64_20(num, mul_i64); break;
        case 21: ms_res = multshiftround_i64_21(num, mul_i64); break;
        case 22: ms_res = multshiftround_i64_22(num, mul_i64); break;
        case 23: ms_res = multshiftround_i64_23(num, mul_i64); break;
        case 24: ms_res = multshiftround_i64_24(num, mul_i64); break;
        case 25: ms_res = multshiftround_i64_25(num, mul_i64); break;
        case 26: ms_res = multshiftround_i64_26(num, mul_i64); break;
        case 27: ms_res = multshiftround_i64_27(num, mul_i64); break;
        case 28: ms_res = multshiftround_i64_28(num, mul_i64); break;
        case 29: ms_res = multshiftround_i64_29(num, mul_i64); break;
        case 30: ms_res = multshiftround_i64_30(num, mul_i64); break;
        case 31: ms_res = multshiftround_i64_31(num, mul_i64); break;
        case 32: ms_res = multshiftround_i64_32(num, mul_i64); break;
        case 33: ms_res = multshiftround_i64_33(num, mul_i64); break;
        case 34: ms_res = multshiftround_i64_34(num, mul_i64); break;
        case 35: ms_res = multshiftround_i64_35(num, mul_i64); break;
        case 36: ms_res = multshiftround_i64_36(num, mul_i64); break;
        case 37: ms_res = multshiftround_i64_37(num, mul_i64); break;
        case 38: ms_res = multshiftround_i64_38(num, mul_i64); break;
        case 39: ms_res = multshiftround_i64_39(num, mul_i64); break;
        case 40: ms_res = multshiftround_i64_40(num, mul_i64); break;
        case 41: ms_res = multshiftround_i64_41(num, mul_i64); break;
        case 42: ms_res = multshiftround_i64_42(num, mul_i64); break;
        case 43: ms_res = multshiftround_i64_43(num, mul_i64); break;
        case 44: ms_res = multshiftround_i64_44(num, mul_i64); break;
        case 45: ms_res = multshiftround_i64_45(num, mul_i64); break;
        case 46: ms_res = multshiftround_i64_46(num, mul_i64); break;
        case 47: ms_res = multshiftround_i64_47(num, mul_i64); break;
        case 48: ms_res = multshiftround_i64_48(num, mul_i64); break;
        case 49: ms_res = multshiftround_i64_49(num, mul_i64); break;
        case 50: ms_res = multshiftround_i64_50(num, mul_i64); break;
        case 51: ms_res = multshiftround_i64_51(num, mul_i64); break;
        case 52: ms_res = multshiftround_i64_52(num, mul_i64); break;
        case 53: ms_res = multshiftround_i64_53(num, mul_i64); break;
        case 54: ms_res = multshiftround_i64_54(num, mul_i64); break;
        case 55: ms_res = multshiftround_i64_55(num, mul_i64); break;
        case 56: ms_res = multshiftround_i64_56(num, mul_i64); break;
        case 57: ms_res = multshiftround_i64_57(num, mul_i64); break;
        case 58: ms_res = multshiftround_i64_58(num, mul_i64); break;
        case 59: ms_res = multshiftround_i64_59(num, mul_i64); break;
        case 60: ms_res = multshiftround_i64_60(num, mul_i64); break;
        case 61: ms_res = multshiftround_i64_61(num, mul_i64); break;
        case 62: ms_res = multshiftround_i64_62(num, mul_i64); break;
        default: std::printf("ERROR: invalid shift int64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      int64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<int64_t>();
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i64_%i(): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, ms_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_i64);
      if (num == std::numeric_limits<int64_t>::lowest() + (1ll << 22)) num = -(1ll << 22) - 1ll;
      if (num == (1ll << 22)) num = std::numeric_limits<int64_t>::max() - (1ll << 22) - 1ll;
      if (num == std::numeric_limits<int64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test int64_t shiftround for num on the ranges
   * [-9223372036854775808, -9223372036850581504]
   * [-4194304, 4194304]
   * [9223372036850581503, 9223372036854775807]
   * and shift on [1, 62].
   */
  for (int8_t shift = 1; shift <= 62; shift++)
  {
    std::printf("testing shiftround<int64_t, %i>()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    int64_t num = std::numeric_limits<int64_t>::lowest();
    while (true)
    {
      int64_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<int64_t,  1>(num); break;
        case  2: s_res = shiftround<int64_t,  2>(num); break;
        case  3: s_res = shiftround<int64_t,  3>(num); break;
        case  4: s_res = shiftround<int64_t,  4>(num); break;
        case  5: s_res = shiftround<int64_t,  5>(num); break;
        case  6: s_res = shiftround<int64_t,  6>(num); break;
        case  7: s_res = shiftround<int64_t,  7>(num); break;
        case  8: s_res = shiftround<int64_t,  8>(num); break;
        case  9: s_res = shiftround<int64_t,  9>(num); break;
        case 10: s_res = shiftround<int64_t, 10>(num); break;
        case 11: s_res = shiftround<int64_t, 11>(num); break;
        case 12: s_res = shiftround<int64_t, 12>(num); break;
        case 13: s_res = shiftround<int64_t, 13>(num); break;
        case 14: s_res = shiftround<int64_t, 14>(num); break;
        case 15: s_res = shiftround<int64_t, 15>(num); break;
        case 16: s_res = shiftround<int64_t, 16>(num); break;
        case 17: s_res = shiftround<int64_t, 17>(num); break;
        case 18: s_res = shiftround<int64_t, 18>(num); break;
        case 19: s_res = shiftround<int64_t, 19>(num); break;
        case 20: s_res = shiftround<int64_t, 20>(num); break;
        case 21: s_res = shiftround<int64_t, 21>(num); break;
        case 22: s_res = shiftround<int64_t, 22>(num); break;
        case 23: s_res = shiftround<int64_t, 23>(num); break;
        case 24: s_res = shiftround<int64_t, 24>(num); break;
        case 25: s_res = shiftround<int64_t, 25>(num); break;
        case 26: s_res = shiftround<int64_t, 26>(num); break;
        case 27: s_res = shiftround<int64_t, 27>(num); break;
        case 28: s_res = shiftround<int64_t, 28>(num); break;
        case 29: s_res = shiftround<int64_t, 29>(num); break;
        case 30: s_res = shiftround<int64_t, 30>(num); break;
        case 31: s_res = shiftround<int64_t, 31>(num); break;
        case 32: s_res = shiftround<int64_t, 32>(num); break;
        case 33: s_res = shiftround<int64_t, 33>(num); break;
        case 34: s_res = shiftround<int64_t, 34>(num); break;
        case 35: s_res = shiftround<int64_t, 35>(num); break;
        case 36: s_res = shiftround<int64_t, 36>(num); break;
        case 37: s_res = shiftround<int64_t, 37>(num); break;
        case 38: s_res = shiftround<int64_t, 38>(num); break;
        case 39: s_res = shiftround<int64_t, 39>(num); break;
        case 40: s_res = shiftround<int64_t, 40>(num); break;
        case 41: s_res = shiftround<int64_t, 41>(num); break;
        case 42: s_res = shiftround<int64_t, 42>(num); break;
        case 43: s_res = shiftround<int64_t, 43>(num); break;
        case 44: s_res = shiftround<int64_t, 44>(num); break;
        case 45: s_res = shiftround<int64_t, 45>(num); break;
        case 46: s_res = shiftround<int64_t, 46>(num); break;
        case 47: s_res = shiftround<int64_t, 47>(num); break;
        case 48: s_res = shiftround<int64_t, 48>(num); break;
        case 49: s_res = shiftround<int64_t, 49>(num); break;
        case 50: s_res = shiftround<int64_t, 50>(num); break;
        case 51: s_res = shiftround<int64_t, 51>(num); break;
        case 52: s_res = shiftround<int64_t, 52>(num); break;
        case 53: s_res = shiftround<int64_t, 53>(num); break;
        case 54: s_res = shiftround<int64_t, 54>(num); break;
        case 55: s_res = shiftround<int64_t, 55>(num); break;
        case 56: s_res = shiftround<int64_t, 56>(num); break;
        case 57: s_res = shiftround<int64_t, 57>(num); break;
        case 58: s_res = shiftround<int64_t, 58>(num); break;
        case 59: s_res = shiftround<int64_t, 59>(num); break;
        case 60: s_res = shiftround<int64_t, 60>(num); break;
        case 61: s_res = shiftround<int64_t, 61>(num); break;
        case 62: s_res = shiftround<int64_t, 62>(num); break;
        default: std::printf("ERROR: invalid shift int64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      int64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<int64_t>();
      if (s_res != dbl_res) std::printf("ERROR: shiftround<int64_t, %i>(): s_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, s_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_i64);
      if (num == std::numeric_limits<int64_t>::lowest() + (1ll << 22)) num = -(1ll << 22) - 1ll;
      if (num == (1ll << 22)) num = std::numeric_limits<int64_t>::max() - (1ll << 22) - 1ll;
      if (num == std::numeric_limits<int64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i64_Y for num on the ranges
   * [-9223372036854775808, -9223372036850581504]
   * [-4194304, 4194304]
   * [9223372036850581503, 9223372036854775807]
   * and Y on [1, 62].
   */
  for (int8_t shift = 1; shift <= 62; shift++)
  {
    std::printf("testing shiftround_i64_%i()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    int64_t num = std::numeric_limits<int64_t>::lowest();
    while (true)
    {
      int64_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_i64_1(num); break;
        case  2: s_res = shiftround_i64_2(num); break;
        case  3: s_res = shiftround_i64_3(num); break;
        case  4: s_res = shiftround_i64_4(num); break;
        case  5: s_res = shiftround_i64_5(num); break;
        case  6: s_res = shiftround_i64_6(num); break;
        case  7: s_res = shiftround_i64_7(num); break;
        case  8: s_res = shiftround_i64_8(num); break;
        case  9: s_res = shiftround_i64_9(num); break;
        case 10: s_res = shiftround_i64_10(num); break;
        case 11: s_res = shiftround_i64_11(num); break;
        case 12: s_res = shiftround_i64_12(num); break;
        case 13: s_res = shiftround_i64_13(num); break;
        case 14: s_res = shiftround_i64_14(num); break;
        case 15: s_res = shiftround_i64_15(num); break;
        case 16: s_res = shiftround_i64_16(num); break;
        case 17: s_res = shiftround_i64_17(num); break;
        case 18: s_res = shiftround_i64_18(num); break;
        case 19: s_res = shiftround_i64_19(num); break;
        case 20: s_res = shiftround_i64_20(num); break;
        case 21: s_res = shiftround_i64_21(num); break;
        case 22: s_res = shiftround_i64_22(num); break;
        case 23: s_res = shiftround_i64_23(num); break;
        case 24: s_res = shiftround_i64_24(num); break;
        case 25: s_res = shiftround_i64_25(num); break;
        case 26: s_res = shiftround_i64_26(num); break;
        case 27: s_res = shiftround_i64_27(num); break;
        case 28: s_res = shiftround_i64_28(num); break;
        case 29: s_res = shiftround_i64_29(num); break;
        case 30: s_res = shiftround_i64_30(num); break;
        case 31: s_res = shiftround_i64_31(num); break;
        case 32: s_res = shiftround_i64_32(num); break;
        case 33: s_res = shiftround_i64_33(num); break;
        case 34: s_res = shiftround_i64_34(num); break;
        case 35: s_res = shiftround_i64_35(num); break;
        case 36: s_res = shiftround_i64_36(num); break;
        case 37: s_res = shiftround_i64_37(num); break;
        case 38: s_res = shiftround_i64_38(num); break;
        case 39: s_res = shiftround_i64_39(num); break;
        case 40: s_res = shiftround_i64_40(num); break;
        case 41: s_res = shiftround_i64_41(num); break;
        case 42: s_res = shiftround_i64_42(num); break;
        case 43: s_res = shiftround_i64_43(num); break;
        case 44: s_res = shiftround_i64_44(num); break;
        case 45: s_res = shiftround_i64_45(num); break;
        case 46: s_res = shiftround_i64_46(num); break;
        case 47: s_res = shiftround_i64_47(num); break;
        case 48: s_res = shiftround_i64_48(num); break;
        case 49: s_res = shiftround_i64_49(num); break;
        case 50: s_res = shiftround_i64_50(num); break;
        case 51: s_res = shiftround_i64_51(num); break;
        case 52: s_res = shiftround_i64_52(num); break;
        case 53: s_res = shiftround_i64_53(num); break;
        case 54: s_res = shiftround_i64_54(num); break;
        case 55: s_res = shiftround_i64_55(num); break;
        case 56: s_res = shiftround_i64_56(num); break;
        case 57: s_res = shiftround_i64_57(num); break;
        case 58: s_res = shiftround_i64_58(num); break;
        case 59: s_res = shiftround_i64_59(num); break;
        case 60: s_res = shiftround_i64_60(num); break;
        case 61: s_res = shiftround_i64_61(num); break;
        case 62: s_res = shiftround_i64_62(num); break;
        default: std::printf("ERROR: invalid shift int64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      int64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<int64_t>();
      if (s_res != dbl_res) std::printf("ERROR: shiftround_i64_%i(): s_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, s_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_i64);
      if (num == std::numeric_limits<int64_t>::lowest() + (1ll << 22)) num = -(1ll << 22) - 1ll;
      if (num == (1ll << 22)) num = std::numeric_limits<int64_t>::max() - (1ll << 22) - 1ll;
      if (num == std::numeric_limits<int64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t multshiftround for num on [0, 255] and shift on [1, 7].
   */
  for (int8_t shift = 1; shift <= 7; shift++)
  {
    std::printf("testing multshiftround<uint8_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    while (true)
    {
      uint8_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<uint8_t, 1>(num, mul_u8); break;
        case  2: ms_res = multshiftround<uint8_t, 2>(num, mul_u8); break;
        case  3: ms_res = multshiftround<uint8_t, 3>(num, mul_u8); break;
        case  4: ms_res = multshiftround<uint8_t, 4>(num, mul_u8); break;
        case  5: ms_res = multshiftround<uint8_t, 5>(num, mul_u8); break;
        case  6: ms_res = multshiftround<uint8_t, 6>(num, mul_u8); break;
        case  7: ms_res = multshiftround<uint8_t, 7>(num, mul_u8); break;
        default: std::printf("ERROR: invalid shift uint8_t\n"); return 0; break;
      }

      uint8_t dbl_res = static_cast<uint8_t>(std::round((static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, %i>(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u8_Y for num on [0, 255] and Y on [1, 7].
   */
  for (int8_t shift = 1; shift <= 7; shift++)
  {
    std::printf("testing multshiftround_u8_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    while (true)
    {
      uint8_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_u8_1(num, mul_u8); break;
        case  2: ms_res = multshiftround_u8_2(num, mul_u8); break;
        case  3: ms_res = multshiftround_u8_3(num, mul_u8); break;
        case  4: ms_res = multshiftround_u8_4(num, mul_u8); break;
        case  5: ms_res = multshiftround_u8_5(num, mul_u8); break;
        case  6: ms_res = multshiftround_u8_6(num, mul_u8); break;
        case  7: ms_res = multshiftround_u8_7(num, mul_u8); break;
        default: std::printf("ERROR: invalid shift uint8_t\n"); return 0; break;
      }

      uint8_t dbl_res = static_cast<uint8_t>(std::round((static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_%i(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t shiftround for num on [0, 255] and shift on [1, 7].
   */
  for (int8_t shift = 1; shift <= 7; shift++)
  {
    std::printf("testing shiftround<uint8_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    while (true)
    {
      uint8_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<uint8_t, 1>(num); break;
        case  2: s_res = shiftround<uint8_t, 2>(num); break;
        case  3: s_res = shiftround<uint8_t, 3>(num); break;
        case  4: s_res = shiftround<uint8_t, 4>(num); break;
        case  5: s_res = shiftround<uint8_t, 5>(num); break;
        case  6: s_res = shiftround<uint8_t, 6>(num); break;
        case  7: s_res = shiftround<uint8_t, 7>(num); break;
        default: std::printf("ERROR: invalid shift uint8_t\n"); return 0; break;
      }

      uint8_t dbl_res = static_cast<uint8_t>(std::round((static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, %i>(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u8_Y for num on [0, 255] and Y on [1, 7].
   */
  for (int8_t shift = 1; shift <= 7; shift++)
  {
    std::printf("testing shiftround_u8_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    while (true)
    {
      uint8_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_u8_1(num); break;
        case  2: s_res = shiftround_u8_2(num); break;
        case  3: s_res = shiftround_u8_3(num); break;
        case  4: s_res = shiftround_u8_4(num); break;
        case  5: s_res = shiftround_u8_5(num); break;
        case  6: s_res = shiftround_u8_6(num); break;
        case  7: s_res = shiftround_u8_7(num); break;
        default: std::printf("ERROR: invalid shift uint8_t\n"); return 0; break;
      }

      uint8_t dbl_res = static_cast<uint8_t>(std::round((static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_u8_%i(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u8) / dbl_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t multshiftround for num on [0, 65535] and shift on [1, 15].
   */
  for (int8_t shift = 1; shift <= 15; shift++)
  {
    std::printf("testing multshiftround<uint16_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    while (true)
    {
      uint16_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<uint16_t,  1>(num, mul_u16); break;
        case  2: ms_res = multshiftround<uint16_t,  2>(num, mul_u16); break;
        case  3: ms_res = multshiftround<uint16_t,  3>(num, mul_u16); break;
        case  4: ms_res = multshiftround<uint16_t,  4>(num, mul_u16); break;
        case  5: ms_res = multshiftround<uint16_t,  5>(num, mul_u16); break;
        case  6: ms_res = multshiftround<uint16_t,  6>(num, mul_u16); break;
        case  7: ms_res = multshiftround<uint16_t,  7>(num, mul_u16); break;
        case  8: ms_res = multshiftround<uint16_t,  8>(num, mul_u16); break;
        case  9: ms_res = multshiftround<uint16_t,  9>(num, mul_u16); break;
        case 10: ms_res = multshiftround<uint16_t, 10>(num, mul_u16); break;
        case 11: ms_res = multshiftround<uint16_t, 11>(num, mul_u16); break;
        case 12: ms_res = multshiftround<uint16_t, 12>(num, mul_u16); break;
        case 13: ms_res = multshiftround<uint16_t, 13>(num, mul_u16); break;
        case 14: ms_res = multshiftround<uint16_t, 14>(num, mul_u16); break;
        case 15: ms_res = multshiftround<uint16_t, 15>(num, mul_u16); break;
        default: std::printf("ERROR: invalid shift uint16_t\n"); return 0; break;
      }

      uint16_t dbl_res = static_cast<uint16_t>(std::round((static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, %i>(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u16_Y for num on [0, 65535] and Y on [1, 15].
   */
  for (int8_t shift = 1; shift <= 15; shift++)
  {
    std::printf("testing multshiftround_u16_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    while (true)
    {
      uint16_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_u16_1(num, mul_u16); break;
        case  2: ms_res = multshiftround_u16_2(num, mul_u16); break;
        case  3: ms_res = multshiftround_u16_3(num, mul_u16); break;
        case  4: ms_res = multshiftround_u16_4(num, mul_u16); break;
        case  5: ms_res = multshiftround_u16_5(num, mul_u16); break;
        case  6: ms_res = multshiftround_u16_6(num, mul_u16); break;
        case  7: ms_res = multshiftround_u16_7(num, mul_u16); break;
        case  8: ms_res = multshiftround_u16_8(num, mul_u16); break;
        case  9: ms_res = multshiftround_u16_9(num, mul_u16); break;
        case 10: ms_res = multshiftround_u16_10(num, mul_u16); break;
        case 11: ms_res = multshiftround_u16_11(num, mul_u16); break;
        case 12: ms_res = multshiftround_u16_12(num, mul_u16); break;
        case 13: ms_res = multshiftround_u16_13(num, mul_u16); break;
        case 14: ms_res = multshiftround_u16_14(num, mul_u16); break;
        case 15: ms_res = multshiftround_u16_15(num, mul_u16); break;
        default: std::printf("ERROR: invalid shift uint16_t\n"); return 0; break;
      }

      uint16_t dbl_res = static_cast<uint16_t>(std::round((static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_%i(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t shiftround for num on [0, 65535] and shift on [1, 15].
   */
  for (int8_t shift = 1; shift <= 15; shift++)
  {
    std::printf("testing shiftround<uint16_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    while (true)
    {
      uint16_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<uint16_t,  1>(num); break;
        case  2: s_res = shiftround<uint16_t,  2>(num); break;
        case  3: s_res = shiftround<uint16_t,  3>(num); break;
        case  4: s_res = shiftround<uint16_t,  4>(num); break;
        case  5: s_res = shiftround<uint16_t,  5>(num); break;
        case  6: s_res = shiftround<uint16_t,  6>(num); break;
        case  7: s_res = shiftround<uint16_t,  7>(num); break;
        case  8: s_res = shiftround<uint16_t,  8>(num); break;
        case  9: s_res = shiftround<uint16_t,  9>(num); break;
        case 10: s_res = shiftround<uint16_t, 10>(num); break;
        case 11: s_res = shiftround<uint16_t, 11>(num); break;
        case 12: s_res = shiftround<uint16_t, 12>(num); break;
        case 13: s_res = shiftround<uint16_t, 13>(num); break;
        case 14: s_res = shiftround<uint16_t, 14>(num); break;
        case 15: s_res = shiftround<uint16_t, 15>(num); break;
        default: std::printf("ERROR: invalid shift uint16_t\n"); return 0; break;
      }

      uint16_t dbl_res = static_cast<uint16_t>(std::round((static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, %i>(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u16_Y for num on [0, 65535] and num on [1, 15].
   */
  for (int8_t shift = 1; shift <= 15; shift++)
  {
    std::printf("testing shiftround_u16_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    while (true)
    {
      uint16_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_u16_1(num); break;
        case  2: s_res = shiftround_u16_2(num); break;
        case  3: s_res = shiftround_u16_3(num); break;
        case  4: s_res = shiftround_u16_4(num); break;
        case  5: s_res = shiftround_u16_5(num); break;
        case  6: s_res = shiftround_u16_6(num); break;
        case  7: s_res = shiftround_u16_7(num); break;
        case  8: s_res = shiftround_u16_8(num); break;
        case  9: s_res = shiftround_u16_9(num); break;
        case 10: s_res = shiftround_u16_10(num); break;
        case 11: s_res = shiftround_u16_11(num); break;
        case 12: s_res = shiftround_u16_12(num); break;
        case 13: s_res = shiftround_u16_13(num); break;
        case 14: s_res = shiftround_u16_14(num); break;
        case 15: s_res = shiftround_u16_15(num); break;
        default: std::printf("ERROR: invalid shift uint16_t\n"); return 0; break;
      }

      uint16_t dbl_res = static_cast<uint16_t>(std::round((static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_%i(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u16) / dbl_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint32_t multshiftround for num on [0, 4294967295] and shift on [1, 31].
   */
  for (int8_t shift = 1; shift <= 31; shift++)
  {
    std::printf("testing multshiftround<uint32_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint32_t num = std::numeric_limits<uint32_t>::lowest();
    while (true)
    {
      uint32_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<uint32_t,  1>(num, mul_u32); break;
        case  2: ms_res = multshiftround<uint32_t,  2>(num, mul_u32); break;
        case  3: ms_res = multshiftround<uint32_t,  3>(num, mul_u32); break;
        case  4: ms_res = multshiftround<uint32_t,  4>(num, mul_u32); break;
        case  5: ms_res = multshiftround<uint32_t,  5>(num, mul_u32); break;
        case  6: ms_res = multshiftround<uint32_t,  6>(num, mul_u32); break;
        case  7: ms_res = multshiftround<uint32_t,  7>(num, mul_u32); break;
        case  8: ms_res = multshiftround<uint32_t,  8>(num, mul_u32); break;
        case  9: ms_res = multshiftround<uint32_t,  9>(num, mul_u32); break;
        case 10: ms_res = multshiftround<uint32_t, 10>(num, mul_u32); break;
        case 11: ms_res = multshiftround<uint32_t, 11>(num, mul_u32); break;
        case 12: ms_res = multshiftround<uint32_t, 12>(num, mul_u32); break;
        case 13: ms_res = multshiftround<uint32_t, 13>(num, mul_u32); break;
        case 14: ms_res = multshiftround<uint32_t, 14>(num, mul_u32); break;
        case 15: ms_res = multshiftround<uint32_t, 15>(num, mul_u32); break;
        case 16: ms_res = multshiftround<uint32_t, 16>(num, mul_u32); break;
        case 17: ms_res = multshiftround<uint32_t, 17>(num, mul_u32); break;
        case 18: ms_res = multshiftround<uint32_t, 18>(num, mul_u32); break;
        case 19: ms_res = multshiftround<uint32_t, 19>(num, mul_u32); break;
        case 20: ms_res = multshiftround<uint32_t, 20>(num, mul_u32); break;
        case 21: ms_res = multshiftround<uint32_t, 21>(num, mul_u32); break;
        case 22: ms_res = multshiftround<uint32_t, 22>(num, mul_u32); break;
        case 23: ms_res = multshiftround<uint32_t, 23>(num, mul_u32); break;
        case 24: ms_res = multshiftround<uint32_t, 24>(num, mul_u32); break;
        case 25: ms_res = multshiftround<uint32_t, 25>(num, mul_u32); break;
        case 26: ms_res = multshiftround<uint32_t, 26>(num, mul_u32); break;
        case 27: ms_res = multshiftround<uint32_t, 27>(num, mul_u32); break;
        case 28: ms_res = multshiftround<uint32_t, 28>(num, mul_u32); break;
        case 29: ms_res = multshiftround<uint32_t, 29>(num, mul_u32); break;
        case 30: ms_res = multshiftround<uint32_t, 30>(num, mul_u32); break;
        case 31: ms_res = multshiftround<uint32_t, 31>(num, mul_u32); break;
        default: std::printf("ERROR: invalid shift uint32_t\n"); return 0; break;
      }

      uint32_t dbl_res = static_cast<uint32_t>(std::round((static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint32_t, %i>(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp, num, mul_u32);
      if (num == std::numeric_limits<uint32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u32_Y for num on [0, 4294967295] and Y on [1, 31].
   */
  for (int8_t shift = 1; shift <= 31; shift++)
  {
    std::printf("testing multshiftround_u32_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint32_t num = std::numeric_limits<uint32_t>::lowest();
    while (true)
    {
      uint32_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_u32_1(num, mul_u32); break;
        case  2: ms_res = multshiftround_u32_2(num, mul_u32); break;
        case  3: ms_res = multshiftround_u32_3(num, mul_u32); break;
        case  4: ms_res = multshiftround_u32_4(num, mul_u32); break;
        case  5: ms_res = multshiftround_u32_5(num, mul_u32); break;
        case  6: ms_res = multshiftround_u32_6(num, mul_u32); break;
        case  7: ms_res = multshiftround_u32_7(num, mul_u32); break;
        case  8: ms_res = multshiftround_u32_8(num, mul_u32); break;
        case  9: ms_res = multshiftround_u32_9(num, mul_u32); break;
        case 10: ms_res = multshiftround_u32_10(num, mul_u32); break;
        case 11: ms_res = multshiftround_u32_11(num, mul_u32); break;
        case 12: ms_res = multshiftround_u32_12(num, mul_u32); break;
        case 13: ms_res = multshiftround_u32_13(num, mul_u32); break;
        case 14: ms_res = multshiftround_u32_14(num, mul_u32); break;
        case 15: ms_res = multshiftround_u32_15(num, mul_u32); break;
        case 16: ms_res = multshiftround_u32_16(num, mul_u32); break;
        case 17: ms_res = multshiftround_u32_17(num, mul_u32); break;
        case 18: ms_res = multshiftround_u32_18(num, mul_u32); break;
        case 19: ms_res = multshiftround_u32_19(num, mul_u32); break;
        case 20: ms_res = multshiftround_u32_20(num, mul_u32); break;
        case 21: ms_res = multshiftround_u32_21(num, mul_u32); break;
        case 22: ms_res = multshiftround_u32_22(num, mul_u32); break;
        case 23: ms_res = multshiftround_u32_23(num, mul_u32); break;
        case 24: ms_res = multshiftround_u32_24(num, mul_u32); break;
        case 25: ms_res = multshiftround_u32_25(num, mul_u32); break;
        case 26: ms_res = multshiftround_u32_26(num, mul_u32); break;
        case 27: ms_res = multshiftround_u32_27(num, mul_u32); break;
        case 28: ms_res = multshiftround_u32_28(num, mul_u32); break;
        case 29: ms_res = multshiftround_u32_29(num, mul_u32); break;
        case 30: ms_res = multshiftround_u32_30(num, mul_u32); break;
        case 31: ms_res = multshiftround_u32_31(num, mul_u32); break;
        default: std::printf("ERROR: invalid shift uint32_t\n"); return 0; break;
      }

      uint32_t dbl_res = static_cast<uint32_t>(std::round((static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u32_%i(): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, (static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp, num, mul_u32);
      if (num == std::numeric_limits<uint32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint32_t shiftround for num on [0, 4294967295] and shift on [1, 31].
   */
  for (int8_t shift = 1; shift <= 31; shift++)
  {
    std::printf("testing shiftround<uint32_t, %i>()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint32_t num = std::numeric_limits<uint32_t>::lowest();
    while (true)
    {
      uint32_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<uint32_t,  1>(num); break;
        case  2: s_res = shiftround<uint32_t,  2>(num); break;
        case  3: s_res = shiftround<uint32_t,  3>(num); break;
        case  4: s_res = shiftround<uint32_t,  4>(num); break;
        case  5: s_res = shiftround<uint32_t,  5>(num); break;
        case  6: s_res = shiftround<uint32_t,  6>(num); break;
        case  7: s_res = shiftround<uint32_t,  7>(num); break;
        case  8: s_res = shiftround<uint32_t,  8>(num); break;
        case  9: s_res = shiftround<uint32_t,  9>(num); break;
        case 10: s_res = shiftround<uint32_t, 10>(num); break;
        case 11: s_res = shiftround<uint32_t, 11>(num); break;
        case 12: s_res = shiftround<uint32_t, 12>(num); break;
        case 13: s_res = shiftround<uint32_t, 13>(num); break;
        case 14: s_res = shiftround<uint32_t, 14>(num); break;
        case 15: s_res = shiftround<uint32_t, 15>(num); break;
        case 16: s_res = shiftround<uint32_t, 16>(num); break;
        case 17: s_res = shiftround<uint32_t, 17>(num); break;
        case 18: s_res = shiftround<uint32_t, 18>(num); break;
        case 19: s_res = shiftround<uint32_t, 19>(num); break;
        case 20: s_res = shiftround<uint32_t, 20>(num); break;
        case 21: s_res = shiftround<uint32_t, 21>(num); break;
        case 22: s_res = shiftround<uint32_t, 22>(num); break;
        case 23: s_res = shiftround<uint32_t, 23>(num); break;
        case 24: s_res = shiftround<uint32_t, 24>(num); break;
        case 25: s_res = shiftround<uint32_t, 25>(num); break;
        case 26: s_res = shiftround<uint32_t, 26>(num); break;
        case 27: s_res = shiftround<uint32_t, 27>(num); break;
        case 28: s_res = shiftround<uint32_t, 28>(num); break;
        case 29: s_res = shiftround<uint32_t, 29>(num); break;
        case 30: s_res = shiftround<uint32_t, 30>(num); break;
        case 31: s_res = shiftround<uint32_t, 31>(num); break;
        default: std::printf("ERROR: invalid shift uint32_t\n"); return 0; break;
      }

      uint32_t dbl_res = static_cast<uint32_t>(std::round((static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<uint32_t, %i>(): s_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp, num, mul_u32);
      if (num == std::numeric_limits<uint32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u32_Y for num on [0, 4294967295] and Y on [1, 31].
   */
  for (int8_t shift = 1; shift <= 31; shift++)
  {
    std::printf("testing shiftround_u32_%i()\n", shift);
    double dbl_twoexp = static_cast<double>(1ull << shift);
    uint32_t num = std::numeric_limits<uint32_t>::lowest();
    while (true)
    {
      uint32_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_u32_1(num); break;
        case  2: s_res = shiftround_u32_2(num); break;
        case  3: s_res = shiftround_u32_3(num); break;
        case  4: s_res = shiftround_u32_4(num); break;
        case  5: s_res = shiftround_u32_5(num); break;
        case  6: s_res = shiftround_u32_6(num); break;
        case  7: s_res = shiftround_u32_7(num); break;
        case  8: s_res = shiftround_u32_8(num); break;
        case  9: s_res = shiftround_u32_9(num); break;
        case 10: s_res = shiftround_u32_10(num); break;
        case 11: s_res = shiftround_u32_11(num); break;
        case 12: s_res = shiftround_u32_12(num); break;
        case 13: s_res = shiftround_u32_13(num); break;
        case 14: s_res = shiftround_u32_14(num); break;
        case 15: s_res = shiftround_u32_15(num); break;
        case 16: s_res = shiftround_u32_16(num); break;
        case 17: s_res = shiftround_u32_17(num); break;
        case 18: s_res = shiftround_u32_18(num); break;
        case 19: s_res = shiftround_u32_19(num); break;
        case 20: s_res = shiftround_u32_20(num); break;
        case 21: s_res = shiftround_u32_21(num); break;
        case 22: s_res = shiftround_u32_22(num); break;
        case 23: s_res = shiftround_u32_23(num); break;
        case 24: s_res = shiftround_u32_24(num); break;
        case 25: s_res = shiftround_u32_25(num); break;
        case 26: s_res = shiftround_u32_26(num); break;
        case 27: s_res = shiftround_u32_27(num); break;
        case 28: s_res = shiftround_u32_28(num); break;
        case 29: s_res = shiftround_u32_29(num); break;
        case 30: s_res = shiftround_u32_30(num); break;
        case 31: s_res = shiftround_u32_31(num); break;
        default: std::printf("ERROR: invalid shift uint32_t\n"); return 0; break;
      }

      uint32_t dbl_res = static_cast<uint32_t>(std::round((static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_u32_%i(): s_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, s_res, dbl_res, (static_cast<double>(num) * dbl_mul_u32) / dbl_twoexp, num, mul_u32);
      if (num == std::numeric_limits<uint32_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint64_t multshiftround for num on the ranges
   * [0, 8388608] [18446744073701163007, 18446744073709551615]
   * and shift on [1, 63].
   */
  for (int8_t shift = 1; shift <= 63; shift++)
  {
    std::printf("testing multshiftround<uint64_t, %i>()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    uint64_t num = std::numeric_limits<uint64_t>::lowest();
    while (true)
    {
      uint64_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround<uint64_t,  1>(num, mul_u64); break;
        case  2: ms_res = multshiftround<uint64_t,  2>(num, mul_u64); break;
        case  3: ms_res = multshiftround<uint64_t,  3>(num, mul_u64); break;
        case  4: ms_res = multshiftround<uint64_t,  4>(num, mul_u64); break;
        case  5: ms_res = multshiftround<uint64_t,  5>(num, mul_u64); break;
        case  6: ms_res = multshiftround<uint64_t,  6>(num, mul_u64); break;
        case  7: ms_res = multshiftround<uint64_t,  7>(num, mul_u64); break;
        case  8: ms_res = multshiftround<uint64_t,  8>(num, mul_u64); break;
        case  9: ms_res = multshiftround<uint64_t,  9>(num, mul_u64); break;
        case 10: ms_res = multshiftround<uint64_t, 10>(num, mul_u64); break;
        case 11: ms_res = multshiftround<uint64_t, 11>(num, mul_u64); break;
        case 12: ms_res = multshiftround<uint64_t, 12>(num, mul_u64); break;
        case 13: ms_res = multshiftround<uint64_t, 13>(num, mul_u64); break;
        case 14: ms_res = multshiftround<uint64_t, 14>(num, mul_u64); break;
        case 15: ms_res = multshiftround<uint64_t, 15>(num, mul_u64); break;
        case 16: ms_res = multshiftround<uint64_t, 16>(num, mul_u64); break;
        case 17: ms_res = multshiftround<uint64_t, 17>(num, mul_u64); break;
        case 18: ms_res = multshiftround<uint64_t, 18>(num, mul_u64); break;
        case 19: ms_res = multshiftround<uint64_t, 19>(num, mul_u64); break;
        case 20: ms_res = multshiftround<uint64_t, 20>(num, mul_u64); break;
        case 21: ms_res = multshiftround<uint64_t, 21>(num, mul_u64); break;
        case 22: ms_res = multshiftround<uint64_t, 22>(num, mul_u64); break;
        case 23: ms_res = multshiftround<uint64_t, 23>(num, mul_u64); break;
        case 24: ms_res = multshiftround<uint64_t, 24>(num, mul_u64); break;
        case 25: ms_res = multshiftround<uint64_t, 25>(num, mul_u64); break;
        case 26: ms_res = multshiftround<uint64_t, 26>(num, mul_u64); break;
        case 27: ms_res = multshiftround<uint64_t, 27>(num, mul_u64); break;
        case 28: ms_res = multshiftround<uint64_t, 28>(num, mul_u64); break;
        case 29: ms_res = multshiftround<uint64_t, 29>(num, mul_u64); break;
        case 30: ms_res = multshiftround<uint64_t, 30>(num, mul_u64); break;
        case 31: ms_res = multshiftround<uint64_t, 31>(num, mul_u64); break;
        case 32: ms_res = multshiftround<uint64_t, 32>(num, mul_u64); break;
        case 33: ms_res = multshiftround<uint64_t, 33>(num, mul_u64); break;
        case 34: ms_res = multshiftround<uint64_t, 34>(num, mul_u64); break;
        case 35: ms_res = multshiftround<uint64_t, 35>(num, mul_u64); break;
        case 36: ms_res = multshiftround<uint64_t, 36>(num, mul_u64); break;
        case 37: ms_res = multshiftround<uint64_t, 37>(num, mul_u64); break;
        case 38: ms_res = multshiftround<uint64_t, 38>(num, mul_u64); break;
        case 39: ms_res = multshiftround<uint64_t, 39>(num, mul_u64); break;
        case 40: ms_res = multshiftround<uint64_t, 40>(num, mul_u64); break;
        case 41: ms_res = multshiftround<uint64_t, 41>(num, mul_u64); break;
        case 42: ms_res = multshiftround<uint64_t, 42>(num, mul_u64); break;
        case 43: ms_res = multshiftround<uint64_t, 43>(num, mul_u64); break;
        case 44: ms_res = multshiftround<uint64_t, 44>(num, mul_u64); break;
        case 45: ms_res = multshiftround<uint64_t, 45>(num, mul_u64); break;
        case 46: ms_res = multshiftround<uint64_t, 46>(num, mul_u64); break;
        case 47: ms_res = multshiftround<uint64_t, 47>(num, mul_u64); break;
        case 48: ms_res = multshiftround<uint64_t, 48>(num, mul_u64); break;
        case 49: ms_res = multshiftround<uint64_t, 49>(num, mul_u64); break;
        case 50: ms_res = multshiftround<uint64_t, 50>(num, mul_u64); break;
        case 51: ms_res = multshiftround<uint64_t, 51>(num, mul_u64); break;
        case 52: ms_res = multshiftround<uint64_t, 52>(num, mul_u64); break;
        case 53: ms_res = multshiftround<uint64_t, 53>(num, mul_u64); break;
        case 54: ms_res = multshiftround<uint64_t, 54>(num, mul_u64); break;
        case 55: ms_res = multshiftround<uint64_t, 55>(num, mul_u64); break;
        case 56: ms_res = multshiftround<uint64_t, 56>(num, mul_u64); break;
        case 57: ms_res = multshiftround<uint64_t, 57>(num, mul_u64); break;
        case 58: ms_res = multshiftround<uint64_t, 58>(num, mul_u64); break;
        case 59: ms_res = multshiftround<uint64_t, 59>(num, mul_u64); break;
        case 60: ms_res = multshiftround<uint64_t, 60>(num, mul_u64); break;
        case 61: ms_res = multshiftround<uint64_t, 61>(num, mul_u64); break;
        case 62: ms_res = multshiftround<uint64_t, 62>(num, mul_u64); break;
        case 63: ms_res = multshiftround<uint64_t, 63>(num, mul_u64); break;
        default: std::printf("ERROR: invalid shift uint64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      uint64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<uint64_t>();
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint64_t, %i>(): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, ms_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_u64);
      if (num == (1ull << 23)) num = std::numeric_limits<uint64_t>::max() - (1ull << 23) - 1ull;
      if (num == std::numeric_limits<uint64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u64_Y for num on the ranges
   * [0, 8388608] [18446744073701163007, 18446744073709551615]
   * and Y on [1, 63].
   */
  for (int8_t shift = 1; shift <= 63; shift++)
  {
    std::printf("testing multshiftround_u64_%i()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    uint64_t num = std::numeric_limits<uint64_t>::lowest();
    while (true)
    {
      uint64_t ms_res;
      switch (shift)
      {
        case  1: ms_res = multshiftround_u64_1(num, mul_u64); break;
        case  2: ms_res = multshiftround_u64_2(num, mul_u64); break;
        case  3: ms_res = multshiftround_u64_3(num, mul_u64); break;
        case  4: ms_res = multshiftround_u64_4(num, mul_u64); break;
        case  5: ms_res = multshiftround_u64_5(num, mul_u64); break;
        case  6: ms_res = multshiftround_u64_6(num, mul_u64); break;
        case  7: ms_res = multshiftround_u64_7(num, mul_u64); break;
        case  8: ms_res = multshiftround_u64_8(num, mul_u64); break;
        case  9: ms_res = multshiftround_u64_9(num, mul_u64); break;
        case 10: ms_res = multshiftround_u64_10(num, mul_u64); break;
        case 11: ms_res = multshiftround_u64_11(num, mul_u64); break;
        case 12: ms_res = multshiftround_u64_12(num, mul_u64); break;
        case 13: ms_res = multshiftround_u64_13(num, mul_u64); break;
        case 14: ms_res = multshiftround_u64_14(num, mul_u64); break;
        case 15: ms_res = multshiftround_u64_15(num, mul_u64); break;
        case 16: ms_res = multshiftround_u64_16(num, mul_u64); break;
        case 17: ms_res = multshiftround_u64_17(num, mul_u64); break;
        case 18: ms_res = multshiftround_u64_18(num, mul_u64); break;
        case 19: ms_res = multshiftround_u64_19(num, mul_u64); break;
        case 20: ms_res = multshiftround_u64_20(num, mul_u64); break;
        case 21: ms_res = multshiftround_u64_21(num, mul_u64); break;
        case 22: ms_res = multshiftround_u64_22(num, mul_u64); break;
        case 23: ms_res = multshiftround_u64_23(num, mul_u64); break;
        case 24: ms_res = multshiftround_u64_24(num, mul_u64); break;
        case 25: ms_res = multshiftround_u64_25(num, mul_u64); break;
        case 26: ms_res = multshiftround_u64_26(num, mul_u64); break;
        case 27: ms_res = multshiftround_u64_27(num, mul_u64); break;
        case 28: ms_res = multshiftround_u64_28(num, mul_u64); break;
        case 29: ms_res = multshiftround_u64_29(num, mul_u64); break;
        case 30: ms_res = multshiftround_u64_30(num, mul_u64); break;
        case 31: ms_res = multshiftround_u64_31(num, mul_u64); break;
        case 32: ms_res = multshiftround_u64_32(num, mul_u64); break;
        case 33: ms_res = multshiftround_u64_33(num, mul_u64); break;
        case 34: ms_res = multshiftround_u64_34(num, mul_u64); break;
        case 35: ms_res = multshiftround_u64_35(num, mul_u64); break;
        case 36: ms_res = multshiftround_u64_36(num, mul_u64); break;
        case 37: ms_res = multshiftround_u64_37(num, mul_u64); break;
        case 38: ms_res = multshiftround_u64_38(num, mul_u64); break;
        case 39: ms_res = multshiftround_u64_39(num, mul_u64); break;
        case 40: ms_res = multshiftround_u64_40(num, mul_u64); break;
        case 41: ms_res = multshiftround_u64_41(num, mul_u64); break;
        case 42: ms_res = multshiftround_u64_42(num, mul_u64); break;
        case 43: ms_res = multshiftround_u64_43(num, mul_u64); break;
        case 44: ms_res = multshiftround_u64_44(num, mul_u64); break;
        case 45: ms_res = multshiftround_u64_45(num, mul_u64); break;
        case 46: ms_res = multshiftround_u64_46(num, mul_u64); break;
        case 47: ms_res = multshiftround_u64_47(num, mul_u64); break;
        case 48: ms_res = multshiftround_u64_48(num, mul_u64); break;
        case 49: ms_res = multshiftround_u64_49(num, mul_u64); break;
        case 50: ms_res = multshiftround_u64_50(num, mul_u64); break;
        case 51: ms_res = multshiftround_u64_51(num, mul_u64); break;
        case 52: ms_res = multshiftround_u64_52(num, mul_u64); break;
        case 53: ms_res = multshiftround_u64_53(num, mul_u64); break;
        case 54: ms_res = multshiftround_u64_54(num, mul_u64); break;
        case 55: ms_res = multshiftround_u64_55(num, mul_u64); break;
        case 56: ms_res = multshiftround_u64_56(num, mul_u64); break;
        case 57: ms_res = multshiftround_u64_57(num, mul_u64); break;
        case 58: ms_res = multshiftround_u64_58(num, mul_u64); break;
        case 59: ms_res = multshiftround_u64_59(num, mul_u64); break;
        case 60: ms_res = multshiftround_u64_60(num, mul_u64); break;
        case 61: ms_res = multshiftround_u64_61(num, mul_u64); break;
        case 62: ms_res = multshiftround_u64_62(num, mul_u64); break;
        case 63: ms_res = multshiftround_u64_63(num, mul_u64); break;
        default: std::printf("ERROR: invalid shift uint64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      uint64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<uint64_t>();
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u64_%i(): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, ms_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_u64);
      if (num == (1ull << 23)) num = std::numeric_limits<uint64_t>::max() - (1ull << 23) - 1ull;
      if (num == std::numeric_limits<uint64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test uint64_t shiftround for num on the ranges
   * [0, 8388608] [18446744073701163007, 18446744073709551615]
   * and shift on [1, 63].
   */
  for (int8_t shift = 1; shift <= 63; shift++)
  {
    std::printf("testing shiftround<uint64_t, %i>()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    uint64_t num = std::numeric_limits<uint64_t>::lowest();
    while (true)
    {
      uint64_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround<uint64_t,  1>(num); break;
        case  2: s_res = shiftround<uint64_t,  2>(num); break;
        case  3: s_res = shiftround<uint64_t,  3>(num); break;
        case  4: s_res = shiftround<uint64_t,  4>(num); break;
        case  5: s_res = shiftround<uint64_t,  5>(num); break;
        case  6: s_res = shiftround<uint64_t,  6>(num); break;
        case  7: s_res = shiftround<uint64_t,  7>(num); break;
        case  8: s_res = shiftround<uint64_t,  8>(num); break;
        case  9: s_res = shiftround<uint64_t,  9>(num); break;
        case 10: s_res = shiftround<uint64_t, 10>(num); break;
        case 11: s_res = shiftround<uint64_t, 11>(num); break;
        case 12: s_res = shiftround<uint64_t, 12>(num); break;
        case 13: s_res = shiftround<uint64_t, 13>(num); break;
        case 14: s_res = shiftround<uint64_t, 14>(num); break;
        case 15: s_res = shiftround<uint64_t, 15>(num); break;
        case 16: s_res = shiftround<uint64_t, 16>(num); break;
        case 17: s_res = shiftround<uint64_t, 17>(num); break;
        case 18: s_res = shiftround<uint64_t, 18>(num); break;
        case 19: s_res = shiftround<uint64_t, 19>(num); break;
        case 20: s_res = shiftround<uint64_t, 20>(num); break;
        case 21: s_res = shiftround<uint64_t, 21>(num); break;
        case 22: s_res = shiftround<uint64_t, 22>(num); break;
        case 23: s_res = shiftround<uint64_t, 23>(num); break;
        case 24: s_res = shiftround<uint64_t, 24>(num); break;
        case 25: s_res = shiftround<uint64_t, 25>(num); break;
        case 26: s_res = shiftround<uint64_t, 26>(num); break;
        case 27: s_res = shiftround<uint64_t, 27>(num); break;
        case 28: s_res = shiftround<uint64_t, 28>(num); break;
        case 29: s_res = shiftround<uint64_t, 29>(num); break;
        case 30: s_res = shiftround<uint64_t, 30>(num); break;
        case 31: s_res = shiftround<uint64_t, 31>(num); break;
        case 32: s_res = shiftround<uint64_t, 32>(num); break;
        case 33: s_res = shiftround<uint64_t, 33>(num); break;
        case 34: s_res = shiftround<uint64_t, 34>(num); break;
        case 35: s_res = shiftround<uint64_t, 35>(num); break;
        case 36: s_res = shiftround<uint64_t, 36>(num); break;
        case 37: s_res = shiftround<uint64_t, 37>(num); break;
        case 38: s_res = shiftround<uint64_t, 38>(num); break;
        case 39: s_res = shiftround<uint64_t, 39>(num); break;
        case 40: s_res = shiftround<uint64_t, 40>(num); break;
        case 41: s_res = shiftround<uint64_t, 41>(num); break;
        case 42: s_res = shiftround<uint64_t, 42>(num); break;
        case 43: s_res = shiftround<uint64_t, 43>(num); break;
        case 44: s_res = shiftround<uint64_t, 44>(num); break;
        case 45: s_res = shiftround<uint64_t, 45>(num); break;
        case 46: s_res = shiftround<uint64_t, 46>(num); break;
        case 47: s_res = shiftround<uint64_t, 47>(num); break;
        case 48: s_res = shiftround<uint64_t, 48>(num); break;
        case 49: s_res = shiftround<uint64_t, 49>(num); break;
        case 50: s_res = shiftround<uint64_t, 50>(num); break;
        case 51: s_res = shiftround<uint64_t, 51>(num); break;
        case 52: s_res = shiftround<uint64_t, 52>(num); break;
        case 53: s_res = shiftround<uint64_t, 53>(num); break;
        case 54: s_res = shiftround<uint64_t, 54>(num); break;
        case 55: s_res = shiftround<uint64_t, 55>(num); break;
        case 56: s_res = shiftround<uint64_t, 56>(num); break;
        case 57: s_res = shiftround<uint64_t, 57>(num); break;
        case 58: s_res = shiftround<uint64_t, 58>(num); break;
        case 59: s_res = shiftround<uint64_t, 59>(num); break;
        case 60: s_res = shiftround<uint64_t, 60>(num); break;
        case 61: s_res = shiftround<uint64_t, 61>(num); break;
        case 62: s_res = shiftround<uint64_t, 62>(num); break;
        case 63: s_res = shiftround<uint64_t, 63>(num); break;
        default: std::printf("ERROR: invalid shift uint64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      uint64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<uint64_t>();
      if (s_res != dbl_res) std::printf("ERROR: shiftround<uint64_t, %i>(): s_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, s_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_u64);
      if (num == (1ull << 23)) num = std::numeric_limits<uint64_t>::max() - (1ull << 23) - 1ull;
      if (num == std::numeric_limits<uint64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u64_Y for num on the ranges
   * [0, 8388608] [18446744073701163007, 18446744073709551615]
   * and Y on [1, 63].
   */
  for (int8_t shift = 1; shift <= 63; shift++)
  {
    std::printf("testing shiftround_u64_%i()\n", shift);
    cpp_bin_float_80 ldbl_twoexp(1ull << shift);
    uint64_t num = std::numeric_limits<uint64_t>::lowest();
    while (true)
    {
      uint64_t s_res;
      switch (shift)
      {
        case  1: s_res = shiftround_u64_1(num); break;
        case  2: s_res = shiftround_u64_2(num); break;
        case  3: s_res = shiftround_u64_3(num); break;
        case  4: s_res = shiftround_u64_4(num); break;
        case  5: s_res = shiftround_u64_5(num); break;
        case  6: s_res = shiftround_u64_6(num); break;
        case  7: s_res = shiftround_u64_7(num); break;
        case  8: s_res = shiftround_u64_8(num); break;
        case  9: s_res = shiftround_u64_9(num); break;
        case 10: s_res = shiftround_u64_10(num); break;
        case 11: s_res = shiftround_u64_11(num); break;
        case 12: s_res = shiftround_u64_12(num); break;
        case 13: s_res = shiftround_u64_13(num); break;
        case 14: s_res = shiftround_u64_14(num); break;
        case 15: s_res = shiftround_u64_15(num); break;
        case 16: s_res = shiftround_u64_16(num); break;
        case 17: s_res = shiftround_u64_17(num); break;
        case 18: s_res = shiftround_u64_18(num); break;
        case 19: s_res = shiftround_u64_19(num); break;
        case 20: s_res = shiftround_u64_20(num); break;
        case 21: s_res = shiftround_u64_21(num); break;
        case 22: s_res = shiftround_u64_22(num); break;
        case 23: s_res = shiftround_u64_23(num); break;
        case 24: s_res = shiftround_u64_24(num); break;
        case 25: s_res = shiftround_u64_25(num); break;
        case 26: s_res = shiftround_u64_26(num); break;
        case 27: s_res = shiftround_u64_27(num); break;
        case 28: s_res = shiftround_u64_28(num); break;
        case 29: s_res = shiftround_u64_29(num); break;
        case 30: s_res = shiftround_u64_30(num); break;
        case 31: s_res = shiftround_u64_31(num); break;
        case 32: s_res = shiftround_u64_32(num); break;
        case 33: s_res = shiftround_u64_33(num); break;
        case 34: s_res = shiftround_u64_34(num); break;
        case 35: s_res = shiftround_u64_35(num); break;
        case 36: s_res = shiftround_u64_36(num); break;
        case 37: s_res = shiftround_u64_37(num); break;
        case 38: s_res = shiftround_u64_38(num); break;
        case 39: s_res = shiftround_u64_39(num); break;
        case 40: s_res = shiftround_u64_40(num); break;
        case 41: s_res = shiftround_u64_41(num); break;
        case 42: s_res = shiftround_u64_42(num); break;
        case 43: s_res = shiftround_u64_43(num); break;
        case 44: s_res = shiftround_u64_44(num); break;
        case 45: s_res = shiftround_u64_45(num); break;
        case 46: s_res = shiftround_u64_46(num); break;
        case 47: s_res = shiftround_u64_47(num); break;
        case 48: s_res = shiftround_u64_48(num); break;
        case 49: s_res = shiftround_u64_49(num); break;
        case 50: s_res = shiftround_u64_50(num); break;
        case 51: s_res = shiftround_u64_51(num); break;
        case 52: s_res = shiftround_u64_52(num); break;
        case 53: s_res = shiftround_u64_53(num); break;
        case 54: s_res = shiftround_u64_54(num); break;
        case 55: s_res = shiftround_u64_55(num); break;
        case 56: s_res = shiftround_u64_56(num); break;
        case 57: s_res = shiftround_u64_57(num); break;
        case 58: s_res = shiftround_u64_58(num); break;
        case 59: s_res = shiftround_u64_59(num); break;
        case 60: s_res = shiftround_u64_60(num); break;
        case 61: s_res = shiftround_u64_61(num); break;
        case 62: s_res = shiftround_u64_62(num); break;
        case 63: s_res = shiftround_u64_63(num); break;
        default: std::printf("ERROR: invalid shift uint64_t\n"); return 0; break;
      }

      cpp_bin_float_80 ldbl_num(num);
      uint64_t dbl_res = boost::math::round((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<uint64_t>();
      if (s_res != dbl_res) std::printf("ERROR: shiftround_u64_%i(): s_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, s_res, dbl_res, ((ldbl_num * ldbl_mul_i64) / ldbl_twoexp).convert_to<double>(), num, mul_u64);
      if (num == (1ull << 23)) num = std::numeric_limits<uint64_t>::max() - (1ull << 23) - 1ull;
      if (num == std::numeric_limits<uint64_t>::max()) break;
      num++;
    }
  }
  std::printf("\n");

  std::printf("Testing succeeded if there are no errors above.\n\n");
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