/**
 * test_multshiftround_shiftround_run.cpp
 * Unit tests for the multshiftround and shiftround functions that
 * require the shift argument at compile time.
 *
 * Full coverage is provided on the num and shift inputs for the
 * int8_t, int16_t, int32_t, uint8_t, uint16_t, and uint32_t types.
 *
 * The coverage of the num input for 64-bit types is only partial in
 * order to acheive a reasonable test time, especially since the
 * extended precision floating point calculations required to test the
 * 64-bit routines are slow.
 * The num input tested with an increment of 2^35-1 across the
 * range of each 64-bit type for about 536,870,912 tests at every shift
 * value.
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
#include <vector>
#include <utility>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include "multshiftround_comp.hpp"
#include "shiftround_comp.hpp"

extern "C"
{
  #include "multshiftround_comp.h"
  #include "shiftround_comp.h"
}

#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/round.hpp>

typedef boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<80, boost::multiprecision::backends::digit_base_2, void, boost::int16_t, -16382, 16383>, boost::multiprecision::et_off> cpp_bin_float_80;

/**
 * Setting the mul argument of multshiftround to 1 for various types.
 * This is for testing the shift and round portions of multshiftround.
 * The multiplication operation in multshiftround
 *     type prod = num * mul;
 * is tested separately.
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

/**
 * There is one atomic bool for each thread, set to true upon
 * thread initiation and set to false as the last computation 
 * in the thread.
 * This helps decide when to join a thread and replace it with
 * a new thread.
 * Plain C array because atomics don't have move or copy constructors.
 */
std::atomic<bool> *thread_running;

/**
 * Mutex for stdout when running multithreaded.
 */
std::mutex print_mutex;

/**
 * Test c++ style compile time int32_t multshiftround for num on [-2147483648, 2147483647].
 * shift should range from 1 to 30.
 */
void test_multshiftround_i32_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<int32_t, %u>()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 30u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case  1:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 1>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 1>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  2:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 2>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 2>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  3:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 3>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 3>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  4:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 4>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 4>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  5:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 5>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 5>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  6:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 6>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 6>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  7:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 7>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 7>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  8:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 8>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 8>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  9:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 9>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 9>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 10>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 10>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 11>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 11>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 12>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 12>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 13>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 13>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 14>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 14>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 15>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 15>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 16>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 16>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 17>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 17>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 18>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 18>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 19>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 19>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 20>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 20>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 21>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 21>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 22>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 22>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 23>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 23>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 24>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 24>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 25>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 25>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 26>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 26>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 27>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 27>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 28>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 28>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 29>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 29>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround<int32_t, 30>(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int32_t, 30>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<int32t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<int32_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }
  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time int32_t multshiftround for num on [-2147483648, 2147483647].
 * shift should range from 1 to 30.
 */
