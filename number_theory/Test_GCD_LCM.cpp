/**
 * Test_GCD_LCM.cpp
 * Rudimentary test code to exercise the functions implemented in
 * gcd_lcm.hpp
 * 
 * Written in 2020 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "gcd_lcm.hpp"
#include <cstdio>
#include <vector>

int main()
{
  std::printf("Running tests on the functions in gcd_lcm.hpp...\n\n");

  uint8_t result8 = 0;
  uint16_t result16 = 0;
  uint32_t result32 = 0;
  uint64_t result64 = 0;

  std::vector<uint8_t> v8{0,0,0};
  std::vector<uint16_t> v16{0,0,0};
  std::vector<uint32_t> v32{0,0,0};
  std::vector<uint64_t> v64{0,0,0};

  /**
   * Test all-zero inputs.
   */
  result8 = gcd<uint8_t>(0,0);
  if (result8 != 0) std::printf("Error: gcd<uint8_t>(0,0) returns %u; expected 0.\n", result8);
  result16 = gcd<uint16_t>(0,0);
  if (result16 != 0) std::printf("Error: gcd<uint16_t>(0,0) returns %u; expected 0.\n", result16);
  result32 = gcd<uint32_t>(0u,0u);
  if (result32 != 0u) std::printf("Error: gcd<uint32_t>(0,0) returns %u; expected 0.\n", result32);
  result64 = gcd<uint64_t>(0ull,0ull);
  if (result64 != 0ull) std::printf("Error: gcd<uint64_t>(0,0) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = gcd<uint8_t>({0,0,0});
  if (result8 != 0) std::printf("Error: gcd<uint8_t>({0,0,0}) returns %u; expected 0.\n", result8);
  result16 = gcd<uint16_t>({0,0,0});
  if (result16 != 0) std::printf("Error: gcd<uint16_t>({0,0,0}) returns %u; expected 0.\n", result16);
  result32 = gcd<uint32_t>({0u,0u,0u});
  if (result32 != 0u) std::printf("Error: gcd<uint32_t>({0,0,0}) returns %u; expected 0.\n", result32);
  result64 = gcd<uint64_t>({0ull,0ull,0ull});
  if (result64 != 0ull) std::printf("Error: gcd<uint64_t>({0,0,0}) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 0) std::printf("Error: gcd(v8.cbegin(), v8.cend()) all-zero returns %u; expected 0.\n", result8);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 0) std::printf("Error: gcd(v16.cbegin(), v16.cend()) all-zero returns %u; expected 0.\n", result16);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 0u) std::printf("Error: gcd(v32.cbegin(), v32.cend()) all-zero returns %u; expected 0.\n", result32);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 0ull) std::printf("Error: gcd(v64.cbegin(), v64.cend()) all-zero returns %" PRIu64 "; expected 0.\n", result64);

  result8 = lcm<uint8_t>(0,0);
  if (result8 != 0) std::printf("Error: lcm<uint8_t>(0,0) returns %u; expected 0.\n", result8);
  result16 = lcm<uint16_t>(0,0);
  if (result16 != 0) std::printf("Error: lcm<uint16_t>(0,0) returns %u; expected 0.\n", result16);
  result32 = lcm<uint32_t>(0u,0u);
  if (result32 != 0u) std::printf("Error: lcm<uint32_t>(0,0) returns %u; expected 0.\n", result32);
  result64 = lcm<uint64_t>(0ull,0ull);
  if (result64 != 0ull) std::printf("Error: lcm<uint64_t>(0,0) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = lcm<uint8_t>({0,0,0});
  if (result8 != 0) std::printf("Error: lcm<uint8_t>({0,0,0}) returns %u; expected 0.\n", result8);
  result16 = lcm<uint16_t>({0,0,0});
  if (result16 != 0) std::printf("Error: lcm<uint16_t>({0,0,0}) returns %u; expected 0.\n", result16);
  result32 = lcm<uint32_t>({0u,0u,0u});
  if (result32 != 0u) std::printf("Error: lcm<uint32_t>({0,0,0}) returns %u; expected 0.\n", result32);
  result64 = lcm<uint64_t>({0ull,0ull,0ull});
  if (result64 != 0ull) std::printf("Error: lcm<uint64_t>({0,0,0}) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 0) std::printf("Error: lcm(v8.cbegin(), v8.cend()) returns %u; expected 0.\n", result8);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 0) std::printf("Error: lcm(v16.cbegin(), v16.cend()) returns %u; expected 0.\n", result16);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 0u) std::printf("Error: lcm(v32.cbegin(), v32.cend()) returns %u; expected 0.\n", result32);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 0ull) std::printf("Error: lcm(v64.cbegin(), v64.cend()) returns %" PRIu64 "; expected 0.\n", result64);

  /**
   * Test mixed zero and one inputs.
   */
  v8 = {0,0,1};
  v16 = {0,0,1};
  v32 = {0,0,1};
  v64 = {0,0,1};

  result8 = gcd<uint8_t>(0,1);
  if (result8 != 1) std::printf("Error: gcd<uint8_t>(0,1) returns %u; expected 1.\n", result8);
  result16 = gcd<uint16_t>(0,1);
  if (result16 != 1) std::printf("Error: gcd<uint16_t>(0,1) returns %u; expected 1.\n", result16);
  result32 = gcd<uint32_t>(0u,1u);
  if (result32 != 1u) std::printf("Error: gcd<uint32_t>(0,1) returns %u; expected 1.\n", result32);
  result64 = gcd<uint64_t>(0ull,1ull);
  if (result64 != 1ull) std::printf("Error: gcd<uint64_t>(0,1) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = gcd<uint8_t>({0,0,1});
  if (result8 != 1) std::printf("Error: gcd<uint8_t>({0,0,1}) returns %u; expected 1.\n", result8);
  result16 = gcd<uint16_t>({0,0,1});
  if (result16 != 1) std::printf("Error: gcd<uint16_t>({0,0,1}) returns %u; expected 1.\n", result16);
  result32 = gcd<uint32_t>({0u,0u,1u});
  if (result32 != 1u) std::printf("Error: gcd<uint32_t>({0,0,1}) returns %u; expected 1.\n", result32);
  result64 = gcd<uint64_t>({0ull,0ull,1ull});
  if (result64 != 1ull) std::printf("Error: gcd<uint64_t>({0,0,1}) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 1) std::printf("Error: gcd(v8.cbegin(), v8.cend()) mixed zero-one returns %u; expected 1.\n", result8);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 1) std::printf("Error: gcd(v16.cbegin(), v16.cend()) mixed zero-one returns %u; expected 1.\n", result16);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 1u) std::printf("Error: gcd(v32.cbegin(), v32.cend()) mixed zero-one returns %u; expected 1.\n", result32);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 1ull) std::printf("Error: gcd(v64.cbegin(), v64.cend()) mixed zero-one returns %" PRIu64 "; expected 1.\n", result64);

  result8 = lcm<uint8_t>(0,1);
  if (result8 != 0) std::printf("Error: lcm<uint8_t>(0,1) returns %u; expected 0.\n", result8);
  result16 = lcm<uint16_t>(0,1);
  if (result16 != 0) std::printf("Error: lcm<uint16_t>(0,1) returns %u; expected 0.\n", result16);
  result32 = lcm<uint32_t>(0u,1u);
  if (result32 != 0u) std::printf("Error: lcm<uint32_t>(0,1) returns %u; expected 0.\n", result32);
  result64 = lcm<uint64_t>(0ull,1ull);
  if (result64 != 0ull) std::printf("Error: lcm<uint64_t>(0,1) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = lcm<uint8_t>({0,0,1});
  if (result8 != 0) std::printf("Error: lcm<uint8_t>({0,0,1}) returns %u; expected 0.\n", result8);
  result16 = lcm<uint16_t>({0,0,1});
  if (result16 != 0) std::printf("Error: lcm<uint16_t>({0,0,1}) returns %u; expected 0.\n", result16);
  result32 = lcm<uint32_t>({0u,0u,1u});
  if (result32 != 0u) std::printf("Error: lcm<uint32_t>({0,0,1}) returns %u; expected 0.\n", result32);
  result64 = lcm<uint64_t>({0ull,0ull,1ull});
  if (result64 != 0ull) std::printf("Error: lcm<uint64_t>({0,0,1}) returns %" PRIu64 "; expected 0.\n", result64);

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 0) std::printf("Error: lcm(v8.cbegin(), v8.cend()) mixed zero-one returns %u; expected 0.\n", result8);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 0) std::printf("Error: lcm(v16.cbegin(), v16.cend()) mixed zero-one returns %u; expected 0.\n", result16);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 0u) std::printf("Error: lcm(v32.cbegin(), v32.cend()) mixed zero-one returns %u; expected 0.\n", result32);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 0ull) std::printf("Error: lcm(v64.cbegin(), v64.cend()) mixed zero-one returns %" PRIu64 "; expected 0.\n", result64);

  /**
   * Test mixed all-one inputs.
   */
  v8 = {1,1,1};
  v16 = {1,1,1};
  v32 = {1,1,1};
  v64 = {1,1,1};

  result8 = gcd<uint8_t>(1,1);
  if (result8 != 1) std::printf("Error: gcd<uint8_t>(1,1) returns %u; expected 1.\n", result8);
  result16 = gcd<uint16_t>(1,1);
  if (result16 != 1) std::printf("Error: gcd<uint16_t>(1,1) returns %u; expected 1.\n", result16);
  result32 = gcd<uint32_t>(1u,1u);
  if (result32 != 1u) std::printf("Error: gcd<uint32_t>(1,1) returns %u; expected 1.\n", result32);
  result64 = gcd<uint64_t>(1ull,1ull);
  if (result64 != 1ull) std::printf("Error: gcd<uint64_t>(1,1) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = gcd<uint8_t>({1,1,1});
  if (result8 != 1) std::printf("Error: gcd<uint8_t>({1,1,1}) returns %u; expected 1.\n", result8);
  result16 = gcd<uint16_t>({1,1,1});
  if (result16 != 1) std::printf("Error: gcd<uint16_t>({1,1,1}) returns %u; expected 1.\n", result16);
  result32 = gcd<uint32_t>({1u,1u,1u});
  if (result32 != 1u) std::printf("Error: gcd<uint32_t>({1,1,1}) returns %u; expected 1.\n", result32);
  result64 = gcd<uint64_t>({1ull,1ull,1ull});
  if (result64 != 1ull) std::printf("Error: gcd<uint64_t>({1,1,1}) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 1) std::printf("Error: gcd(v8.cbegin(), v8.cend()) all-one returns %u; expected 1.\n", result8);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 1) std::printf("Error: gcd(v16.cbegin(), v16.cend()) all-one returns %u; expected 1.\n", result16);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 1u) std::printf("Error: gcd(v32.cbegin(), v32.cend()) all-one returns %u; expected 1.\n", result32);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 1ull) std::printf("Error: gcd(v64.cbegin(), v64.cend()) all-one returns %" PRIu64 "; expected 1.\n", result64);

  result8 = lcm<uint8_t>(1,1);
  if (result8 != 1) std::printf("Error: lcm<uint8_t>(1,1) returns %u; expected 1.\n", result8);
  result16 = lcm<uint16_t>(1,1);
  if (result16 != 1) std::printf("Error: lcm<uint16_t>(1,1) returns %u; expected 1.\n", result16);
  result32 = lcm<uint32_t>(1u,1u);
  if (result32 != 1u) std::printf("Error: lcm<uint32_t>(1,1) returns %u; expected 1.\n", result32);
  result64 = lcm<uint64_t>(1ull,1ull);
  if (result64 != 1ull) std::printf("Error: lcm<uint64_t>(1,1) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = lcm<uint8_t>({1,1,1});
  if (result8 != 1) std::printf("Error: lcm<uint8_t>({1,1,1}) returns %u; expected 1.\n", result8);
  result16 = lcm<uint16_t>({1,1,1});
  if (result16 != 1) std::printf("Error: lcm<uint16_t>({1,1,1}) returns %u; expected 1.\n", result16);
  result32 = lcm<uint32_t>({1u,1u,1u});
  if (result32 != 1u) std::printf("Error: lcm<uint32_t>({1,1,1}) returns %u; expected 1.\n", result32);
  result64 = lcm<uint64_t>({1ull,1ull,1ull});
  if (result64 != 1ull) std::printf("Error: lcm<uint64_t>({1,1,1}) returns %" PRIu64 "; expected 1.\n", result64);

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 1) std::printf("Error: lcm(v8.cbegin(), v8.cend()) all-one returns %u; expected 1.\n", result8);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 1) std::printf("Error: lcm(v16.cbegin(), v16.cend()) all-one returns %u; expected 1.\n", result16);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 1u) std::printf("Error: lcm(v32.cbegin(), v32.cend()) all-one returns %u; expected 1.\n", result32);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 1ull) std::printf("Error: lcm(v64.cbegin(), v64.cend()) all-one returns %" PRIu64 "; expected 1.\n", result64);

  /**
   * Test non-zero GCD inputs.
   */
  result8 = gcd<uint8_t>(2*3*5,2*3*5*7);
  if (result8 != 2*3*5) std::printf("Error: gcd<uint8_t>(2*3*5,2*3*5*7) returns %u; expected %u.\n", result8, 2*3*5);
  result8 = gcd<uint8_t>(2*3*5*7,2*3*5);
  if (result8 != 2*3*5) std::printf("Error: gcd<uint8_t>(2*3*5*7,2*3*5) returns %u; expected %u.\n", result8, 2*3*5);
  result16 = gcd<uint16_t>(2*3*5,2*3*5*7);
  if (result16 != 2*3*5) std::printf("Error: gcd<uint16_t>(2*3*5,2*3*5*7) returns %u; expected %u.\n", result16, 2*3*5);
  result16 = gcd<uint16_t>(2*3*5*7,2*3*5);
  if (result16 != 2*3*5) std::printf("Error: gcd<uint16_t>(2*3*5*7,2*3*5) returns %u; expected %u.\n", result16, 2*3*5);
  result32 = gcd<uint32_t>(2*3*5,2*3*5*7);
  if (result32 != 2*3*5) std::printf("Error: gcd<uint32_t>(2*3*5,2*3*5*7) returns %u; expected %u.\n", result32, 2*3*5);
  result32 = gcd<uint32_t>(2*3*5*7,2*3*5);
  if (result32 != 2*3*5) std::printf("Error: gcd<uint32_t>(2*3*5*7,2*3*5) returns %u; expected %u.\n", result32, 2*3*5);
  result64 = gcd<uint64_t>(2*3*5,2*3*5*7);
  if (result64 != 2*3*5) std::printf("Error: gcd<uint64_t>(2*3*5,2*3*5*7) returns %" PRIu64 "; expected %u.\n", result64, 2*3*5);
  result64 = gcd<uint64_t>(2*3*5*7,2*3*5);
  if (result64 != 2*3*5) std::printf("Error: gcd<uint64_t>(2*3*5*7,2*3*5) returns %" PRIu64 "; expected %u.\n", result64, 2*3*5);

  result8 = gcd<uint8_t>({2*3,2*3*5,2*3*5*7});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3,2*3*5,2*3*5*7}) returns %u; expected %u.\n", result8, 2*3);
  result8 = gcd<uint8_t>({2*3*5,2*3,2*3*5*7});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3*5,2*3,2*3*5*7}) returns %u; expected %u.\n", result8, 2*3);
  result8 = gcd<uint8_t>({2*3*5,2*3*5*7,2*3});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3*5,2*3*5*7,2*3}) returns %u; expected %u.\n", result8, 2*3);
  result8 = gcd<uint8_t>({2*3,2*3*5*7,2*3*5});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3,2*3*5*7,2*3*5}) returns %u; expected %u.\n", result8, 2*3);
  result8 = gcd<uint8_t>({2*3*5*7,2*3,2*3*5});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3*5*7,2*3,2*3*5}) returns %u; expected %u.\n", result8, 2*3);
  result8 = gcd<uint8_t>({2*3*5*7,2*3*5,2*3});
  if (result8 != 2*3) std::printf("Error: gcd<uint8_t>({2*3*5*7,2*3*5,2*3}) returns %u; expected %u.\n", result8, 2*3);

  result16 = gcd<uint16_t>({2*3,2*3*5,2*3*5*7});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3,2*3*5,2*3*5*7}) returns %u; expected %u.\n", result16, 2*3);
  result16 = gcd<uint16_t>({2*3*5,2*3,2*3*5*7});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3*5,2*3,2*3*5*7}) returns %u; expected %u.\n", result16, 2*3);
  result16 = gcd<uint16_t>({2*3*5,2*3*5*7,2*3});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3*5,2*3*5*7,2*3}) returns %u; expected %u.\n", result16, 2*3);
  result16 = gcd<uint16_t>({2*3,2*3*5*7,2*3*5});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3,2*3*5*7,2*3*5}) returns %u; expected %u.\n", result16, 2*3);
  result16 = gcd<uint16_t>({2*3*5*7,2*3,2*3*5});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3*5*7,2*3,2*3*5}) returns %u; expected %u.\n", result16, 2*3);
  result16 = gcd<uint16_t>({2*3*5*7,2*3*5,2*3});
  if (result16 != 2*3) std::printf("Error: gcd<uint16_t>({2*3*5*7,2*3*5,2*3}) returns %u; expected %u.\n", result16, 2*3);

  result32 = gcd<uint32_t>({2*3,2*3*5,2*3*5*7});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3,2*3*5,2*3*5*7}) returns %u; expected %u.\n", result32, 2*3);
  result32 = gcd<uint32_t>({2*3*5,2*3,2*3*5*7});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3*5,2*3,2*3*5*7}) returns %u; expected %u.\n", result32, 2*3);
  result32 = gcd<uint32_t>({2*3*5,2*3*5*7,2*3});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3*5,2*3*5*7,2*3}) returns %u; expected %u.\n", result32, 2*3);
  result32 = gcd<uint32_t>({2*3,2*3*5*7,2*3*5});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3,2*3*5*7,2*3*5}) returns %u; expected %u.\n", result32, 2*3);
  result32 = gcd<uint32_t>({2*3*5*7,2*3,2*3*5});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3*5*7,2*3,2*3*5}) returns %u; expected %u.\n", result32, 2*3);
  result32 = gcd<uint32_t>({2*3*5*7,2*3*5,2*3});
  if (result32 != 2*3) std::printf("Error: gcd<uint32_t>({2*3*5*7,2*3*5,2*3}) returns %u; expected %u.\n", result32, 2*3);


  result64 = gcd<uint64_t>({2*3,2*3*5,2*3*5*7});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3,2*3*5,2*3*5*7}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);
  result64 = gcd<uint64_t>({2*3*5,2*3,2*3*5*7});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3*5,2*3,2*3*5*7}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);
  result64 = gcd<uint64_t>({2*3*5,2*3*5*7,2*3});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3*5,2*3*5*7,2*3}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);
  result64 = gcd<uint64_t>({2*3,2*3*5*7,2*3*5});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3,2*3*5*7,2*3*5}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);
  result64 = gcd<uint64_t>({2*3*5*7,2*3,2*3*5});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3*5*7,2*3,2*3*5}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);
  result64 = gcd<uint64_t>({2*3*5*7,2*3*5,2*3});
  if (result64 != 2*3) std::printf("Error: gcd<uint64_t>({2*3*5*7,2*3*5,2*3}) returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3,2*3*5,2*3*5*7};
  v16 = {2*3,2*3*5,2*3*5*7};
  v32 = {2*3,2*3*5,2*3*5*7};
  v64 = {2*3,2*3*5,2*3*5*7};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3,2*3*5,2*3*5*7} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3,2*3*5,2*3*5*7} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3,2*3*5,2*3*5*7} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3,2*3*5,2*3*5*7} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3*5,2*3,2*3*5*7};
  v16 = {2*3*5,2*3,2*3*5*7};
  v32 = {2*3*5,2*3,2*3*5*7};
  v64 = {2*3*5,2*3,2*3*5*7};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3*5,2*3,2*3*5*7} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3*5,2*3,2*3*5*7} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3*5,2*3,2*3*5*7} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3*5,2*3,2*3*5*7} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3*5,2*3*5*7,2*3};
  v16 = {2*3*5,2*3*5*7,2*3};
  v32 = {2*3*5,2*3*5*7,2*3};
  v64 = {2*3*5,2*3*5*7,2*3};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3*5,2*3*5*7,2*3} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3*5,2*3*5*7,2*3} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3*5,2*3*5*7,2*3} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3*5,2*3*5*7,2*3} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3,2*3*5*7,2*3*5};
  v16 = {2*3,2*3*5*7,2*3*5};
  v32 = {2*3,2*3*5*7,2*3*5};
  v64 = {2*3,2*3*5*7,2*3*5};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3,2*3*5*7,2*3*5} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3,2*3*5*7,2*3*5} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3,2*3*5*7,2*3*5} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3,2*3*5*7,2*3*5} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3*5*7,2*3,2*3*5};
  v16 = {2*3*5*7,2*3,2*3*5};
  v32 = {2*3*5*7,2*3,2*3*5};
  v64 = {2*3*5*7,2*3,2*3*5};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3*5*7,2*3,2*3*5} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3*5*7,2*3,2*3*5} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3*5*7,2*3,2*3*5} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3*5*7,2*3,2*3*5} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  v8 = {2*3*5*7,2*3*5,2*3};
  v16 = {2*3*5*7,2*3*5,2*3};
  v32 = {2*3*5*7,2*3*5,2*3};
  v64 = {2*3*5*7,2*3*5,2*3};

  result8 = gcd(v8.cbegin(), v8.cend());
  if (result8 != 2*3) std::printf("Error: gcd(v8.cbegin(), v8.cend()) {2*3*5*7,2*3*5,2*3} returns %u; expected %u.\n", result8, 2*3);
  result16 = gcd(v16.cbegin(), v16.cend());
  if (result16 != 2*3) std::printf("Error: gcd(v16.cbegin(), v16.cend()) {2*3*5*7,2*3*5,2*3} returns %u; expected %u.\n", result16, 2*3);
  result32 = gcd(v32.cbegin(), v32.cend());
  if (result32 != 2*3) std::printf("Error: gcd(v32.cbegin(), v32.cend()) {2*3*5*7,2*3*5,2*3} returns %u; expected %u.\n", result32, 2*3);
  result64 = gcd(v64.cbegin(), v64.cend());
  if (result64 != 2*3) std::printf("Error: gcd(v64.cbegin(), v64.cend()) {2*3*5*7,2*3*5,2*3} returns %" PRIu64 "; expected %u.\n", result64, 2*3);

  /**
   * Test non-zero LCM inputs.
   */
  result8 = lcm<uint8_t>(2*3*3,2*3*5);
  if (result8 != 2*3*3*5) std::printf("Error: lcm<uint8_t>(2*3*3,2*3*5) returns %u; expected %u.\n", result8, 2*3*3*5);
  result8 = lcm<uint8_t>(2*3*5,2*3*3);
  if (result8 != 2*3*3*5) std::printf("Error: lcm<uint8_t>(2*3*5,2*3*3) returns %u; expected %u.\n", result8, 2*3*3*5);
  result16 = lcm<uint16_t>(2*3*3,2*3*5);
  if (result16 != 2*3*3*5) std::printf("Error: lcm<uint16_t>(2*3*3,2*3*5) returns %u; expected %u.\n", result16, 2*3*3*5);
  result16 = lcm<uint16_t>(2*3*5,2*3*3);
  if (result16 != 2*3*3*5) std::printf("Error: lcm<uint16_t>(2*3*5,2*3*3) returns %u; expected %u.\n", result16, 2*3*3*5);
  result32 = lcm<uint32_t>(2*3*3,2*3*5);
  if (result32 != 2*3*3*5) std::printf("Error: lcm<uint32_t>(2*3*3,2*3*5) returns %u; expected %u.\n", result32, 2*3*3*5);
  result32 = lcm<uint32_t>(2*3*5,2*3*3);
  if (result32 != 2*3*3*5) std::printf("Error: lcm<uint32_t>(2*3*5,2*3*3) returns %u; expected %u.\n", result32, 2*3*3*5);
  result64 = lcm<uint64_t>(2*3*3,2*3*5);
  if (result64 != 2*3*3*5) std::printf("Error: lcm<uint64_t>(2*3*3,2*3*5) returns %" PRIu64 "; expected %u.\n", result64, 2*3*3*5);
  result64 = lcm<uint64_t>(2*3*5,2*3*3);
  if (result64 != 2*3*3*5) std::printf("Error: lcm<uint64_t>(2*3*5,2*3*3) returns %" PRIu64 "; expected %u.\n", result64, 2*3*3*5);

  result8 = lcm<uint8_t>({2*2*2,2*2*3,2*2*5});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*2,2*2*3,2*2*5}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result8 = lcm<uint8_t>({2*2*3,2*2*2,2*2*5});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*3,2*2*2,2*2*5}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result8 = lcm<uint8_t>({2*2*3,2*2*5,2*2*2});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*3,2*2*5,2*2*2}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result8 = lcm<uint8_t>({2*2*2,2*2*5,2*2*3});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*2,2*2*5,2*2*3}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result8 = lcm<uint8_t>({2*2*5,2*2*2,2*2*3});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*5,2*2*2,2*2*3}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result8 = lcm<uint8_t>({2*2*5,2*2*3,2*2*2});
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm<uint8_t>({2*2*5,2*2*3,2*2*2}) returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*2,2*2*3,2*2*5});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*2,2*2*3,2*2*5}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*3,2*2*2,2*2*5});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*3,2*2*2,2*2*5}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*3,2*2*5,2*2*2});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*3,2*2*5,2*2*2}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*2,2*2*5,2*2*3});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*2,2*2*5,2*2*3}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*5,2*2*2,2*2*3});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*5,2*2*2,2*2*3}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result16 = lcm<uint16_t>({2*2*5,2*2*3,2*2*2});
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm<uint16_t>({2*2*5,2*2*3,2*2*2}) returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*2,2*2*3,2*2*5});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*2,2*2*3,2*2*5}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*3,2*2*2,2*2*5});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*3,2*2*2,2*2*5}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*3,2*2*5,2*2*2});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*3,2*2*5,2*2*2}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*2,2*2*5,2*2*3});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*2,2*2*5,2*2*3}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*5,2*2*2,2*2*3});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*5,2*2*2,2*2*3}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result32 = lcm<uint32_t>({2*2*5,2*2*3,2*2*2});
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm<uint32_t>({2*2*5,2*2*3,2*2*2}) returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*2,2*2*3,2*2*5});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*2,2*2*3,2*2*5}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*3,2*2*2,2*2*5});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*3,2*2*2,2*2*5}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*3,2*2*5,2*2*2});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*3,2*2*5,2*2*2}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*2,2*2*5,2*2*3});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*2,2*2*5,2*2*3}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*5,2*2*2,2*2*3});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*5,2*2*2,2*2*3}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);
  result64 = lcm<uint64_t>({2*2*5,2*2*3,2*2*2});
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm<uint64_t>({2*2*5,2*2*3,2*2*2}) returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*2,2*2*3,2*2*5};
  v16 = {2*2*2,2*2*3,2*2*5};
  v32 = {2*2*2,2*2*3,2*2*5};
  v64 = {2*2*2,2*2*3,2*2*5};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*2,2*2*3,2*2*5} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*2,2*2*3,2*2*5} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*2,2*2*3,2*2*5} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*2,2*2*3,2*2*5} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*3,2*2*2,2*2*5};
  v16 = {2*2*3,2*2*2,2*2*5};
  v32 = {2*2*3,2*2*2,2*2*5};
  v64 = {2*2*3,2*2*2,2*2*5};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*3,2*2*2,2*2*5} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*3,2*2*2,2*2*5} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*3,2*2*2,2*2*5} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*3,2*2*2,2*2*5} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*3,2*2*5,2*2*2};
  v16 = {2*2*3,2*2*5,2*2*2};
  v32 = {2*2*3,2*2*5,2*2*2};
  v64 = {2*2*3,2*2*5,2*2*2};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*3,2*2*5,2*2*2} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*3,2*2*5,2*2*2} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*3,2*2*5,2*2*2} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*3,2*2*5,2*2*2} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*2,2*2*5,2*2*3};
  v16 = {2*2*2,2*2*5,2*2*3};
  v32 = {2*2*2,2*2*5,2*2*3};
  v64 = {2*2*2,2*2*5,2*2*3};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*2,2*2*5,2*2*3} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*2,2*2*5,2*2*3} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*2,2*2*5,2*2*3} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*2,2*2*5,2*2*3} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*5,2*2*2,2*2*3};
  v16 = {2*2*5,2*2*2,2*2*3};
  v32 = {2*2*5,2*2*2,2*2*3};
  v64 = {2*2*5,2*2*2,2*2*3};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*5,2*2*2,2*2*3} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*5,2*2*2,2*2*3} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*5,2*2*2,2*2*3} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*5,2*2*2,2*2*3} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  v8 = {2*2*5,2*2*3,2*2*2};
  v16 = {2*2*5,2*2*3,2*2*2};
  v32 = {2*2*5,2*2*3,2*2*2};
  v64 = {2*2*5,2*2*3,2*2*2};

  result8 = lcm(v8.cbegin(), v8.cend());
  if (result8 != 2*2*2*3*5) std::printf("Error: lcm(v8.cbegin(), v8.cend()) {2*2*5,2*2*3,2*2*2} returns %u; expected %u.\n", result8, 2*2*2*3*5);
  result16 = lcm(v16.cbegin(), v16.cend());
  if (result16 != 2*2*2*3*5) std::printf("Error: lcm(v16.cbegin(), v16.cend()) {2*2*5,2*2*3,2*2*2} returns %u; expected %u.\n", result16, 2*2*2*3*5);
  result32 = lcm(v32.cbegin(), v32.cend());
  if (result32 != 2*2*2*3*5) std::printf("Error: lcm(v32.cbegin(), v32.cend()) {2*2*5,2*2*3,2*2*2} returns %u; expected %u.\n", result32, 2*2*2*3*5);
  result64 = lcm(v64.cbegin(), v64.cend());
  if (result64 != 2*2*2*3*5) std::printf("Error: lcm(v64.cbegin(), v64.cend()) {2*2*5,2*2*3,2*2*2} returns %" PRIu64 "; expected %u.\n", result64, 2*2*2*3*5);

  /**
   * Checking overflow exceptions in lcm.
   */
  bool caught_overflow = false;
  try {
    result8 = lcm<uint8_t>(17,19);
  } catch (std::overflow_error &e) {
    (void) e;
    caught_overflow = true;
  }
  if (!caught_overflow) std::printf("lcm<uint8_t>(17,19) failed to throw an overflow exception.\n");

  caught_overflow = false;
  try {
    result16 = lcm<uint16_t>(257,263);
  } catch (std::overflow_error &e) {
    (void) e;
    caught_overflow = true;
  }
  if (!caught_overflow) std::printf("lcm<uint16_t>(257,263) failed to throw an overflow exception.\n");

  caught_overflow = false;
  try {
    result32 = lcm<uint32_t>(65537,65539);
  } catch (std::overflow_error &e) {
    (void) e;
    caught_overflow = true;
  }
  if (!caught_overflow) std::printf("lcm<uint32_t>(65537,65539) failed to throw an overflow exception.\n");

  caught_overflow = false;
  try {
    result64 = lcm<uint64_t>(4294967311ull,4294967357ull);
  } catch (std::overflow_error &e) {
    (void) e;
    caught_overflow = true;
  }
  if (!caught_overflow) std::printf("lcm<uint64_t>(4294967311ull,4294967357ull) failed to throw an overflow exception.\n");

  std::printf("If no errors are printed above, all tests passed.\n\n");
  return EXIT_SUCCESS;
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