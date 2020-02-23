/**
 * test_multshiftround_shiftround_run.cpp
 * Unit tests for the multshiftround and shiftround functions that
 * evaluate the shift argument at runtime. These functions are tested
 * separately from those that require the shift argument at compile
 * time because they can be conditionally compiled to use rounding masks
 * based on array lookups (ARRAY_MASKS) or shift operations (COMPUTED_MASKS).
 * Consequently, this file will be compiled into two executables, one
 * to test each type of rounding mask.
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
 * Written in 2019 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
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
#include "multshiftround_run.hpp"
#include "shiftround_run.hpp"

#ifdef __cplusplus
  extern "C"
  {
#endif
    #include "multshiftround_run.h"
    #include "shiftround_run.h"
#ifdef __cplusplus
  }
#endif

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
 * Test c++ style runtime int32_t multshiftround for num on [-2147483648, 2147483647].
 * shift should range from 0 to 30.
 */
void test_multshiftround_i32_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<int32_t>(num, mul, %u)\n", shift);
  }
  
  if (shift > 30u)
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("ERROR: multshiftround<int32_t>(num, mul, ?""?): invalid shift value %u\n", shift);
  }
  else
  {
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int32_t ms_res = multshiftround<int32_t>(num, mul_i32, shift);
      int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
      if (ms_res != dbl_res) {
        std::lock_guard<std::mutex> print_lock(print_mutex);
        std::printf("ERROR: multshiftround<int32_t>(num, mul, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
      } 
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<int32_t>(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime int32_t multshiftround for num on [-2147483648, 2147483647].
 * shift should range from 0 to 30.
 */
void test_multshiftround_i32_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_i32(num, mul, %u)\n", shift);
  }

  if (shift > 30u)
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("ERROR: multshiftround_i32(num, mul, ?""?): invalid shift value %u\n", shift);
  }
  else
  {
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int32_t num = std::numeric_limits<int32_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int32_t ms_res = multshiftround_i32(num, mul_i32, shift);
      int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_mul_i32 * dbl_inv_twoexp));
      if (ms_res != dbl_res) {
        std::lock_guard<std::mutex> print_lock(print_mutex);
        std::printf("ERROR: multshiftround_i32(num, mul, %i): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i32 * dbl_inv_twoexp, num, mul_i32);
      }
      if (num == std::numeric_limits<int32_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_i32(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime int32_t shiftround for num on [-2147483648, 2147483647].
 * shift should range from 0 to 30.
 */
void test_shiftround_i32_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<int32_t>(num, %u)\n", shift);
  }

  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  int32_t num = std::numeric_limits<int32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    int32_t s_res = shiftround<int32_t>(num, shift);
    int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
    if (s_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<int32_t>(num, %u): s_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
    }
    if (num == std::numeric_limits<int32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<int32_t>(mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }
  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime int32_t shiftround for num on [-2147483648, 2147483647].
 * shift should range from 0 to 30.
 */
void test_shiftround_i32_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_i32(num, %u)\n", shift);
  }

  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  int32_t num = std::numeric_limits<int32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    int32_t s_res = shiftround_i32(num, shift);
    int32_t dbl_res = static_cast<int32_t>(std::round(dbl_num * dbl_inv_twoexp));
    if (s_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_i32(num, %u): s_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
    }
    if (num == std::numeric_limits<int32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_i32(mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }
  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime uint32_t multshiftround for num on [0, 4294967295].
 * shift should range from 0 to 31.
 */
void test_multshiftround_u32_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<uint32_t>(num, mul, %u)\n", shift);
  }

  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  uint32_t num = std::numeric_limits<uint32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    uint32_t ms_res = multshiftround<uint32_t>(num, mul_u32, shift);
    uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<uint32_t>(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u32 * dbl_inv_twoexp, num, mul_u32);
    }
    if (num == std::numeric_limits<uint32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<uint32_t>(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime uint32_t multshiftround for num on [0, 4294967295].
 * shift should range from 0 to 31.
 */
void test_multshiftround_u32_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_u32(num, mul, %u)\n", shift);
  }

  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  uint32_t num = std::numeric_limits<uint32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    uint32_t ms_res = multshiftround_u32(num, mul_u32, shift);
    uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_mul_u32 * dbl_inv_twoexp));
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_u32(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u32 * dbl_inv_twoexp, num, mul_u32);
    }
    if (num == std::numeric_limits<uint32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_u32(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime uint32_t shiftround for num on [0, 4294967295].
 * shift should range from 0 to 31.
 */
void test_shiftround_u32_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<uint32_t>(num, %u)\n", shift);
  }
    
  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  uint32_t num = std::numeric_limits<uint32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    uint32_t s_res = shiftround<uint32_t>(num, shift);
    uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
    if (s_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<uint32_t>(num, %u): s_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
    }
    if (num == std::numeric_limits<uint32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<uint32_t>(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime uint32_t shiftround for num on [0, 4294967295].
 * shift should range from 0 to 31.
 */
void test_shiftround_u32_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_u32(num, %u)\n", shift);
  }

  double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
  uint32_t num = std::numeric_limits<uint32_t>::lowest();
  double dbl_num = static_cast<double>(num);
  while (true)
  {
    uint32_t s_res = shiftround_u32(num, shift);
    uint32_t dbl_res = static_cast<uint32_t>(std::round(dbl_num * dbl_inv_twoexp));
    if (s_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_u32(num, %u): s_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
    }
    if (num == std::numeric_limits<uint32_t>::max()) break;
    num++;
    dbl_num += 1.0;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_u32(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime int64_t multshiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 0 to 62.
 */
void test_multshiftround_i64_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<int64_t>(num, mul, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  int64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  int64_t num = std::numeric_limits<int64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    int64_t ms_res = multshiftround<int64_t>(num, mul_i64, shift);
    int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<int64_t>(num, mul, %u): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
    }
    if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<int64_t>(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime int64_t multshiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 0 to 62.
 */
void test_multshiftround_i64_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_i64(num, mul, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  int64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  int64_t num = std::numeric_limits<int64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    int64_t ms_res = multshiftround_i64(num, mul_i64, shift);
    int64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<int64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_i64(num, mul, %u): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 ", mul %" PRIi64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_mul_i64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_i64);
    }
    if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_i64(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime int64_t shiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 0 to 62.
 */
void test_shiftround_i64_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<int64_t>(num, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  int64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  int64_t num = std::numeric_limits<int64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    int64_t ms_res = shiftround<int64_t>(num, shift);
    int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<int64_t>(num, %u): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
    }
    if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<int64_t>(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime int64_t shiftround for num on
 * [-9223372036854775808, 9223372036854775807]
 * in steps of 34359738367 for approximately 536,870,912 tests.
 * shift should range from 0 to 62.
 */
void test_shiftround_i64_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<int64_t>(num, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  int64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  int64_t num = std::numeric_limits<int64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    int64_t ms_res = shiftround_i64(num, shift);
    int64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<int64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_i64(num, %i): ms_res %" PRIi64 ", dbl_res %" PRIi64 ", dbl %.16f, num %" PRIi64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
    }
    if (num > 0ll && std::numeric_limits<int64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_i64(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime uint64_t multshiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_multshiftround_u64_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround<uint64_t>(num, mul, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  uint64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  uint64_t num = std::numeric_limits<uint64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    uint64_t ms_res = multshiftround<uint64_t>(num, mul_u64, shift);
    uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround<uint64_t>(num, mul, %u): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
    }
    if (std::numeric_limits<uint64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround<uint64_t>(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime uint64_t multshiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_multshiftround_u64_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing multshiftround_u64(num, mul, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  uint64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  uint64_t num = std::numeric_limits<uint64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    uint64_t ms_res = multshiftround_u64(num, mul_u64, shift);
    uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<uint64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: multshiftround_u64(num, mul, %u): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 ", mul %" PRIu64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_mul_u64 * ldbl_inv_twoexp).convert_to<double>(), num, mul_u64);
    }
    if (std::numeric_limits<uint64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  multshiftround_u64(num, mul, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c++ style runtime uint64_t shiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_shiftround_u64_run_cpp(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround<uint64_t>(num, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  uint64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  uint64_t num = std::numeric_limits<uint64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    uint64_t ms_res = shiftround<uint64_t>(num, shift);
    uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround<uint64_t>(num, %u): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
    }
    if (std::numeric_limits<uint64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround<uint64_t>(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

/**
 * Test c style runtime uint64_t shiftround for num on
 * [0, 18446744073709551615] in steps of 34359738367 for
 * approximately 536,870,912 tests.
 * shift should range from 0 to 63.
 */
void test_shiftround_u64_run_c(uint8_t shift, size_t thread_index) {
  std::chrono::high_resolution_clock::time_point test_start = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("testing shiftround_u64(num, %u)\n", shift);
  }

  cpp_bin_float_80 ldbl_inv_twoexp = cpp_bin_float_80(1.0) / cpp_bin_float_80(1ull << shift);
  uint64_t increment = (1ll << 35) - 1ll;
  cpp_bin_float_80 ldbl_increment(increment);
  uint64_t num = std::numeric_limits<uint64_t>::lowest();
  cpp_bin_float_80 ldbl_num(num);
  while (true)
  {
    uint64_t ms_res = shiftround_u64(num, shift);
    uint64_t dbl_res = boost::math::round(ldbl_num * ldbl_inv_twoexp).convert_to<uint64_t>();
    if (ms_res != dbl_res) {
      std::lock_guard<std::mutex> print_lock(print_mutex);
      std::printf("ERROR: shiftround_u64(num, %u): ms_res %" PRIu64 ", dbl_res %" PRIu64 ", dbl %.16f, num %" PRIu64 "\n", shift, ms_res, dbl_res, (ldbl_num * ldbl_inv_twoexp).convert_to<double>(), num);
    }
    if (std::numeric_limits<uint64_t>::max() - num < increment) break;
    num += increment;
    ldbl_num += ldbl_increment;
  }

  std::chrono::high_resolution_clock::time_point test_end = std::chrono::high_resolution_clock::now();
  {
    std::lock_guard<std::mutex> print_lock(print_mutex);
    std::printf("  shiftround_u64(num, %u) took %" PRIu64 " ms\n", shift, static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(test_end-test_start).count()));
  }

  thread_running[thread_index].store(false);
}

int main()
{
  /**
   * Result variables for initial quick multiplication and rounding tests.
   */
  int8_t msr_i8;
  uint8_t msr_u8;
  int16_t msr_i16;
  uint16_t msr_u16;
  int32_t msr_i32;
  uint32_t msr_u32;
  int64_t msr_i64;
  uint64_t msr_u64;

  std::printf("\nTesting multiplication operation in multshiftround routines.\n");

  for (uint8_t shift = 0u; shift <= 6u; shift++) {
    uint8_t half_shift = shift >> 1;
    int8_t result = 1;
    int8_t num = static_cast<int8_t>(1) << half_shift;
    if (num < 2) {
      num = 2;
      result = 2;
    }
    int8_t mul = static_cast<int8_t>(1) << (shift - half_shift);
    
    msr_i8 = multshiftround_i8(num, mul, shift);
    if (msr_i8 != result) std::printf("\nERROR: multshiftround_i8(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i8, result);

    msr_i8 = multshiftround<int8_t>(num, mul, shift);
    if (msr_i8 != result) std::printf("\nERROR: multshiftround<int8_t>(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i8, result);
  }

  for (uint8_t shift = 0u; shift <= 7u; shift++) {
    uint8_t half_shift = shift >> 1;
    uint8_t result = 1u;
    uint8_t num = static_cast<uint8_t>(1) << half_shift;
    if (num < 2u) {
      num = 2u;
      result = 2u;
    }
    uint8_t mul = static_cast<uint8_t>(1) << (shift - half_shift);
    
    msr_u8 = multshiftround_u8(num, mul, shift);
    if (msr_u8 != result) std::printf("\nERROR: multshiftround_u8(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u8, result);

    msr_u8 = multshiftround<uint8_t>(num, mul, shift);
    if (msr_u8 != result) std::printf("\nERROR: multshiftround<uint8_t>(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u8, result);
  }
  
  for (uint8_t shift = 0u; shift <= 14u; shift++) {
    uint8_t half_shift = shift >> 1;
    int16_t result = 1;
    int16_t num = static_cast<int16_t>(1) << half_shift;
    if (num < 2) {
      num = 2;
      result = 2;
    }
    int16_t mul = static_cast<int16_t>(1) << (shift - half_shift);
    
    msr_i16 = multshiftround_i16(num, mul, shift);
    if (msr_i16 != result) std::printf("\nERROR: multshiftround_i16(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i16, result);

    msr_i16 = multshiftround<int16_t>(num, mul, shift);
    if (msr_i16 != result) std::printf("\nERROR: multshiftround<int16_t>(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i16, result);
  }

  for (uint8_t shift = 0u; shift <= 15u; shift++) {
    uint8_t half_shift = shift >> 1;
    uint16_t result = 1u;
    uint16_t num = static_cast<uint16_t>(1) << half_shift;
    if (num < 2u) {
      num = 2u;
      result = 2u;
    }
    uint16_t mul = static_cast<uint16_t>(1) << (shift - half_shift);
    
    msr_u16 = multshiftround_u16(num, mul, shift);
    if (msr_u16 != result) std::printf("\nERROR: multshiftround_u16(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u16, result);

    msr_u16 = multshiftround<uint16_t>(num, mul, shift);
    if (msr_u16 != result) std::printf("\nERROR: multshiftround<uint16_t>(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u16, result);
  }

  for (uint8_t shift = 0u; shift <= 30u; shift++) {
    uint8_t half_shift = shift >> 1;
    int32_t result = 1;
    int32_t num = 1 << half_shift;
    if (num < 2) {
      num = 2;
      result = 2;
    }
    int32_t mul = 1 << (shift - half_shift);
    
    msr_i32 = multshiftround_i32(num, mul, shift);
    if (msr_i32 != result) std::printf("\nERROR: multshiftround_i32(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i32, result);

    msr_i32 = multshiftround<int32_t>(num, mul, shift);
    if (msr_i32 != result) std::printf("\nERROR: multshiftround<int32_t>(%i, %i, %u) returned %i. expected %i.\n\n", num, mul, shift, msr_i32, result);
  }

  for (uint8_t shift = 0u; shift <= 31u; shift++) {
    uint8_t half_shift = shift >> 1;
    uint32_t result = 1u;
    uint32_t num = 1u << half_shift;
    if (num < 2u) {
      num = 2u;
      result = 2u;
    }
    uint32_t mul = 1u << (shift - half_shift);
    
    msr_u32 = multshiftround_u32(num, mul, shift);
    if (msr_u32 != result) std::printf("\nERROR: multshiftround_u32(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u32, result);

    msr_u32 = multshiftround<uint32_t>(num, mul, shift);
    if (msr_u32 != result) std::printf("\nERROR: multshiftround<uint32_t>(%u, %u, %u) returned %u. expected %u.\n\n", num, mul, shift, msr_u32, result);
  }

  for (uint8_t shift = 0u; shift <= 62u; shift++) {
    uint8_t half_shift = shift >> 1;
    int64_t result = 1ll;
    int64_t num = 1ll << half_shift;
    if (num < 2ll) {
      num = 2ll;
      result = 2ll;
    }
    int64_t mul = 1ll << (shift - half_shift);
    
    msr_i64 = multshiftround_i64(num, mul, shift);
    if (msr_i64 != result) std::printf("\nERROR: multshiftround_i64(%" PRIi64 ", %" PRIi64 ", %u) returned %" PRIi64 ". expected %" PRIi64 ".\n\n", num, mul, shift, msr_i64, result);

    msr_i64 = multshiftround<int64_t>(num, mul, shift);
    if (msr_i64 != result) std::printf("\nERROR: multshiftround<int64_t>(%" PRIi64 ", %" PRIi64 ", %u) returned %" PRIi64 ". expected %" PRIi64 ".\n\n", num, mul, shift, msr_i64, result);
  }

  for (uint8_t shift = 0u; shift <= 63u; shift++) {
    uint8_t half_shift = shift >> 1;
    uint64_t result = 1ull;
    uint64_t num = 1ull << half_shift;
    if (num < 2ull) {
      num = 2ull;
      result = 2ull;
    }
    uint64_t mul = 1ull << (shift - half_shift);
    
    msr_u64 = multshiftround_u64(num, mul, shift);
    if (msr_u64 != result) std::printf("\nERROR: multshiftround_u64(%" PRIu64 ", %" PRIu64 ", %u) returned %" PRIu64 ". expected %" PRIu64 ".\n\n", num, mul, shift, msr_u64, result);

    msr_u64 = multshiftround<uint64_t>(num, mul, shift);
    if (msr_u64 != result) std::printf("\nERROR: multshiftround<uint64_t>(%" PRIu64 ", %" PRIu64 ", %u) returned %" PRIu64 ". expected %" PRIu64 ".\n\n", num, mul, shift, msr_u64, result);
  }
  
  std::printf("Multiplication tests finished.\n\n");
  
  std::printf("Running quick tests of rounding operation in multshiftround and shiftround routines.\n");

  for (uint8_t shift = 1u; shift <= 6u; shift++) {
    int8_t num = -(1 << (shift-1u));
    msr_i8 = multshiftround_i8(num, 1, shift);
    if (msr_i8 != -1) std::printf("\nERROR: multshiftround_i8(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i8);
    msr_i8 = multshiftround<int8_t>(num, 1, shift);
    if (msr_i8 != -1) std::printf("\nERROR: multshiftround<int8_t>(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i8);

    num = -(1 << (shift-1u)) + 1;
    msr_i8 = multshiftround_i8(num, 1, shift);
    if (msr_i8 != 0) std::printf("\nERROR: multshiftround_i8(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);
    msr_i8 = multshiftround<int8_t>(num, 1, shift);
    if (msr_i8 != 0) std::printf("\nERROR: multshiftround<int8_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);

    num = (1 << (shift-1u)) - 1;
    msr_i8 = multshiftround_i8(num, 1, shift);
    if (msr_i8 != 0) std::printf("\nERROR: multshiftround_i8(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);
    msr_i8 = multshiftround<int8_t>(num, 1, shift);
    if (msr_i8 != 0) std::printf("\nERROR: multshiftround<int8_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);

    num = 1 << (shift-1u);
    msr_i8 = multshiftround_i8(num, 1, shift);
    if (msr_i8 != 1) std::printf("\nERROR: multshiftround_i8(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i8);
    msr_i8 = multshiftround<int8_t>(num, 1, shift);
    if (msr_i8 != 1) std::printf("\nERROR: multshiftround<int8_t>(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i8);
  }

  for (uint8_t shift = 1u; shift <= 14u; shift++) {
    int16_t num = -(1 << (shift-1u));
    msr_i16 = multshiftround_i16(num, 1, shift);
    if (msr_i16 != -1) std::printf("\nERROR: multshiftround_i16(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i16);
    msr_i16 = multshiftround<int16_t>(num, 1, shift);
    if (msr_i16 != -1) std::printf("\nERROR: multshiftround<int16_t>(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i16);

    num = -(1 << (shift-1u)) + 1;
    msr_i16 = multshiftround_i16(num, 1, shift);
    if (msr_i16 != 0) std::printf("\nERROR: multshiftround_i16(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);
    msr_i16 = multshiftround<int16_t>(num, 1, shift);
    if (msr_i16 != 0) std::printf("\nERROR: multshiftround<int16_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);

    num = (1 << (shift-1u)) - 1;
    msr_i16 = multshiftround_i16(num, 1, shift);
    if (msr_i16 != 0) std::printf("\nERROR: multshiftround_i16(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);
    msr_i16 = multshiftround<int16_t>(num, 1, shift);
    if (msr_i16 != 0) std::printf("\nERROR: multshiftround<int16_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);

    num = 1 << (shift-1u);
    msr_i16 = multshiftround_i16(num, 1, shift);
    if (msr_i16 != 1) std::printf("\nERROR: multshiftround_i16(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i16);
    msr_i16 = multshiftround<int16_t>(num, 1, shift);
    if (msr_i16 != 1) std::printf("\nERROR: multshiftround<int16_t>(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i16);
  }

  for (uint8_t shift = 1u; shift <= 30u; shift++) {
    int32_t num = -(1 << (shift-1u));
    msr_i32 = multshiftround_i32(num, 1, shift);
    if (msr_i32 != -1) std::printf("\nERROR: multshiftround_i32(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i32);
    msr_i32 = multshiftround<int32_t>(num, 1, shift);
    if (msr_i32 != -1) std::printf("\nERROR: multshiftround<int32_t>(%i, 1, %u) returned %i. expected -1.\n\n", num, shift, msr_i32);

    num = -(1 << (shift-1u)) + 1;
    msr_i32 = multshiftround_i32(num, 1, shift);
    if (msr_i32 != 0) std::printf("\nERROR: multshiftround_i32(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);
    msr_i32 = multshiftround<int32_t>(num, 1, shift);
    if (msr_i32 != 0) std::printf("\nERROR: multshiftround<int32_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);

    num = (1 << (shift-1u)) - 1;
    msr_i32 = multshiftround_i32(num, 1, shift);
    if (msr_i32 != 0) std::printf("\nERROR: multshiftround_i32(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);
    msr_i32 = multshiftround<int32_t>(num, 1, shift);
    if (msr_i32 != 0) std::printf("\nERROR: multshiftround<int32_t>(%i, 1, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);

    num = 1 << (shift-1u);
    msr_i32 = multshiftround_i32(num, 1, shift);
    if (msr_i32 != 1) std::printf("\nERROR: multshiftround_i32(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i32);
    msr_i32 = multshiftround<int32_t>(num, 1, shift);
    if (msr_i32 != 1) std::printf("\nERROR: multshiftround<int32_t>(%i, 1, %u) returned %i. expected 1.\n\n", num, shift, msr_i32);
  }

  for (uint8_t shift = 1u; shift <= 62u; shift++) {
    int64_t num = -(1ll << (shift-1u));
    msr_i64 = multshiftround_i64(num, 1ll, shift);
    if (msr_i64 != -1ll) std::printf("\nERROR: multshiftround_i64(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected -1.\n\n", num, shift, msr_i64);
    msr_i64 = multshiftround<int64_t>(num, 1ll, shift);
    if (msr_i64 != -1ll) std::printf("\nERROR: multshiftround<int64_t>(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected -1.\n\n", num, shift, msr_i64);

    num = -(1ll << (shift-1u)) + 1ll;
    msr_i64 = multshiftround_i64(num, 1ll, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: multshiftround_i64(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);
    msr_i64 = multshiftround<int64_t>(num, 1ll, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: multshiftround<int64_t>(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);

    num = (1ll << (shift-1u)) - 1ll;
    msr_i64 = multshiftround_i64(num, 1ll, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: multshiftround_i64(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);
    msr_i64 = multshiftround<int64_t>(num, 1ll, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: multshiftround<int64_t>(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);

    num = 1ll << (shift-1u);
    msr_i64 = multshiftround_i64(num, 1ll, shift);
    if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround_i64(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 1.\n\n", num, shift, msr_i64);
    msr_i64 = multshiftround<int64_t>(num, 1ll, shift);
    if (msr_i64 != 1ll) std::printf("\nERROR: multshiftround<int64_t>(%" PRIi64 ", 1, %u) returned %" PRIi64 ". expected 1.\n\n", num, shift, msr_i64);
  }

  for (uint8_t shift = 1u; shift <= 7u; shift++) {
    uint8_t num = (1u << (shift-1u)) - 1u;
    msr_u8 = multshiftround_u8(num, 1u, shift);
    if (msr_u8 != 0u) std::printf("\nERROR: multshiftround_u8(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u8);
    msr_u8 = multshiftround<uint8_t>(num, 1u, shift);
    if (msr_u8 != 0u) std::printf("\nERROR: multshiftround<uint8_t>(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u8);

    num = 1u << (shift-1u);
    msr_u8 = multshiftround_u8(num, 1u, shift);
    if (msr_u8 != 1u) std::printf("\nERROR: multshiftround_u8(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u8);
    msr_u8 = multshiftround<uint8_t>(num, 1u, shift);
    if (msr_u8 != 1u) std::printf("\nERROR: multshiftround<uint8_t>(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u8);
  }

  for (uint8_t shift = 1u; shift <= 15u; shift++) {
    uint16_t num = (1u << (shift-1u)) - 1u;
    msr_u16 = multshiftround_u16(num, 1u, shift);
    if (msr_u16 != 0u) std::printf("\nERROR: multshiftround_u16(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u16);
    msr_u16 = multshiftround<uint16_t>(num, 1u, shift);
    if (msr_u16 != 0u) std::printf("\nERROR: multshiftround<uint16_t>(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u16);

    num = 1u << (shift-1u);
    msr_u16 = multshiftround_u16(num, 1u, shift);
    if (msr_u16 != 1u) std::printf("\nERROR: multshiftround_u16(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u16);
    msr_u16 = multshiftround<uint16_t>(num, 1u, shift);
    if (msr_u16 != 1u) std::printf("\nERROR: multshiftround<uint16_t>(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u16);
  }

  for (uint8_t shift = 1u; shift <= 31u; shift++) {
    uint32_t num = (1u << (shift-1u)) - 1u;
    msr_u32 = multshiftround_u32(num, 1u, shift);
    if (msr_u32 != 0u) std::printf("\nERROR: multshiftround_u32(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u32);
    msr_u32 = multshiftround<uint32_t>(num, 1u, shift);
    if (msr_u32 != 0u) std::printf("\nERROR: multshiftround<uint32_t>(%u, 1, %u) returned %u. expected 0.\n\n", num, shift, msr_u32);

    num = 1u << (shift-1u);
    msr_u32 = multshiftround_u32(num, 1u, shift);
    if (msr_u32 != 1u) std::printf("\nERROR: multshiftround_u32(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u32);
    msr_u32 = multshiftround<uint32_t>(num, 1u, shift);
    if (msr_u32 != 1u) std::printf("\nERROR: multshiftround<uint32_t>(%u, 1, %u) returned %u. expected 1.\n\n", num, shift, msr_u32);
  }

  for (uint8_t shift = 1u; shift <= 63u; shift++) {
    uint64_t num = (1ull << (shift-1u)) - 1ull;
    msr_u64 = multshiftround_u64(num, 1ull, shift);
    if (msr_u64 != 0ull) std::printf("\nERROR: multshiftround_u64(%" PRIu64 ", 1, %u) returned %" PRIu64 ". expected 0.\n\n", num, shift, msr_u64);
    msr_u64 = multshiftround<uint64_t>(num, 1ull, shift);
    if (msr_u64 != 0ull) std::printf("\nERROR: multshiftround<uint64_t>(%" PRIu64 ", 1, %u) returned %" PRIu64 ". expected 0.\n\n", num, shift, msr_u64);

    num = 1ull << (shift-1u);
    msr_u64 = multshiftround_u64(num, 1ull, shift);
    if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround_u64(%" PRIu64 ", 1, %u) returned %" PRIu64 ". expected 1.\n\n", num, shift, msr_u64);
    msr_u64 = multshiftround<uint64_t>(num, 1ull, shift);
    if (msr_u64 != 1ull) std::printf("\nERROR: multshiftround<uint64_t>(%" PRIu64 ", 1, %u) returned %" PRIu64 ". expected 1.\n\n", num, shift, msr_u64);
  }

  for (uint8_t shift = 1u; shift <= 6u; shift++) {
    int8_t num = -(1 << (shift-1u));
    msr_i8 = shiftround_i8(num, shift);
    if (msr_i8 != -1) std::printf("\nERROR: shiftround_i8(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i8);
    msr_i8 = shiftround<int8_t>(num, shift);
    if (msr_i8 != -1) std::printf("\nERROR: shiftround<int8_t>(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i8);

    num = -(1 << (shift-1u)) + 1;
    msr_i8 = shiftround_i8(num, shift);
    if (msr_i8 != 0) std::printf("\nERROR: shiftround_i8(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);
    msr_i8 = shiftround<int8_t>(num, shift);
    if (msr_i8 != 0) std::printf("\nERROR: shiftround<int8_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);

    num = (1 << (shift-1u)) - 1;
    msr_i8 = shiftround_i8(num, shift);
    if (msr_i8 != 0) std::printf("\nERROR: shiftround_i8(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);
    msr_i8 = shiftround<int8_t>(num, shift);
    if (msr_i8 != 0) std::printf("\nERROR: shiftround<int8_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i8);

    num = 1 << (shift-1u);
    msr_i8 = shiftround_i8(num, shift);
    if (msr_i8 != 1) std::printf("\nERROR: shiftround_i8(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i8);
    msr_i8 = shiftround<int8_t>(num, shift);
    if (msr_i8 != 1) std::printf("\nERROR: shiftround<int8_t>(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i8);
  }

  for (uint8_t shift = 1u; shift <= 14u; shift++) {
    int16_t num = -(1 << (shift-1u));
    msr_i16 = shiftround_i16(num, shift);
    if (msr_i16 != -1) std::printf("\nERROR: shiftround_i16(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i16);
    msr_i16 = shiftround<int16_t>(num, shift);
    if (msr_i16 != -1) std::printf("\nERROR: shiftround<int16_t>(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i16);

    num = -(1 << (shift-1u)) + 1;
    msr_i16 = shiftround_i16(num, shift);
    if (msr_i16 != 0) std::printf("\nERROR: shiftround_i16(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);
    msr_i16 = shiftround<int16_t>(num, shift);
    if (msr_i16 != 0) std::printf("\nERROR: shiftround<int16_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);

    num = (1 << (shift-1u)) - 1;
    msr_i16 = shiftround_i16(num, shift);
    if (msr_i16 != 0) std::printf("\nERROR: shiftround_i16(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);
    msr_i16 = shiftround<int16_t>(num, shift);
    if (msr_i16 != 0) std::printf("\nERROR: shiftround<int16_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i16);

    num = 1 << (shift-1u);
    msr_i16 = shiftround_i16(num, shift);
    if (msr_i16 != 1) std::printf("\nERROR: shiftround_i16(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i16);
    msr_i16 = shiftround<int16_t>(num, shift);
    if (msr_i16 != 1) std::printf("\nERROR: shiftround<int16_t>(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i16);
  }

  for (uint8_t shift = 1u; shift <= 30u; shift++) {
    int32_t num = -(1 << (shift-1u));
    msr_i32 = shiftround_i32(num, shift);
    if (msr_i32 != -1) std::printf("\nERROR: shiftround_i32(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i32);
    msr_i32 = shiftround<int32_t>(num, shift);
    if (msr_i32 != -1) std::printf("\nERROR: shiftround<int32_t>(%i, %u) returned %i. expected -1.\n\n", num, shift, msr_i32);

    num = -(1 << (shift-1u)) + 1;
    msr_i32 = shiftround_i32(num, shift);
    if (msr_i32 != 0) std::printf("\nERROR: shiftround_i32(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);
    msr_i32 = shiftround<int32_t>(num, shift);
    if (msr_i32 != 0) std::printf("\nERROR: shiftround<int32_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);

    num = (1 << (shift-1u)) - 1;
    msr_i32 = shiftround_i32(num, shift);
    if (msr_i32 != 0) std::printf("\nERROR: shiftround_i32(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);
    msr_i32 = shiftround<int32_t>(num, shift);
    if (msr_i32 != 0) std::printf("\nERROR: shiftround<int32_t>(%i, %u) returned %i. expected 0.\n\n", num, shift, msr_i32);

    num = 1 << (shift-1u);
    msr_i32 = shiftround_i32(num, shift);
    if (msr_i32 != 1) std::printf("\nERROR: shiftround_i32(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i32);
    msr_i32 = shiftround<int32_t>(num, shift);
    if (msr_i32 != 1) std::printf("\nERROR: shiftround<int32_t>(%i, %u) returned %i. expected 1.\n\n", num, shift, msr_i32);
  }

  for (uint8_t shift = 1u; shift <= 62u; shift++) {
    int64_t num = -(1ll << (shift-1u));
    msr_i64 = shiftround_i64(num, shift);
    if (msr_i64 != -1ll) std::printf("\nERROR: shiftround_i64(%" PRIi64 ", %u) returned %" PRIi64 ". expected -1.\n\n", num, shift, msr_i64);
    msr_i64 = shiftround<int64_t>(num, shift);
    if (msr_i64 != -1ll) std::printf("\nERROR: shiftround<int64_t>(%" PRIi64 ", %u) returned %" PRIi64 ". expected -1.\n\n", num, shift, msr_i64);

    num = -(1ll << (shift-1u)) + 1ll;
    msr_i64 = shiftround_i64(num, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: shiftround_i64(%" PRIi64 ", %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);
    msr_i64 = shiftround<int64_t>(num, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: shiftround<int64_t>(%" PRIi64 ", %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);

    num = (1ll << (shift-1u)) - 1ll;
    msr_i64 = shiftround_i64(num, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: shiftround_i64(%" PRIi64 ", %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);
    msr_i64 = shiftround<int64_t>(num, shift);
    if (msr_i64 != 0ll) std::printf("\nERROR: shiftround<int64_t>(%" PRIi64 ", %u) returned %" PRIi64 ". expected 0.\n\n", num, shift, msr_i64);

    num = 1ll << (shift-1u);
    msr_i64 = shiftround_i64(num, shift);
    if (msr_i64 != 1ll) std::printf("\nERROR: shiftround_i64(%" PRIi64 ", %u) returned %" PRIi64 ". expected 1.\n\n", num, shift, msr_i64);
    msr_i64 = shiftround<int64_t>(num, shift);
    if (msr_i64 != 1ll) std::printf("\nERROR: shiftround<int64_t>(%" PRIi64 ", %u) returned %" PRIi64 ". expected 1.\n\n", num, shift, msr_i64);
  }

  for (uint8_t shift = 1u; shift <= 7u; shift++) {
    uint8_t num = (1u << (shift-1u)) - 1u;
    msr_u8 = shiftround_u8(num, shift);
    if (msr_u8 != 0u) std::printf("\nERROR: shiftround_u8(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u8);
    msr_u8 = shiftround<uint8_t>(num, shift);
    if (msr_u8 != 0u) std::printf("\nERROR: shiftround<uint8_t>(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u8);

    num = 1u << (shift-1u);
    msr_u8 = shiftround_u8(num, shift);
    if (msr_u8 != 1u) std::printf("\nERROR: shiftround_u8(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u8);
    msr_u8 = shiftround<uint8_t>(num, shift);
    if (msr_u8 != 1u) std::printf("\nERROR: shiftround<uint8_t>(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u8);
  }

  for (uint8_t shift = 1u; shift <= 15u; shift++) {
    uint16_t num = (1u << (shift-1u)) - 1u;
    msr_u16 = shiftround_u16(num, shift);
    if (msr_u16 != 0u) std::printf("\nERROR: shiftround_u16(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u16);
    msr_u16 = shiftround<uint16_t>(num, shift);
    if (msr_u16 != 0u) std::printf("\nERROR: shiftround<uint16_t>(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u16);

    num = 1u << (shift-1u);
    msr_u16 = shiftround_u16(num, shift);
    if (msr_u16 != 1u) std::printf("\nERROR: shiftround_u16(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u16);
    msr_u16 = shiftround<uint16_t>(num, shift);
    if (msr_u16 != 1u) std::printf("\nERROR: shiftround<uint16_t>(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u16);
  }

  for (uint8_t shift = 1u; shift <= 31u; shift++) {
    uint32_t num = (1u << (shift-1u)) - 1u;
    msr_u32 = shiftround_u32(num, shift);
    if (msr_u32 != 0u) std::printf("\nERROR: shiftround_u32(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u32);
    msr_u32 = shiftround<uint32_t>(num, shift);
    if (msr_u32 != 0u) std::printf("\nERROR: shiftround<uint32_t>(%u, %u) returned %u. expected 0.\n\n", num, shift, msr_u32);

    num = 1u << (shift-1u);
    msr_u32 = shiftround_u32(num, shift);
    if (msr_u32 != 1u) std::printf("\nERROR: shiftround_u32(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u32);
    msr_u32 = shiftround<uint32_t>(num, shift);
    if (msr_u32 != 1u) std::printf("\nERROR: shiftround<uint32_t>(%u, %u) returned %u. expected 1.\n\n", num, shift, msr_u32);
  }

  for (uint8_t shift = 1u; shift <= 63u; shift++) {
    uint64_t num = (1ull << (shift-1u)) - 1ull;
    msr_u64 = shiftround_u64(num, shift);
    if (msr_u64 != 0ull) std::printf("\nERROR: shiftround_u64(%" PRIu64 ", %u) returned %" PRIu64 ". expected 0.\n\n", num, shift, msr_u64);
    msr_u64 = shiftround<uint64_t>(num, shift);
    if (msr_u64 != 0ull) std::printf("\nERROR: shiftround<uint64_t>(%" PRIu64 ", %u) returned %" PRIu64 ". expected 0.\n\n", num, shift, msr_u64);

    num = 1ull << (shift-1u);
    msr_u64 = shiftround_u64(num, shift);
    if (msr_u64 != 1ull) std::printf("\nERROR: shiftround_u64(%" PRIu64 ", %u) returned %" PRIu64 ". expected 1.\n\n", num, shift, msr_u64);
    msr_u64 = shiftround<uint64_t>(num, shift);
    if (msr_u64 != 1ull) std::printf("\nERROR: shiftround<uint64_t>(%" PRIu64 ", %u) returned %" PRIu64 ". expected 1.\n\n", num, shift, msr_u64);
  }

  std::printf("Quick tests of rounding operation finished.\n\n");

  /**
   * Test int8_t multshiftround for num on [-128, 127] and shift on [0, 6].
   */
  for (uint8_t shift = 0u; shift <= 6u; shift++)
  {
    std::printf("testing multshiftround<int8_t>(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int8_t ms_res = multshiftround<int8_t>(num, mul_i8, shift);
      int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int8_t>(num, mul, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i8 for num on [-128, 127] and shift on [0, 6].
   */
  for (uint8_t shift = 0u; shift <= 6u; shift++)
  {
    std::printf("testing multshiftround_i8(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int8_t ms_res = multshiftround_i8(num, mul_i8, shift);
      int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_mul_i8 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i8(num, mul, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i8 * dbl_inv_twoexp, num, mul_i8);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test int8_t shiftround for num on [-128, 127] and shift on [0, 6].
   */
  for (uint8_t shift = 0u; shift <= 6u; shift++)
  {
    std::printf("testing shiftround<int8_t>(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int8_t ms_res = shiftround<int8_t>(num, shift);
      int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround<int8_t>(num, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i8 for num on [-128, 127] and shift on [0, 6].
   */
  for (uint8_t shift = 0u; shift <= 6u; shift++)
  {
    std::printf("testing shiftround_i8(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int8_t num = std::numeric_limits<int8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int8_t ms_res = shiftround_i8(num, shift);
      int8_t dbl_res = static_cast<int8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround_i8(num, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<int8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test int16_t multshiftround for num on [-32768, 32767] and shift on [0, 14].
   */
  for (uint8_t shift = 0u; shift <= 14u; shift++)
  {
    std::printf("testing multshiftround<int16_t>(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int16_t ms_res = multshiftround<int16_t>(num, mul_i16, shift);
      int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<int16_t>(num, mul, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_i16 for num on [-32768, 32767] and shift on [0, 14].
   */
  for (uint8_t shift = 0u; shift <= 14u; shift++)
  {
    std::printf("testing multshiftround_i16(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int16_t ms_res = multshiftround_i16(num, mul_i16, shift);
      int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_mul_i16 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_i16(num, mul, %u): ms_res %i, dbl_res %i, dbl %.16f, num %i, mul %i\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_i16 * dbl_inv_twoexp, num, mul_i16);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test int16_t shiftround for num on [-32768, 32767] and shift on [0, 14].
   */
  for (uint8_t shift = 0u; shift <= 14u; shift++)
  {
    std::printf("testing shiftround<int16_t>(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int16_t s_res = shiftround<int16_t>(num, shift);
      int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<int16_t>(num, %u): s_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_i16 for num on [-32768, 32767] and shift on [0, 14].
   */
  for (uint8_t shift = 0u; shift <= 14u; shift++)
  {
    std::printf("testing shiftround_i16(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    int16_t num = std::numeric_limits<int16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      int16_t s_res = shiftround_i16(num, shift);
      int16_t dbl_res = static_cast<int16_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_i16(num, %u): s_res %i, dbl_res %i, dbl %.16f, num %i\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<int16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t multshiftround for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing multshiftround<uint8_t>(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = multshiftround<uint8_t>(num, mul_u8, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint8_t>(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u8 for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing multshiftround_u8(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = multshiftround_u8(num, mul_u8, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_mul_u8 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u8(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u8 * dbl_inv_twoexp, num, mul_u8);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t shiftround for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround<uint8_t>(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = shiftround<uint8_t>(num, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t>(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u8 for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround_u8(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = shiftround_u8(num, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test uint8_t shiftround for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround<uint8_t>(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = shiftround<uint8_t>(num, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround<uint8_t>(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u8 for num on [0, 255] and shift on [0, 7].
   */
  for (uint8_t shift = 0u; shift <= 7u; shift++)
  {
    std::printf("testing shiftround_u8(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint8_t num = std::numeric_limits<uint8_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint8_t ms_res = shiftround_u8(num, shift);
      uint8_t dbl_res = static_cast<uint8_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: shiftround_u8(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, ms_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint8_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t multshiftround for num on [0, 65535] and shift on [0, 15].
   */
  for (uint8_t shift = 0u; shift <= 15u; shift++)
  {
    std::printf("testing multshiftround<uint16_t>(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint16_t ms_res = multshiftround<uint16_t>(num, mul_u16, shift);
      uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround<uint16_t>(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test multshiftround_u16 for num on [0, 65535] and shift on [0, 15].
   */
  for (uint8_t shift = 0u; shift <= 15u; shift++)
  {
    std::printf("testing multshiftround_u16(num, mul, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint16_t ms_res = multshiftround_u16(num, mul_u16, shift);
      uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_mul_u16 * dbl_inv_twoexp));
      if (ms_res != dbl_res) std::printf("ERROR: multshiftround_u16(num, mul, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u, mul %u\n", shift, ms_res, dbl_res, dbl_num * dbl_mul_u16 * dbl_inv_twoexp, num, mul_u16);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test uint16_t shiftround for num on [0, 65535] and shift on [0, 15].
   */
  for (uint8_t shift = 0u; shift <= 15u; shift++)
  {
    std::printf("testing shiftround<uint16_t>(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint16_t s_res = shiftround<uint16_t>(num, shift);
      uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround<uint16_t>(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
      dbl_num += 1.0;
    }
  }
  std::printf("\n");

  /**
   * Test shiftround_u16 for num on [0, 65535] and shift on [0, 15].
   */
  for (uint8_t shift = 0u; shift <= 15u; shift++)
  {
    std::printf("testing shiftround_u16(num, %u)\n", shift);
    double dbl_inv_twoexp = 1.0 / static_cast<double>(1ull << shift);
    uint16_t num = std::numeric_limits<uint16_t>::lowest();
    double dbl_num = static_cast<double>(num);
    while (true)
    {
      uint16_t s_res = shiftround_u16(num, shift);
      uint16_t dbl_res = static_cast<uint16_t>(std::round(dbl_num * dbl_inv_twoexp));
      if (s_res != dbl_res) std::printf("ERROR: shiftround_u16(num, %u): ms_res %u, dbl_res %u, dbl %.16f, num %u\n", shift, s_res, dbl_res, dbl_num * dbl_inv_twoexp, num);
      if (num == std::numeric_limits<uint16_t>::max()) break;
      num++;
      dbl_num += 1.0;
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
  for (uint8_t shift = 0u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u64_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u64_run_c, shift));

  for (uint8_t shift = 0u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u64_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 63u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u64_run_c, shift));

  for (uint8_t shift = 0u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i64_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i64_run_c, shift));

  for (uint8_t shift = 0u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i64_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 62u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i64_run_c, shift));

  for (uint8_t shift = 0u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u32_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_u32_run_c, shift));

  for (uint8_t shift = 0u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u32_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 31u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_u32_run_c, shift));

  for (uint8_t shift = 0u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i32_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_shiftround_i32_run_c, shift));

  for (uint8_t shift = 0u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i32_run_cpp, shift));

  for (uint8_t shift = 0u; shift <= 30u; shift++)
    vTests.push_back(std::make_pair(test_multshiftround_i32_run_c, shift));

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