void test_multshiftround_i32_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_i32_%u()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 30u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case  1:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_1(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_1(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  2:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_2(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_2(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  3:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_3(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_3(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  4:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_4(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_4(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  5:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_5(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_5(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  6:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_6(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_6(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  7:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_7(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_7(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  8:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_8(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_8(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  9:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_9(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_9(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  10:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_10(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_10(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  11:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_11(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_11(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  12:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_12(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_12(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  13:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_13(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_13(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  14:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_14(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_14(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  15:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_15(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_15(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  16:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_16(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_16(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  17:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_17(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_17(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  18:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_18(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_18(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  19:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_19(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_19(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  20:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_20(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_20(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case  21:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_21(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_21(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_22(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_22(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_23(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_23(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_24(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_24(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_25(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_25(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_26(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_26(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_27(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_27(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_28(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_28(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_29(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_29(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = multshiftround_i32_30(num, mul_i32);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i32_30(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_i32_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_i32_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }
  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time int32_t shiftround for num on [-2147483648, 2147483647].
 * shift should range from 1 to 30.
 */
void test_shiftround_i32_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<int32_t, %u>()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 30u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 1>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 1>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 2>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 2>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 3>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 3>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 4>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 4>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 5>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 5>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 6>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 6>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 7>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 7>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 8>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 8>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 9>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 9>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 10>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 10>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 11>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 11>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 12>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 12>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 13>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 13>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 14>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 14>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 15>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 15>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 16>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 16>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 17>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 17>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 18>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 18>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 19>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 19>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 20>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 20>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 21>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 21>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 22>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 22>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 23>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 23>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 24>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 24>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 25>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 25>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 26>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 26>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 27>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 27>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 28>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 28>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 29>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 29>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround<int32_t, 30>(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int32_t, 30>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<int32_t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<int32_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time int32_t shiftround for num on [-2147483648, 2147483647].
 * shift should range from 1 to 30.
 */
void test_shiftround_i32_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_i32_%u()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 30u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_1(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_1(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_2(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_2(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_3(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_3(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_4(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_4(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_5(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_5(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_6(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_6(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_7(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_7(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_8(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_8(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_9(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_9(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_10(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_10(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_11(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_11(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_12(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_12(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_13(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_13(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_14(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_14(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_15(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_15(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_16(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_16(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_17(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_17(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_18(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_18(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_19(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_19(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_20(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_20(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_21(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_21(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_22(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_22(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_23(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_23(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_24(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_24(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_25(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_25(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_26(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_26(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_27(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_27(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_28(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_28(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_29(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_29(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      int32_t num = std::numeric_limits<int32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        int32_t ms_res = shiftround_i32_30(num);
        int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i32_30(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<int32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_i32_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_i32_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time uint32_t multshiftround for num on [0, 4294967295].
 * shift should range from 1 to 31.
 */
void test_multshiftround_u32_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<uint32_t, %u>()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 31u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 1>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 1>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 2>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 2>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 3>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 3>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 4>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 4>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 5>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 5>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 6>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 6>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 7>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 7>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 8>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 8>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 9>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 9>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 10>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 10>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 11>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 11>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 12>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 12>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 13>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 13>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 14>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 14>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 15>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 15>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 16>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 16>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 17>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 17>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 18>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 18>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 19>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 19>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 20>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 20>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 21>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 21>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 22>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 22>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 23>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 23>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 24>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 24>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 25>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 25>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 26>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 26>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 27>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 27>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 28>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 28>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 29>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 29>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 30>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 30>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 31:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround<uint32_t, 31>(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint32_t, 31>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<uint32t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<uint32_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time uint32_t multshiftround for num on [0, 4294967295].
 * shift should range from 1 to 31.
 */
void test_multshiftround_u32_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_u32_%u()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 31u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_1(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_1(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_2(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_2(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_3(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_3(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_4(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_4(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_5(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_5(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_6(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_6(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_7(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_7(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_8(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_8(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_9(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_9(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_10(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_10(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_11(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_11(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_12(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_12(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_13(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_13(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_14(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_14(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_15(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_15(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_16(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_16(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_17(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_17(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_18(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_18(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_19(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_19(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_20(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_20(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_21(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_21(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_22(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_22(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_23(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_23(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_24(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_24(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_25(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_25(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_26(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_26(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_27(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_27(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_28(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_28(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_29(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_29(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_30(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_30(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 31:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = multshiftround_u32_31(num, mul_u32);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u32_31(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_u32);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_u32_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_u32_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time uint32_t shiftround for num on [0, 4294967295].
 * shift should range from 1 to 31.
 */
void test_shiftround_u32_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<uint32_t, %u>()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 31u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 1>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 1>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 2>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 2>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 3>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 3>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 4>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 4>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 5>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 5>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 6>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 6>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 7>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 7>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 8>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 8>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 9>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 9>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 10>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 10>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 11>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 11>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 12>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 12>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 13>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 13>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 14>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 14>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 15>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 15>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 16>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 16>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 17>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 17>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 18>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 18>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 19>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 19>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 20>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 20>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 21>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 21>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 22>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 22>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 23>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 23>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 24>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 24>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 25>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 25>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 26>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 26>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 27>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 27>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 28>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 28>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 29>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 29>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 30>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 30>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 31:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround<uint32_t, 31>(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint32_t, 31>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<uint32t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<uint32_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time uint32_t shiftround for num on [0, 4294967295].
 * shift should range from 1 to 31.
 */
void test_shiftround_u32_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_u32_%u()\n", shift);
  }

  double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
  if (shift >= 1u && shift <= 31u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_1(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_1(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 2:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_2(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_2(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 3:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_3(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_3(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 4:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_4(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_4(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 5:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_5(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_5(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 6:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_6(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_6(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 7:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_7(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_7(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 8:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_8(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_8(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 9:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_9(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_9(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 10:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_10(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_10(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 11:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_11(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_11(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 12:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_12(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_12(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 13:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_13(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_13(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 14:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_14(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_14(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 15:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_15(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_15(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 16:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_16(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_16(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 17:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_17(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_17(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 18:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_18(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_18(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 19:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_19(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_19(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 20:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_20(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_20(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 21:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_21(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_21(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 22:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_22(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_22(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 23:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_23(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_23(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 24:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_24(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_24(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 25:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_25(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_25(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 26:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_26(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_26(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 27:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_27(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_27(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 28:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_28(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_28(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 29:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_29(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_29(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 30:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_30(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_30(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    case 31:
    {
      uint32_t num = std::numeric_limits<uint32_t>::lowest();
      double dbl_num = static_cast<double>(num);
      while (true)
      {
        uint32_t ms_res = shiftround_u32_31(num);
        uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u32_31(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
        }
        if (num == std::numeric_limits<uint32_t>::max()) break;
        num++;
        dbl_num += 1.0;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_u32_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_u32_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time int64_t multshiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 1 to 62.
 */
void test_multshiftround_i64_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<int64_t, %u>()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 62u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 1>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 1>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 2>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 2>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 3>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 3>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 4>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 4>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 5>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 5>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 6>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 6>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 7>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 7>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 8>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 8>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 9>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 9>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 10>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 10>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 11>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 11>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 12>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 12>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 13>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 13>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 14>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 14>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 15>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 15>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 16>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 16>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 17>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 17>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 18>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 18>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 19>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 19>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 20>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 20>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 21>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 21>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 22>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 22>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 23>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 23>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 24>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 24>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 25>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 25>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 26>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 26>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 27>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 27>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 28>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 28>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 29>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 29>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 30>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 30>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 31>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 31>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 32>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 32>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 33>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 33>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 34>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 34>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 35>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 35>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 36>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 36>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 37>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 37>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 38>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 38>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 39>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 39>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 40>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 40>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 41>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 41>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 42>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 42>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 43>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 43>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 44>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 44>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 45>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 45>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 46>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 46>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 47>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 47>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 48>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 48>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 49>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 49>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 50>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 50>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 51>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 51>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 52>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 52>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 53>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 53>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 54>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 54>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 55>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 55>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 56>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 56>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 57>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 57>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 58>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 58>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 59>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 59>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 60>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 60>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 61>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 61>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround<int64_t, 62>(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<int64_t, 62>(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<int64_t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<int64_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time int64_t multshiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 1 to 62.
 */
void test_multshiftround_i64_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_i64_%u()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 62u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_1(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_1(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_2(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_2(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_3(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_3(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_4(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_4(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_5(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_5(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_6(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_6(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_7(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_7(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_8(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_8(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_9(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_9(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_10(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_10(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_11(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_11(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_12(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_12(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_13(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_13(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_14(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_14(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_15(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_15(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_16(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_16(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_17(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_17(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_18(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_18(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_19(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_19(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_20(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_20(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_21(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_21(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_22(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_22(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_23(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_23(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_24(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_24(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_25(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_25(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_26(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_26(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_27(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_27(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_28(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_28(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_29(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_29(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_30(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_30(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_31(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_31(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_32(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_32(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_33(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_33(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_34(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_34(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_35(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_35(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_36(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_36(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_37(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_37(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_38(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_38(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_39(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_39(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_40(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_40(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_41(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_41(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_42(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_42(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_43(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_43(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_44(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_44(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_45(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_45(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_46(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_46(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_47(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_47(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_48(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_48(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_49(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_49(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_50(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_50(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_51(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_51(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_52(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_52(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_53(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_53(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_54(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_54(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_55(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_55(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_56(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_56(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_57(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_57(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_58(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_58(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_59(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_59(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_60(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_60(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_61(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_61(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = multshiftround_i64_62(num, mul_i64);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_i64_62(num, mul): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_i64_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_i64_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time int64_t shiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 1 to 62.
 */
void test_shiftround_i64_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<int64_t, %u>()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 62u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 1>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 1>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 2>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 2>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 3>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 3>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 4>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 4>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 5>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 5>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 6>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 6>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 7>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 7>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 8>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 8>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 9>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 9>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 10>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 10>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 11>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 11>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 12>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 12>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 13>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 13>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 14>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 14>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 15>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 15>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 16>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 16>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 17>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 17>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 18>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 18>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 19>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 19>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 20>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 20>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 21>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 21>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 22>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 22>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 23>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 23>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 24>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 24>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 25>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 25>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 26>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 26>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 27>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 27>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 28>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 28>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 29>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 29>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 30>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 30>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 31>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 31>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 32>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 32>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 33>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 33>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 34>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 34>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 35>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 35>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 36>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 36>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 37>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 37>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 38>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 38>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 39>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 39>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 40>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 40>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 41>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 41>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 42>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 42>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 43>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 43>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 44>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 44>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 45>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 45>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 46>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 46>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 47>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 47>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 48>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 48>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 49>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 49>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 50>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 50>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 51>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 51>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 52>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 52>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 53>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 53>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 54>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 54>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 55>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 55>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 56>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 56>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 57>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 57>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 58>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 58>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 59>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 59>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 60>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 60>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 61>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 61>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround<int64_t, 62>(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<int64_t, 62>(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<int64_t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<int64_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time int64_t shiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 1 to 62.
 */
void test_shiftround_i64_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_i64_%u()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 62u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_1(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_1(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_2(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_2(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_3(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_3(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_4(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_4(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_5(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_5(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_6(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_6(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_7(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_7(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_8(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_8(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_9(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_9(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_10(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_10(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_11(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_11(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_12(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_12(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_13(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_13(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_14(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_14(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_15(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_15(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_16(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_16(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_17(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_17(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_18(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_18(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_19(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_19(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_20(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_20(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_21(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_21(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_22(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_22(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_23(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_23(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_24(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_24(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_25(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_25(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_26(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_26(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_27(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_27(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_28(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_28(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_29(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_29(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_30(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_30(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_31(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_31(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_32(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_32(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_33(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_33(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_34(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_34(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_35(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_35(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_36(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_36(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_37(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_37(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_38(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_38(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_39(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_39(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_40(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_40(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_41(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_41(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_42(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_42(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_43(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_43(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_44(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_44(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_45(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_45(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_46(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_46(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_47(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_47(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_48(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_48(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_49(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_49(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_50(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_50(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_51(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_51(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_52(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_52(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_53(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_53(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_54(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_54(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_55(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_55(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_56(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_56(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_57(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_57(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_58(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_58(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_59(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_59(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_60(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_60(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_61(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_61(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      int64_t increment = (1ll << 35) - 1ll;
      cpp_bin_float_80 ldbl_increment(increment);
      int64_t num = std::numeric_limits<int64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        int64_t ms_res = shiftround_i64_62(num);
        int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_i64_62(num): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_i64_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_i64_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time uint64_t multshiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 1 to 63.
 */
void test_multshiftround_u64_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<uint64_t, %u>()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 63u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 1>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 1>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 2>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 2>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 3>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 3>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 4>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 4>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 5>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 5>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 6>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 6>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 7>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 7>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 8>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 8>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 9>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 9>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 10>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 10>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 11>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 11>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 12>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 12>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 13>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 13>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 14>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 14>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 15>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 15>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 16>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 16>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 17>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 17>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 18>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 18>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 19>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 19>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 20>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 20>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 21>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 21>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 22>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 22>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 23>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 23>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 24>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 24>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 25>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 25>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 26>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 26>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 27>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 27>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 28>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 28>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 29>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 29>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 30>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 30>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 31>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 31>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 32>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 32>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 33>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 33>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 34>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 34>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 35>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 35>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 36>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 36>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 37>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 37>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 38>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 38>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 39>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 39>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 40>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 40>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 41>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 41>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 42>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 42>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 43>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 43>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 44>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 44>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 45>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 45>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 46>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 46>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 47>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 47>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 48>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 48>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 49>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 49>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 50>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 50>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 51>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 51>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 52>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 52>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 53>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 53>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 54>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 54>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 55>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 55>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 56>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 56>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 57>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 57>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 58>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 58>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 59>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 59>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 60>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 60>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 61>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 61>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 62>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 62>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 63:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround<uint64_t, 63>(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround<uint64_t, 63>(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<uint64_t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<uint64_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time uint64_t multshiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 1 to 63.
 */
void test_multshiftround_u64_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_u64_%u()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 63u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_1(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_1(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_2(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_2(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_3(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_3(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_4(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_4(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_5(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_5(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_6(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_6(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_7(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_7(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_8(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_8(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_9(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_9(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_10(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_10(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_11(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_11(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_12(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_12(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_13(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_13(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_14(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_14(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_15(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_15(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_16(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_16(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_17(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_17(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_18(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_18(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_19(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_19(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_20(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_20(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_21(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_21(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_22(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_22(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_23(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_23(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_24(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_24(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_25(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_25(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_26(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_26(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_27(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_27(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_28(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_28(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_29(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_29(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_30(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_30(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_31(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_31(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_32(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_32(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_33(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_33(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_34(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_34(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_35(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_35(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_36(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_36(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_37(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_37(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_38(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_38(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_39(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_39(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_40(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_40(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_41(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_41(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_42(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_42(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_43(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_43(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_44(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_44(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_45(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_45(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_46(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_46(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_47(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_47(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_48(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_48(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_49(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_49(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_50(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_50(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_51(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_51(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_52(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_52(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_53(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_53(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_54(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_54(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_55(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_55(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_56(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_56(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_57(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_57(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_58(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_58(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_59(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_59(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_60(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_60(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_61(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_61(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_62(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_62(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 63:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = multshiftround_u64_63(num, mul_u64);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: multshiftround_u64_63(num, mul): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_u64_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_u64_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style compile time uint64_t shiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_shiftround_u64_comp_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<uint64_t, %u>()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 63u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 1>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 1>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 2>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 2>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 3>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 3>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 4>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 4>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 5>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 5>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 6>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 6>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 7>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 7>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 8>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 8>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 9>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 9>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 10>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 10>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 11>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 11>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 12>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 12>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 13>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 13>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 14>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 14>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 15>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 15>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 16>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 16>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 17>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 17>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 18>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 18>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 19>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 19>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 20>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 20>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 21>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 21>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 22>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 22>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 23>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 23>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 24>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 24>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 25>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 25>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 26>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 26>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 27>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 27>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 28>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 28>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 29>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 29>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 30>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 30>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 31>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 31>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 32>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 32>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 33>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 33>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 34>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 34>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 35>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 35>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 36>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 36>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 37>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 37>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 38>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 38>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 39>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 39>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 40>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 40>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 41>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 41>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 42>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 42>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 43>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 43>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 44>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 44>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 45>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 45>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 46>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 46>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 47>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 47>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 48>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 48>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 49>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 49>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 50>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 50>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 51>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 51>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 52>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 52>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 53>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 53>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 54>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 54>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 55>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 55>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 56>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 56>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 57>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 57>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 58>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 58>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 59>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 59>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 60>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 60>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 61>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 61>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 62>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 62>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 63:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround<uint64_t, 63>(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround<uint64_t, 63>(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<uint64_t, ?""?>(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<uint64_t, %u>() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style compile time uint64_t shiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_shiftround_u64_comp_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_u64_%u()\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = std::numeric_limits<cpp_bin_float_80>::quiet_NaN();
  if (shift >= 1u && shift <= 63u) ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);

  switch (shift)
  {
    case 1:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_1(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_1(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 2:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_2(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_2(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 3:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_3(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_3(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 4:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_4(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_4(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 5:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_5(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_5(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 6:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_6(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_6(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 7:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_7(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_7(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 8:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_8(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_8(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 9:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_9(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_9(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 10:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_10(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_10(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 11:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_11(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_11(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 12:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_12(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_12(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 13:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_13(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_13(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 14:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_14(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_14(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 15:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_15(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_15(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 16:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_16(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_16(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 17:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_17(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_17(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 18:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_18(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_18(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 19:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_19(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_19(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 20:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_20(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_20(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 21:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_21(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_21(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 22:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_22(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_22(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 23:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_23(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_23(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 24:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_24(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_24(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 25:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_25(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_25(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 26:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_26(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_26(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 27:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_27(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_27(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 28:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_28(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_28(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 29:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_29(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_29(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 30:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_30(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_30(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 31:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_31(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_31(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 32:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_32(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_32(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 33:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_33(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_33(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 34:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_34(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_34(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 35:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_35(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_35(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 36:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_36(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_36(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 37:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_37(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_37(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 38:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_38(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_38(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 39:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_39(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_39(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 40:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_40(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_40(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 41:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_41(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_41(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 42:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_42(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_42(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 43:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_43(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_43(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 44:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_44(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_44(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 45:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_45(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_45(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 46:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_46(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_46(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 47:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_47(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_47(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 48:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_48(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_48(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 49:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_49(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_49(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 50:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_50(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_50(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 51:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_51(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_51(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 52:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_52(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_52(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 53:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_53(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_53(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 54:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_54(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_54(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 55:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_55(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_55(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 56:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_56(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_56(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 57:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_57(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_57(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 58:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_58(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_58(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 59:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_59(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_59(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 60:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_60(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_60(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 61:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_61(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_61(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 62:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_62(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_62(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    case 63:
    {
      uint64_t increment = (1ull << 35) - 1ull;
      cpp_bin_float_80 ldbl_increment(increment);
      uint64_t num = std::numeric_limits<uint64_t>::lowest();
      cpp_bin_float_80 ldbl_num(num);
      while (true)
      {
        uint64_t ms_res = shiftround_u64_63(num);
        uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
        if (ms_res != dbl_res) {
          std::lock_guard<std::mutex> print_lock(print_mutex);
          std::printf("ERROR: shiftround_u64_63(num): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
        }
        if (std::numeric_limits<uint64_t>::max() - num < increment) break;
        num += increment;
        ldbl_num += ldbl_increment;
      }
      break;
    }
    default: 
    {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_u64_?""?(): invalid shift value %u\n", shift);
      break;
    }
  }
  
  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_u64_%u() took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}


int main()
{
  std::printf("\nTesting multiplication operation in all multshiftround routines.\n");

  /**
   * int8_t multshiftround routines that require the shift value at compile time
   */
  int8_t msr_i8 = multshiftround_i8_1(2, 2);
  if (msr_i8 != 2) std::printf("\nERROR: multshiftround_i8_1(2, 2) returned %i. expected 2.\n\n", msr_i8);
  msr_i8 = multshiftround_i8_2(2, 2);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8_2(2, 2) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround_i8_3(2, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8_3(2, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround_i8_4(4, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8_4(4, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround_i8_5(8, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8_5(8, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround_i8_6(8, 8);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8_6(8, 8) returned %i. expected 1.\n\n", msr_i8);

  msr_i8 = multshiftround<int8_t, 1>(2, 2);
  if (msr_i8 != 2) std::printf("\nERROR: multshiftround<int8_t, 1>(2, 2) returned %i. expected 2.\n\n", msr_i8);
  msr_i8 = multshiftround<int8_t, 2>(2, 2);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t, 2>(2, 2) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround<int8_t, 3>(2, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t, 3>(2, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround<int8_t, 4>(4, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t, 4>(4, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround<int8_t, 5>(8, 4);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t, 5>(8, 4) returned %i. expected 1.\n\n", msr_i8);
  msr_i8 = multshiftround<int8_t, 6>(8, 8);
  if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t, 6>(8, 8) returned %i. expected 1.\n\n", msr_i8);

  /**
   * uint8_t multshiftround routines that require the shift value at compile time
   */
  uint8_t msr_u8 = multshiftround_u8_1(2u, 2u);
  if (msr_u8 != 2) std::printf("\nERROR: multshiftround_u8_1(2u, 2u) returned %i. expected 2.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_2(2u, 2u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_2(2u, 2u) returned %i. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_3(2u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_3(2u, 4u) returned %i. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_4(4u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_4(4u, 4u) returned %i. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_5(8u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_5(8u, 4u) returned %i. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_6(8u, 8u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_6(8u, 8u) returned %i. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround_u8_7(8u, 16u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround_u8_7(8u, 16u) returned %i. expected 1.\n\n", msr_u8);

  msr_u8 = multshiftround<uint8_t, 1>(2u, 2u);
  if (msr_u8 != 2) std::printf("\nERROR: multshiftround<uint8_t, 1>(2u, 2u) returned %u. expected 2.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 2>(2u, 2u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 2>(2u, 2u) returned %u. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 3>(2u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 3>(2u, 4u) returned %u. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 4>(4u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 4>(4u, 4u) returned %u. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 5>(8u, 4u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 5>(8u, 4u) returned %u. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 6>(8u, 8u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 6>(8u, 8u) returned %u. expected 1.\n\n", msr_u8);
  msr_u8 = multshiftround<uint8_t, 7>(8u, 16u);
  if (msr_u8 != 1) std::printf("\nERROR: multshiftround<uint8_t, 7>(8u, 16u) returned %u. expected 1.\n\n", msr_u8);

  /**
   * int16_t multshiftround routines that require the shift value at compile time
   */
  int16_t msr_i16 = multshiftround_i16_1(2, 2);
  if (msr_i16 != 2) std::printf("\nERROR: multshiftround_i16_1(2, 2) returned %i. expected 2.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_2(2, 2);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_2(2, 2) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_3(2, 4);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_3(2, 4) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_4(4, 4);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_4(4, 4) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_5(4, 8);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_5(4, 8) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_6(8, 8);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_6(8, 8) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_7(8, 16);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_7(8, 16) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_8(16, 16);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_8(16, 16) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_9(16, 32);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_9(16, 32) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_10(32, 32);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_10(32, 32) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_11(32, 64);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_11(32, 64) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_12(64, 64);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_12(64, 64) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_13(64, 128);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_13(64, 128) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround_i16_14(128, 128);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16_14(128, 128) returned %i. expected 1.\n\n", msr_i16);

  msr_i16 = multshiftround<int16_t, 1>(2, 2);
  if (msr_i16 != 2) std::printf("\nERROR: multshiftround<int16_t, 1>(2, 2) returned %i. expected 2.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 2>(2, 2);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 2>(2, 2) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 3>(2, 4);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 3>(2, 4) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 4>(4, 4);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 4>(4, 4) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 5>(4, 8);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 5>(4, 8) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 6>(8, 8);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 6>(8, 8) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 7>(8, 16);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 7>(8, 16) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 8>(16, 16);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 8>(16, 16) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 9>(16, 32);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 9>(16, 32) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 10>(32, 32);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 10>(32, 32) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 11>(32, 64);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 11>(32, 64) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 12>(64, 64);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 12>(64, 64) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 13>(64, 128);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 13>(64, 128) returned %i. expected 1.\n\n", msr_i16);
  msr_i16 = multshiftround<int16_t, 14>(128, 128);
  if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t, 14>(128, 128) returned %i. expected 1.\n\n", msr_i16);

  /**
   * uint16_t multshiftround routines that require the shift value at compile time
   */
  uint16_t msr_u16 = multshiftround_u16_1(2u, 2u);
  if (msr_u16 != 2) std::printf("\nERROR: multshiftround_u16_1(2u, 2u) returned %u. expected 2.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_2(2u, 2u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_2(2u, 2u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_3(2u, 4u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_3(2u, 4u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_4(4u, 4u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_4(4u, 4u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_5(4u, 8u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_5(4u, 8u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_6(8u, 8u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_6(8u, 8u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_7(8u, 16u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_7(8u, 16u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_8(16u, 16u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_8(16u, 16u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_9(16u, 32u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_9(16u, 32u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_10(32u, 32u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_10(32u, 32u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_11(32u, 64u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_11(32u, 64u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_12(64u, 64u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_12(64u, 64u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_13(64u, 128u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_13(64u, 128u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_14(128u, 128u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_14(128u, 128u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround_u16_15(128u, 256u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround_u16_15(128u, 256u) returned %u. expected 1.\n\n", msr_u16);

  msr_u16 = multshiftround<uint16_t, 1>(2u, 2u);
  if (msr_u16 != 2) std::printf("\nERROR: multshiftround<uint16_t, 1>(2u, 2u) returned %u. expected 2.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 2>(2u, 2u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 2>(2u, 2u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 3>(2u, 4u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 3>(2u, 4u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 4>(4u, 4u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 4>(4u, 4u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 5>(4u, 8u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 5>(4u, 8u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 6>(8u, 8u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 6>(8u, 8u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 7>(8u, 16u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 7>(8u, 16u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 8>(16u, 16u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 8>(16u, 16u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 9>(16u, 32u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 9>(16u, 32u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 10>(32u, 32u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 10>(32u, 32u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 11>(32u, 64u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 11>(32u, 64u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 12>(64u, 64u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 12>(64u, 64u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 13>(64u, 128u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 13>(64u, 128u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 14>(128u, 128u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 14>(128u, 128u) returned %u. expected 1.\n\n", msr_u16);
  msr_u16 = multshiftround<uint16_t, 15>(128u, 256u);
  if (msr_u16 != 1) std::printf("\nERROR: multshiftround<uint16_t, 15>(128u, 256u) returned %u. expected 1.\n\n", msr_u16);

  /**
   * int32_t multshiftround routines that require the shift value at compile time
   */
  int32_t msr_i32 = multshiftround_i32_1(2, 2);
  if (msr_i32 != 2) std::printf("\nERROR: multshiftround_i32_1(2, 2) returned %i. expected 2.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_2(2, 2);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_2(2, 2) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_3(2, 4);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_3(2, 4) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_4(4, 4);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_4(4, 4) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_5(4, 8);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_5(4, 8) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_6(8, 8);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_6(8, 8) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_7(8, 16);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_7(8, 16) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_8(16, 16);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_8(16, 16) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_9(16, 32);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_9(16, 32) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_10(32, 32);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_10(32, 32) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_11(32, 64);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_11(32, 64) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_12(64, 64);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_12(64, 64) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_13(64, 128);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_13(64, 128) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_14(128, 128);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_14(128, 128) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_15(128, 256);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_15(128, 256) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_16(256, 256);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_16(256, 256) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_17(256, 512);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_17(256, 512) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_18(512, 512);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_18(512, 512) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_19(512, 1024);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_19(512, 1024) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_20(1024, 1024);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_20(1024, 1024) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_21(1024, 2048);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_21(1024, 2048) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_22(2048, 2048);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_22(2048, 2048) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_23(2048, 4096);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_23(2048, 4096) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_24(4096, 4096);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_24(4096, 4096) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_25(4096, 8192);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_25(4096, 8192) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_26(8192, 8192);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_26(8192, 8192) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_27(8192, 16384);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_27(8192, 16384) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_28(16384, 16384);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_28(16384, 16384) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_29(16384, 32768);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_29(16384, 32768) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround_i32_30(32768, 32768);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32_30(32768, 32768) returned %i. expected 1.\n\n", msr_i32);

  msr_i32 = multshiftround<int32_t, 1>(2, 2);
  if (msr_i32 != 2) std::printf("\nERROR: multshiftround<int32_t, 1>(2, 2) returned %i. expected 2.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 2>(2, 2);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 2>(2, 2) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 3>(2, 4);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 3>(2, 4) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 4>(4, 4);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 4>(4, 4) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 5>(4, 8);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 5>(4, 8) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 6>(8, 8);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 6>(8, 8) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 7>(8, 16);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 7>(8, 16) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 8>(16, 16);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 8>(16, 16) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 9>(16, 32);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 9>(16, 32) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 10>(32, 32);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 10>(32, 32) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 11>(32, 64);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 11>(32, 64) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 12>(64, 64);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 12>(64, 64) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 13>(64, 128);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 13>(64, 128) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 14>(128, 128);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 14>(128, 128) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 15>(128, 256);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 15>(128, 256) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 16>(256, 256);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 16>(256, 256) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 17>(256, 512);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 17>(256, 512) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 18>(512, 512);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 18>(512, 512) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 19>(512, 1024);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 19>(512, 1024) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 20>(1024, 1024);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 20>(1024, 1024) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 21>(1024, 2048);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 21>(1024, 2048) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 22>(2048, 2048);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 22>(2048, 2048) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 23>(2048, 4096);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 23>(2048, 4096) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 24>(4096, 4096);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 24>(4096, 4096) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 25>(4096, 8192);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 25>(4096, 8192) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 26>(8192, 8192);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 26>(8192, 8192) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 27>(8192, 16384);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 27>(8192, 16384) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 28>(16384, 16384);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 28>(16384, 16384) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 29>(16384, 32768);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 29>(16384, 32768) returned %i. expected 1.\n\n", msr_i32);
  msr_i32 = multshiftround<int32_t, 30>(32768, 32768);
  if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t, 30>(32768, 32768) returned %i. expected 1.\n\n", msr_i32);

  /**
   * uint32_t multshiftround routines that require the shift value at compile time
   */
  uint32_t msr_u32 = multshiftround_u32_1(2u, 2u);
  if (msr_u32 != 2) std::printf("\nERROR: multshiftround_u32_1(2u, 2u) returned %u. expected 2.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_2(2u, 2u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_2(2u, 2u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_3(2u, 4u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_3(2u, 4u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_4(4u, 4u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_4(4u, 4u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_5(4u, 8u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_5(4u, 8u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_6(8u, 8u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_6(8u, 8u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_7(8u, 16u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_7(8u, 16u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_8(16u, 16u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_8(16u, 16u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_9(16u, 32u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_9(16u, 32u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_10(32u, 32u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_10(32u, 32u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_11(32u, 64u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_11(32u, 64u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_12(64u, 64u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_12(64u, 64u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_13(64u, 128u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_13(64u, 128u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_14(128u, 128u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_14(128u, 128u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_15(128u, 256u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_15(128u, 256u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_16(256u, 256u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_16(256u, 256u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_17(256u, 512u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_17(256u, 512u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_18(512u, 512u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_18(512u, 512u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_19(512u, 1024u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_19(512u, 1024u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_20(1024u, 1024u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_20(1024u, 1024u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_21(1024u, 2048u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_21(1024u, 2048u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_22(2048u, 2048u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_22(2048u, 2048u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_23(2048u, 4096u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_23(2048u, 4096u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_24(4096u, 4096u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_24(4096u, 4096u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_25(4096u, 8192u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_25(4096u, 8192u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_26(8192u, 8192u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_26(8192u, 8192u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_27(8192u, 16384u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_27(8192u, 16384u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_28(16384u, 16384u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_28(16384u, 16384u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_29(16384u, 32768u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_29(16384u, 32768u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_30(32768u, 32768u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_30(32768u, 32768u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround_u32_31(32768u, 65536u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround_u32_31(32768u, 65536u) returned %u. expected 1.\n\n", msr_u32);

  msr_u32 = multshiftround<uint32_t, 1>(2u, 2u);
  if (msr_u32 != 2) std::printf("\nERROR: multshiftround<uint32_t, 1>(2u, 2u) returned %u. expected 2.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 2>(2u, 2u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 2>(2u, 2u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 3>(2u, 4u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 3>(2u, 4u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 4>(4u, 4u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 4>(4u, 4u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 5>(4u, 8u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 5>(4u, 8u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 6>(8u, 8u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 6>(8u, 8u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 7>(8u, 16u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 7>(8u, 16u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 8>(16u, 16u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 8>(16u, 16u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 9>(16u, 32u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 9>(16u, 32u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 10>(32u, 32u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 10>(32u, 32u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 11>(32u, 64u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 11>(32u, 64u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 12>(64u, 64u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 12>(64u, 64u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 13>(64u, 128u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 13>(64u, 128u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 14>(128u, 128u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 14>(128u, 128u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 15>(128u, 256u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 15>(128u, 256u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 16>(256u, 256u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 16>(256u, 256u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 17>(256u, 512u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 17>(256u, 512u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 18>(512u, 512u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 18>(512u, 512u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 19>(512u, 1024u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 19>(512u, 1024u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 20>(1024u, 1024u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 20>(1024u, 1024u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 21>(1024u, 2048u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 21>(1024u, 2048u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 22>(2048u, 2048u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 22>(2048u, 2048u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 23>(2048u, 4096u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 23>(2048u, 4096u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 24>(4096u, 4096u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 24>(4096u, 4096u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 25>(4096u, 8192u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 25>(4096u, 8192u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 26>(8192u, 8192u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 26>(8192u, 8192u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 27>(8192u, 16384u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 27>(8192u, 16384u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 28>(16384u, 16384u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 28>(16384u, 16384u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 29>(16384u, 32768u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 29>(16384u, 32768u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 30>(32768u, 32768u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 30>(32768u, 32768u) returned %u. expected 1.\n\n", msr_u32);
  msr_u32 = multshiftround<uint32_t, 31>(32768u, 65536u);
  if (msr_u32 != 1) std::printf("\nERROR: multshiftround<uint32_t, 31>(32768u, 65536u) returned %u. expected 1.\n\n", msr_u32);

  /**
   * int64_t multshiftround routines that require the shift value at compile time
   */
  int64_t msr_i64 = multshiftround_i64_1(2ll, 2ll);
  if (msr_i64 != 2ll) std::printf("\nERROR: multshiftround_i64_1(2ll, 2ll) returned %" PRIi64 ". expected 2.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_2(2ll, 2ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_2(2ll, 2ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_3(2ll, 4ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_3(2ll, 4ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_4(4ll, 4ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_4(4ll, 4ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_5(4ll, 8ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_5(4ll, 8ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_6(8ll, 8ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_6(8ll, 8ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_7(8ll, 16ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_7(8ll, 16ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_8(16ll, 16ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_8(16ll, 16ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_9(16ll, 32ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_9(16ll, 32ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_10(32ll, 32ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_10(32ll, 32ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_11(32ll, 64ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_11(32ll, 64ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_12(64ll, 64ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_12(64ll, 64ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_13(64ll, 128ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_13(64ll, 128ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_14(128ll, 128ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_14(128ll, 128ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_15(128ll, 256ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_15(128ll, 256ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_16(256ll, 256ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_16(256ll, 256ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_17(256ll, 512ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_17(256ll, 512ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_18(512ll, 512ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_18(512ll, 512ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_19(512ll, 1024ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_19(512ll, 1024ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_20(1024ll, 1024ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_20(1024ll, 1024ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_21(1024ll, 2048ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_21(1024ll, 2048ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_22(2048ll, 2048ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_22(2048ll, 2048ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_23(2048ll, 4096ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_23(2048ll, 4096ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_24(4096ll, 4096ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_24(4096ll, 4096ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_25(4096ll, 8192ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_25(4096ll, 8192ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_26(8192ll, 8192ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_26(8192ll, 8192ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_27(8192ll, 16384ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_27(8192ll, 16384ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_28(16384ll, 16384ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_28(16384ll, 16384ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_29(16384ll, 32768ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_29(16384ll, 32768ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_30(32768ll, 32768ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_30(32768ll, 32768ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_31(32768ll, 65536ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_31(32768ll, 65536ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_32(65536ll, 65536ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_32(65536ll, 65536ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_33(65536ll, 131072ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_33(65536ll, 131072ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_34(131072ll, 131072ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_34(131072ll, 131072ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_35(131072ll, 262144ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_35(131072ll, 262144ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_36(262144ll, 262144ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_36(262144ll, 262144ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_37(262144ll, 524288ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_37(262144ll, 524288ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_38(524288ll, 524288ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_38(524288ll, 524288ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_39(524288ll, 1048576ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_39(524288ll, 1048576ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_40(1048576ll, 1048576ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_40(1048576ll, 1048576ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_41(1048576ll, 2097152ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_41(1048576ll, 2097152ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_42(2097152ll, 2097152ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_42(2097152ll, 2097152ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_43(2097152ll, 4194304ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_43(2097152ll, 4194304ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_44(4194304ll, 4194304ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_44(4194304ll, 4194304ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_45(4194304ll, 8388608ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_45(4194304ll, 8388608ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_46(8388608ll, 8388608ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_46(8388608ll, 8388608ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_47(8388608ll, 16777216ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_47(8388608ll, 16777216ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_48(16777216ll, 16777216ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_48(16777216ll, 16777216ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_49(16777216ll, 33554432ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_49(16777216ll, 33554432ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_50(33554432ll, 33554432ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_50(33554432ll, 33554432ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_51(33554432ll, 67108864ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_51(33554432ll, 67108864ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_52(67108864ll, 67108864ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_52(67108864ll, 67108864ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_53(67108864ll, 134217728ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_53(67108864ll, 134217728ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_54(134217728ll, 134217728ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_54(134217728ll, 134217728ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_55(134217728ll, 268435456ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_55(134217728ll, 268435456ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_56(268435456ll, 268435456ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_56(268435456ll, 268435456ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_57(268435456ll, 536870912ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_57(268435456ll, 536870912ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_58(536870912ll, 536870912ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_58(536870912ll, 536870912ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_59(536870912ll, 1073741824ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_59(536870912ll, 1073741824ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_60(1073741824ll, 1073741824ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_60(1073741824ll, 1073741824ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_61(1073741824ll, 2147483648ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_61(1073741824ll, 2147483648ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround_i64_62(2147483648ll, 2147483648ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64_62(2147483648ll, 2147483648ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);

  msr_i64 = multshiftround<int64_t, 1>(2ll, 2ll);
  if (msr_i64 != 2ll) std::printf("\nERROR: multshiftround<int64_t, 1>(2ll, 2ll) returned %" PRIi64 ". expected 2.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 2>(2ll, 2ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 2>(2ll, 2ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 3>(2ll, 4ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 3>(2ll, 4ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 4>(4ll, 4ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 4>(4ll, 4ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 5>(4ll, 8ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 5>(4ll, 8ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 6>(8ll, 8ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 6>(8ll, 8ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 7>(8ll, 16ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 7>(8ll, 16ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 8>(16ll, 16ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 8>(16ll, 16ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 9>(16ll, 32ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 9>(16ll, 32ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 10>(32ll, 32ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 10>(32ll, 32ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 11>(32ll, 64ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 11>(32ll, 64ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 12>(64ll, 64ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 12>(64ll, 64ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 13>(64ll, 128ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 13>(64ll, 128ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 14>(128ll, 128ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 14>(128ll, 128ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 15>(128ll, 256ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 15>(128ll, 256ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 16>(256ll, 256ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 16>(256ll, 256ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 17>(256ll, 512ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 17>(256ll, 512ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 18>(512ll, 512ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 18>(512ll, 512ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 19>(512ll, 1024ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 19>(512ll, 1024ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 20>(1024ll, 1024ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 20>(1024ll, 1024ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 21>(1024ll, 2048ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 21>(1024ll, 2048ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 22>(2048ll, 2048ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 22>(2048ll, 2048ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 23>(2048ll, 4096ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 23>(2048ll, 4096ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 24>(4096ll, 4096ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 24>(4096ll, 4096ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 25>(4096ll, 8192ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 25>(4096ll, 8192ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 26>(8192ll, 8192ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 26>(8192ll, 8192ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 27>(8192ll, 16384ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 27>(8192ll, 16384ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 28>(16384ll, 16384ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 28>(16384ll, 16384ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 29>(16384ll, 32768ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 29>(16384ll, 32768ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 30>(32768ll, 32768ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 30>(32768ll, 32768ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 31>(32768ll, 65536ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 31>(32768ll, 65536ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 32>(65536ll, 65536ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 32>(65536ll, 65536ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 33>(65536ll, 131072ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 33>(65536ll, 131072ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 34>(131072ll, 131072ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 34>(131072ll, 131072ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 35>(131072ll, 262144ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 35>(131072ll, 262144ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 36>(262144ll, 262144ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 36>(262144ll, 262144ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 37>(262144ll, 524288ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 37>(262144ll, 524288ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 38>(524288ll, 524288ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 38>(524288ll, 524288ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 39>(524288ll, 1048576ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 39>(524288ll, 1048576ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 40>(1048576ll, 1048576ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 40>(1048576ll, 1048576ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 41>(1048576ll, 2097152ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 41>(1048576ll, 2097152ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 42>(2097152ll, 2097152ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 42>(2097152ll, 2097152ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 43>(2097152ll, 4194304ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 43>(2097152ll, 4194304ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 44>(4194304ll, 4194304ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 44>(4194304ll, 4194304ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 45>(4194304ll, 8388608ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 45>(4194304ll, 8388608ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 46>(8388608ll, 8388608ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 46>(8388608ll, 8388608ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 47>(8388608ll, 16777216ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 47>(8388608ll, 16777216ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 48>(16777216ll, 16777216ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 48>(16777216ll, 16777216ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 49>(16777216ll, 33554432ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 49>(16777216ll, 33554432ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 50>(33554432ll, 33554432ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 50>(33554432ll, 33554432ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 51>(33554432ll, 67108864ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 51>(33554432ll, 67108864ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 52>(67108864ll, 67108864ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 52>(67108864ll, 67108864ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 53>(67108864ll, 134217728ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 53>(67108864ll, 134217728ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 54>(134217728ll, 134217728ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 54>(134217728ll, 134217728ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 55>(134217728ll, 268435456ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 55>(134217728ll, 268435456ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 56>(268435456ll, 268435456ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 56>(268435456ll, 268435456ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 57>(268435456ll, 536870912ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 57>(268435456ll, 536870912ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 58>(536870912ll, 536870912ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 58>(536870912ll, 536870912ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 59>(536870912ll, 1073741824ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 59>(536870912ll, 1073741824ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 60>(1073741824ll, 1073741824ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 60>(1073741824ll, 1073741824ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 61>(1073741824ll, 2147483648ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 61>(1073741824ll, 2147483648ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);
  msr_i64 = multshiftround<int64_t, 62>(2147483648ll, 2147483648ll);
  if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t, 62>(2147483648ll, 2147483648ll) returned %" PRIi64 ". expected 1.\n\n", msr_i64);

  /**
   * uint64_t multshiftround routines that require the shift value at compile time
   */
  uint64_t msr_u64 = multshiftround_u64_1(2ull, 2ull);
  if (msr_u64 != 2ull) std::printf("\nERROR: multshiftround_u64_1(2ull, 2ull) returned %" PRIu64 ". expected 2.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_2(2ull, 2ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_2(2ull, 2ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_3(2ull, 4ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_3(2ull, 4ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_4(4ull, 4ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_4(4ull, 4ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_5(4ull, 8ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_5(4ull, 8ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_6(8ull, 8ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_6(8ull, 8ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_7(8ull, 16ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_7(8ull, 16ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_8(16ull, 16ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_8(16ull, 16ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_9(16ull, 32ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_9(16ull, 32ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_10(32ull, 32ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_10(32ull, 32ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_11(32ull, 64ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_11(32ull, 64ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_12(64ull, 64ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_12(64ull, 64ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_13(64ull, 128ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_13(64ull, 128ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_14(128ull, 128ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_14(128ull, 128ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_15(128ull, 256ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_15(128ull, 256ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_16(256ull, 256ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_16(256ull, 256ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_17(256ull, 512ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_17(256ull, 512ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_18(512ull, 512ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_18(512ull, 512ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_19(512ull, 1024ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_19(512ull, 1024ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_20(1024ull, 1024ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_20(1024ull, 1024ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_21(1024ull, 2048ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_21(1024ull, 2048ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_22(2048ull, 2048ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_22(2048ull, 2048ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_23(2048ull, 4096ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_23(2048ull, 4096ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_24(4096ull, 4096ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_24(4096ull, 4096ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_25(4096ull, 8192ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_25(4096ull, 8192ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_26(8192ull, 8192ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_26(8192ull, 8192ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_27(8192ull, 16384ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_27(8192ull, 16384ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_28(16384ull, 16384ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_28(16384ull, 16384ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_29(16384ull, 32768ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_29(16384ull, 32768ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_30(32768ull, 32768ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_30(32768ull, 32768ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_31(32768ull, 65536ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_31(32768ull, 65536ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_32(65536ull, 65536ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_32(65536ull, 65536ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_33(65536ull, 131072ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_33(65536ull, 131072ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_34(131072ull, 131072ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_34(131072ull, 131072ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_35(131072ull, 262144ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_35(131072ull, 262144ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_36(262144ull, 262144ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_36(262144ull, 262144ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_37(262144ull, 524288ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_37(262144ull, 524288ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_38(524288ull, 524288ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_38(524288ull, 524288ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_39(524288ull, 1048576ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_39(524288ull, 1048576ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_40(1048576ull, 1048576ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_40(1048576ull, 1048576ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_41(1048576ull, 2097152ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_41(1048576ull, 2097152ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_42(2097152ull, 2097152ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_42(2097152ull, 2097152ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_43(2097152ull, 4194304ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_43(2097152ull, 4194304ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_44(4194304ull, 4194304ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_44(4194304ull, 4194304ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_45(4194304ull, 8388608ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_45(4194304ull, 8388608ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_46(8388608ull, 8388608ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_46(8388608ull, 8388608ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_47(8388608ull, 16777216ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_47(8388608ull, 16777216ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_48(16777216ull, 16777216ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_48(16777216ull, 16777216ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_49(16777216ull, 33554432ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_49(16777216ull, 33554432ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_50(33554432ull, 33554432ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_50(33554432ull, 33554432ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_51(33554432ull, 67108864ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_51(33554432ull, 67108864ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_52(67108864ull, 67108864ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_52(67108864ull, 67108864ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_53(67108864ull, 134217728ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_53(67108864ull, 134217728ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_54(134217728ull, 134217728ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_54(134217728ull, 134217728ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_55(134217728ull, 268435456ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_55(134217728ull, 268435456ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_56(268435456ull, 268435456ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_56(268435456ull, 268435456ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_57(268435456ull, 536870912ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_57(268435456ull, 536870912ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_58(536870912ull, 536870912ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_58(536870912ull, 536870912ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_59(536870912ull, 1073741824ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_59(536870912ull, 1073741824ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_60(1073741824ull, 1073741824ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_60(1073741824ull, 1073741824ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_61(1073741824ull, 2147483648ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_61(1073741824ull, 2147483648ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_62(2147483648ull, 2147483648ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_62(2147483648ull, 2147483648ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround_u64_63(2147483648ull, 4294967296ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64_63(2147483648ull, 4294967296ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);

  msr_u64 = multshiftround<uint64_t, 1>(2ull, 2ull);
  if (msr_u64 != 2ull) std::printf("\nERROR: multshiftround<uint64_t, 1>(2ull, 2ull) returned %" PRIu64 ". expected 2.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 2>(2ull, 2ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 2>(2ull, 2ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 3>(2ull, 4ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 3>(2ull, 4ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 4>(4ull, 4ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 4>(4ull, 4ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 5>(4ull, 8ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 5>(4ull, 8ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 6>(8ull, 8ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 6>(8ull, 8ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 7>(8ull, 16ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 7>(8ull, 16ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 8>(16ull, 16ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 8>(16ull, 16ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 9>(16ull, 32ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 9>(16ull, 32ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 10>(32ull, 32ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 10>(32ull, 32ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 11>(32ull, 64ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 11>(32ull, 64ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 12>(64ull, 64ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 12>(64ull, 64ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 13>(64ull, 128ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 13>(64ull, 128ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 14>(128ull, 128ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 14>(128ull, 128ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 15>(128ull, 256ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 15>(128ull, 256ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 16>(256ull, 256ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 16>(256ull, 256ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 17>(256ull, 512ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 17>(256ull, 512ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 18>(512ull, 512ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 18>(512ull, 512ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 19>(512ull, 1024ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 19>(512ull, 1024ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 20>(1024ull, 1024ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 20>(1024ull, 1024ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 21>(1024ull, 2048ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 21>(1024ull, 2048ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 22>(2048ull, 2048ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 22>(2048ull, 2048ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 23>(2048ull, 4096ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 23>(2048ull, 4096ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 24>(4096ull, 4096ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 24>(4096ull, 4096ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 25>(4096ull, 8192ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 25>(4096ull, 8192ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 26>(8192ull, 8192ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 26>(8192ull, 8192ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 27>(8192ull, 16384ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 27>(8192ull, 16384ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 28>(16384ull, 16384ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 28>(16384ull, 16384ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 29>(16384ull, 32768ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 29>(16384ull, 32768ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 30>(32768ull, 32768ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 30>(32768ull, 32768ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 31>(32768ull, 65536ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 31>(32768ull, 65536ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 32>(65536ull, 65536ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 32>(65536ull, 65536ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 33>(65536ull, 131072ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 33>(65536ull, 131072ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 34>(131072ull, 131072ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 34>(131072ull, 131072ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 35>(131072ull, 262144ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 35>(131072ull, 262144ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 36>(262144ull, 262144ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 36>(262144ull, 262144ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 37>(262144ull, 524288ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 37>(262144ull, 524288ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 38>(524288ull, 524288ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 38>(524288ull, 524288ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 39>(524288ull, 1048576ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 39>(524288ull, 1048576ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 40>(1048576ull, 1048576ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 40>(1048576ull, 1048576ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 41>(1048576ull, 2097152ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 41>(1048576ull, 2097152ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 42>(2097152ull, 2097152ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 42>(2097152ull, 2097152ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 43>(2097152ull, 4194304ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 43>(2097152ull, 4194304ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 44>(4194304ull, 4194304ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 44>(4194304ull, 4194304ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 45>(4194304ull, 8388608ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 45>(4194304ull, 8388608ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 46>(8388608ull, 8388608ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 46>(8388608ull, 8388608ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 47>(8388608ull, 16777216ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 47>(8388608ull, 16777216ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 48>(16777216ull, 16777216ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 48>(16777216ull, 16777216ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 49>(16777216ull, 33554432ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 49>(16777216ull, 33554432ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 50>(33554432ull, 33554432ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 50>(33554432ull, 33554432ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 51>(33554432ull, 67108864ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 51>(33554432ull, 67108864ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 52>(67108864ull, 67108864ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 52>(67108864ull, 67108864ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 53>(67108864ull, 134217728ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 53>(67108864ull, 134217728ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 54>(134217728ull, 134217728ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 54>(134217728ull, 134217728ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 55>(134217728ull, 268435456ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 55>(134217728ull, 268435456ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 56>(268435456ull, 268435456ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 56>(268435456ull, 268435456ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 57>(268435456ull, 536870912ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 57>(268435456ull, 536870912ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 58>(536870912ull, 536870912ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 58>(536870912ull, 536870912ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 59>(536870912ull, 1073741824ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 59>(536870912ull, 1073741824ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 60>(1073741824ull, 1073741824ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 60>(1073741824ull, 1073741824ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 61>(1073741824ull, 2147483648ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 61>(1073741824ull, 2147483648ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 62>(2147483648ull, 2147483648ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 62>(2147483648ull, 2147483648ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);
  msr_u64 = multshiftround<uint64_t, 63>(2147483648ull, 4294967296ull);
  if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t, 63>(2147483648ull, 4294967296ull) returned %" PRIu64 ". expected 1.\n\n", msr_u64);

  std::printf("Multiplication tests finished.\n\n");

  /**
   * Test int8_t multshiftround for num on [-128, 127] and shift on [1, 6].
   */
  for (uint8_t shift = 1u; shift <= 6u; shift++)
  {
    std::printf("testing multshiftround<int8_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 6u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case  1:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 1>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 1>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case  2:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 2>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 2>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case  3:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 3>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 3>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case  4:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 4>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 4>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case  5:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 5>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 5>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case  6:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround<int8_t, 6>(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t, 6>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround<int8t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i8_Y for num on [-128, 127] and Y on [1, 6].
   */
  for (uint8_t shift = 1u; shift <= 6u; shift++)
  {
    std::printf("testing multshiftround_i8_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 6u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_1(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_1(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_2(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_2(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_3(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_3(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_4(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_4(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_5(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_5(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = multshiftround_i8_6(num, mul_i8);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8_6(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround<int8t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test int8_t shiftround for num on [-128, 127] and shift on [1, 6].
   */
  for (uint8_t shift = 1u; shift <= 6u; shift++)
  {
    std::printf("testing shiftround<int8_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 6u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 1>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 1>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 2>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 2>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 3>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 3>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 4>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 4>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 5>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 5>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround<int8_t, 6>(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t, 6>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround<int8t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i8_Y for num on [-128, 127] and Y on [1, 6].
   */
  for (uint8_t shift = 1u; shift <= 6u; shift++)
  {
    std::printf("testing shiftround_i8_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 6u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_1(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_1(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_2(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_2(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_3(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_3(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_4(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_4(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_5(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_5(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int8_t num = std::numeric_limits<int8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int8_t ms_res = shiftround_i8_6(num);
          int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8_6(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround_i8_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test int16_t multshiftround for num on [-32768, 32767] and shift on [1, 14].
   */
  for (uint8_t shift = 1u; shift <= 14u; shift++)
  {
    std::printf("testing multshiftround<int16_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 14u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 1>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 1>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 2>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 2>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 3>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 3>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 4>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 4>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 5>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 5>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 6>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 6>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 7>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 7>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 8>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 8>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 9>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 9>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 10>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 10>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 11>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 11>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 12>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 12>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 13>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 13>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround<int16_t, 14>(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t, 14>(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround<int16_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i16_Y for num on [-32768, 32767] and Y on [1, 14].
   */
  for (uint8_t shift = 1u; shift <= 14u; shift++)
  {
    std::printf("testing multshiftround_i16_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 14u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_1(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_1(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_2(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_2(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_3(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_3(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_4(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_4(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_5(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_5(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_6(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_6(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_7(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_7(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_8(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_8(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_9(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_9(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_10(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_10(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_11(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_11(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_12(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_12(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_13(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_13(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = multshiftround_i16_14(num, mul_i16);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16_14(num, mul): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround_i16_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test int16_t shiftround for num on [-32768, 32767] and shift on [1, 14].
   */
  for (uint8_t shift = 1u; shift <= 14u; shift++)
  {
    std::printf("testing shiftround<int16_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 14u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 1>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 1>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 2>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 2>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 3>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 3>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 4>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 4>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 5>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 5>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 6>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 6>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 7>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 7>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 8>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 8>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 9>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 9>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 10>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 10>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 11>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 11>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 12>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 12>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 13>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 13>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround<int16_t, 14>(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<int16_t, 14>(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround<int16_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i16_Y for num on [-32768, 32767] and Y on [1, 14].
   */
  for (uint8_t shift = 1u; shift <= 14u; shift++)
  {
    std::printf("testing shiftround_i16_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 14u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_1(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_1(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_2(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_2(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_3(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_3(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_4(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_4(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_5(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_5(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_6(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_6(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_7(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_7(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_8(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_8(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_9(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_9(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_10(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_10(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_11(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_11(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_12(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_12(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_13(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_13(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        int16_t num = std::numeric_limits<int16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          int16_t ms_res = shiftround_i16_14(num);
          int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_i16_14(num): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<int16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround_i16_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t multshiftround for num on [0, 255] and shift on [1, 7].
   */
  for (uint8_t shift = 1u; shift <= 7u; shift++)
  {
    std::printf("testing multshiftround<uint8_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 7u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 1>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 1>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 2>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 2>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 3>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 3>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 4>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 4>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 5>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 5>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 6>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 6>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround<uint8_t, 7>(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t, 7>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround<uint8_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u8_Y for num on [0, 255] and Y on [1, 7].
   */
  for (uint8_t shift = 1u; shift <= 7u; shift++)
  {
    std::printf("testing multshiftround_u8_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 7u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_1(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_1(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_2(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_2(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_3(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_3(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_4(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_4(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_5(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_5(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_6(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_6(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = multshiftround_u8_7(num, mul_i8);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8_7(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround_u8_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t shiftround for num on [0, 255] and shift on [1, 7].
   */
  for (uint8_t shift = 1u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround<uint8_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 7u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 1>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 1>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 2>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 2>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 3>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 3>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 4>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 4>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 5>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 5>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 6>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 6>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround<uint8_t, 7>(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t, 7>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround<uint8_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u8_Y for num on [0, 255] and Y on [1, 7].
   */
  for (uint8_t shift = 1u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround_u8_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 7u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_1(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_1(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_2(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_2(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_3(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_3(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_4(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_4(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_5(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_5(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_6(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_6(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint8_t num = std::numeric_limits<uint8_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint8_t ms_res = shiftround_u8_7(num);
          uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8_7(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint8_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround_u8_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t multshiftround for num on [0, 65535] and shift on [1, 15].
   */
  for (uint8_t shift = 1u; shift <= 15u; shift++)
  {
    std::printf("testing multshiftround<uint16_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 15u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 1>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 1>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 2>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 2>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 3>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 3>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 4>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 4>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 5>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 5>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 6>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 6>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 7>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 7>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 8>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 8>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 9>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 9>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 10>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 10>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 11>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 11>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 12>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 12>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 13>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 13>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 14>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 14>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 15:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround<uint16_t, 15>(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t, 15>(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround<uint16_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u16_Y for num on [0, 65535] and Y on [1, 15].
   */
  for (uint8_t shift = 1u; shift <= 15u; shift++)
  {
    std::printf("testing multshiftround_u16_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 15u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_1(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_1(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_2(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_2(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_3(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_3(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_4(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_4(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_5(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_5(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_6(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_6(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_7(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_7(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_8(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_8(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_9(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_9(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_10(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_10(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_11(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_11(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_12(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_12(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_13(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_13(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_14(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_14(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 15:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t ms_res = multshiftround_u16_15(num, mul_u16);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
          if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16_15(num, mul): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: multshiftround_u16_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t shiftround for num on [0, 65535] and shift on [1, 15].
   */
  for (uint8_t shift = 1u; shift <= 15u; shift++)
  {
    std::printf("testing shiftround<uint16_t, %u>()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 15u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 1>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 1>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 2>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 2>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 3>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 3>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 4>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 4>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 5>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 5>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 6>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 6>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 7>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 7>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 8>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 8>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 9>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 9>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 10>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 10>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 11>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 11>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 12>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 12>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 13>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 13>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 14>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 14>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 15:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround<uint16_t, 15>(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t, 15>(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround<uint16_t, ?""?>(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u16_Y for num on [0, 65535] and num on [1, 15].
   */
  for (uint8_t shift = 1u; shift <= 15u; shift++)
  {
    std::printf("testing shiftround_u16_%u()\n", shift);
    
    double dbl_inv_twoexp = std::numeric_limits<double>::quiet_NaN();
    if (shift >= 1u && shift <= 15u) dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);

    switch (shift)
    {
      case 1:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_1(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_1(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 2:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_2(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_2(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 3:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_3(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_3(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 4:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_4(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_4(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 5:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_5(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_5(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 6:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_6(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_6(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 7:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_7(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_7(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 8:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_8(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_8(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 9:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_9(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_9(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 10:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_10(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_10(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 11:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_11(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_11(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 12:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_12(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_12(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 13:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_13(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_13(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 14:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_14(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_14(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      case 15:
      {
        uint16_t num = std::numeric_limits<uint16_t>::lowest();
        double dbl_num = static_cast<double>(num);
        while (true)
        {
          uint16_t s_res = shiftround_u16_15(num);
          uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
          if (s_res != dbl_res) std::printf("ERROR: shiftround_u16_15(num): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
          if (num == std::numeric_limits<uint16_t>::max()) break;
          num++;
          dbl_num += 1.0;
        }
        break;
      }
      default: 
      {
        std::printf("ERROR: shiftround_u16_?""?(): invalid shift value %u\n", shift);
        break;
      }
    }
  }
  std::printf("\n");

  /**
   * vTests stores the list of tests to run multithreaded.
   * The first pair element function pointer should point to one of the 
   * test_...(uint8_t shift, size_t thread_index) functions.
   * The second pair element uint8_t is the shift argument value to use
   * when running the test.
   */
  std::vector<std::pair<void (*)(uint8_t, size_t), uint8_t> > vTests;

  /**
   * Queue all the tests that will be run multithreaded.
   */
  for (uint8_t shift = 1u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u64_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u64_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u64_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u64_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i64_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i64_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i64_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i64_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u32_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u32_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u32_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u32_comp_c, shift));
  
  for (uint8_t shift = 1u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i32_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i32_comp_c, shift));

  for (uint8_t shift = 1u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i32_comp_cpp, shift));

  for (uint8_t shift = 1u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i32_comp_c, shift));

  /**
   * Use one thread if only one hardware thread is available. Otherwise, use
   * one less than the number of available hardware threads.
   */
  uint32_t nThreads = std::thread::hardware_concurrency();
  if (nThreads <= 2u) nThreads = 1u;
  else nThreads--;

  std::printf("Starting multithreaded tests with %u threads.\n\n", nThreads);

  /**
   * Allocate and initialize the atomic bools for checking
   * when threads are done running tests.
   */
  thread_running = new std::atomic<bool>[nThreads];
  for (uint32_t jThread = 0u; jThread < nThreads; jThread++) {
    thread_running[jThread].store(false);
  }

  /**
   * Start all threads running with some test.
   */
  std::vector<std::thread> vThreads;
  for (uint32_t jThread = 0u; jThread < nThreads && !vTests.empty(); jThread++) {
    thread_running[jThread].store(true);
    vThreads.push_back(std::move(std::thread(vTests.back().first, vTests.back().second, jThread)));
    vTests.pop_back();
  }

  while (!vTests.empty()) {
    /**
     * Replace finished threads with new ones until there are no more
     * tests to run.
     */
    for (size_t jThread = 0ull; jThread < vThreads.size() && !vTests.empty(); jThread++) {
      if (!thread_running[jThread].load() && vThreads.at(jThread).joinable()) {
        vThreads.at(jThread).join();
        thread_running[jThread].store(true);
        vThreads.at(jThread) = std::move(std::thread(vTests.back().first, vTests.back().second, jThread));
        vTests.pop_back();
      }
    }
    /**
     * Sleep so as not to spam the CPU.
     */
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  /**
   * Wait until all threads are finished.
   */
  bool any_joinable = true;
  while (any_joinable) {
    any_joinable = false;
    for (size_t jThread = 0ull; jThread < vThreads.size(); jThread++) {
      if (vThreads.at(jThread).joinable()) {
        any_joinable = true;
        if (!thread_running[jThread].load())
          vThreads.at(jThread).join();
      }
    }
    /**
     * Sleep so as not to spam the CPU.
     */
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  delete[] thread_running;

  std::printf("\nFinished running multithreaded code.\n\n");
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