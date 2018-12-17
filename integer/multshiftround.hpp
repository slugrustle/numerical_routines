/**
 * multshiftround.hpp
 * Specifies the templated function
 *     type multshiftround<typename type, int8_t shift>(const type num, const type mul);
 * which returns the value ROUND((num * mul) / 2^shift) without using the division
 * operator.
 *
 * type may be int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t,
 * uint64_t, or any type equivalent to these.
 *
 * shift may range from 1 to one less than the the word length of type for unsigned
 * types. shift may range from 1 to two less than the word length of type for
 * signed types.
 *
 * The correct operation of multshiftround on negative signed inputs requires
 * the compiler to encode right shifts on signed types as arithmetic right
 * shifts rather than logical right shifts. Verify that your implementation
 * does this.
 *
 * Conceptually, multshiftround allows one to multiply the argument num by a
 * rational number with a base-2 denominator of the form mul / 2^shift. This is a
 * useful operation in fixed point arithmetic.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef MULSHFTRND_HPP_
#define MULSHFTRND_HPP_

#include <cinttypes>

/**
 * The multshiftround primary template is a catch-all for invalid
 * and/or presently unimplemented template arguments.
 */
template <typename type, int8_t shift> type multshiftround(const type num, const type mul) {
  static_assert(false, "type multshiftround<type,shift>(const type num, const type mul) is not defined for the specified type and/or shift value.");
}

/********************************************************************************
 ********                int8_t template specializations                 ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> int8_t multshiftround<int8_t, 1>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if ((prod & 0x81u) == 0x01) return (prod >> 1) + 1;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> int8_t multshiftround<int8_t, 2>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if (((prod & 0x03) >= 0x02) & ((prod & 0x83u) != 0x82u)) return (prod >> 2) + 1;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> int8_t multshiftround<int8_t, 3>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if (((prod & 0x07) >= 0x04) & ((prod & 0x87u) != 0x84u)) return (prod >> 3) + 1;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> int8_t multshiftround<int8_t, 4>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if (((prod & 0x0F) >= 0x08) & ((prod & 0x8Fu) != 0x88u)) return (prod >> 4) + 1;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> int8_t multshiftround<int8_t, 5>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if (((prod & 0x1F) >= 0x10) & ((prod & 0x9Fu) != 0x90u)) return (prod >> 5) + 1;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> int8_t multshiftround<int8_t, 6>(const int8_t num, const int8_t mul) {
  int8_t prod = num * mul;
  if (((prod & 0x3F) >= 0x20) & ((prod & 0xBFu) != 0xA0u)) return (prod >> 6) + 1;
  return prod >> 6;
}

/********************************************************************************
 ********                int16_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> int16_t multshiftround<int16_t, 1>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if ((prod & 0x8001u) == 0x0001) return (prod >> 1) + 1;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> int16_t multshiftround<int16_t, 2>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x0003) >= 0x0002) & ((prod & 0x8003u) != 0x8002u)) return (prod >> 2) + 1;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> int16_t multshiftround<int16_t, 3>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x0007) >= 0x0004) & ((prod & 0x8007u) != 0x8004u)) return (prod >> 3) + 1;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> int16_t multshiftround<int16_t, 4>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x000F) >= 0x0008) & ((prod & 0x800Fu) != 0x8008u)) return (prod >> 4) + 1;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> int16_t multshiftround<int16_t, 5>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x001F) >= 0x0010) & ((prod & 0x801Fu) != 0x8010u)) return (prod >> 5) + 1;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> int16_t multshiftround<int16_t, 6>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x003F) >= 0x0020) & ((prod & 0x803Fu) != 0x8020u)) return (prod >> 6) + 1;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> int16_t multshiftround<int16_t, 7>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x007F) >= 0x0040) & ((prod & 0x807Fu) != 0x8040u)) return (prod >> 7) + 1;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> int16_t multshiftround<int16_t, 8>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x00FF) >= 0x0080) & ((prod & 0x80FFu) != 0x8080u)) return (prod >> 8) + 1;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> int16_t multshiftround<int16_t, 9>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x01FF) >= 0x0100) & ((prod & 0x81FFu) != 0x8100u)) return (prod >> 9) + 1;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> int16_t multshiftround<int16_t, 10>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x03FF) >= 0x0200) & ((prod & 0x83FFu) != 0x8200u)) return (prod >> 10) + 1;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> int16_t multshiftround<int16_t, 11>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x07FF) >= 0x0400) & ((prod & 0x87FFu) != 0x8400u)) return (prod >> 11) + 1;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> int16_t multshiftround<int16_t, 12>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x0FFF) >= 0x0800) & ((prod & 0x8FFFu) != 0x8800u)) return (prod >> 12) + 1;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> int16_t multshiftround<int16_t, 13>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x1FFF) >= 0x1000) & ((prod & 0x9FFFu) != 0x9000u)) return (prod >> 13) + 1;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> int16_t multshiftround<int16_t, 14>(const int16_t num, const int16_t mul) {
  int16_t prod = num * mul;
  if (((prod & 0x3FFF) >= 0x2000) & ((prod & 0xBFFFu) != 0xA000u)) return (prod >> 14) + 1;
  return prod >> 14;
}

/********************************************************************************
 ********                int32_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> int32_t multshiftround<int32_t, 1>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if ((prod & 0x80000001u) == 0x00000001) return (prod >> 1) + 1;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> int32_t multshiftround<int32_t, 2>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00000003) >= 0x00000002) & ((prod & 0x80000003u) != 0x80000002u)) return (prod >> 2) + 1;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> int32_t multshiftround<int32_t, 3>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00000007) >= 0x00000004) & ((prod & 0x80000007u) != 0x80000004u)) return (prod >> 3) + 1;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> int32_t multshiftround<int32_t, 4>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0000000F) >= 0x00000008) & ((prod & 0x8000000Fu) != 0x80000008u)) return (prod >> 4) + 1;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> int32_t multshiftround<int32_t, 5>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0000001F) >= 0x00000010) & ((prod & 0x8000001Fu) != 0x80000010u)) return (prod >> 5) + 1;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> int32_t multshiftround<int32_t, 6>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0000003F) >= 0x00000020) & ((prod & 0x8000003Fu) != 0x80000020u)) return (prod >> 6) + 1;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> int32_t multshiftround<int32_t, 7>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0000007F) >= 0x00000040) & ((prod & 0x8000007Fu) != 0x80000040u)) return (prod >> 7) + 1;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> int32_t multshiftround<int32_t, 8>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x000000FF) >= 0x00000080) & ((prod & 0x800000FFu) != 0x80000080u)) return (prod >> 8) + 1;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> int32_t multshiftround<int32_t, 9>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x000001FF) >= 0x00000100) & ((prod & 0x800001FFu) != 0x80000100u)) return (prod >> 9) + 1;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> int32_t multshiftround<int32_t, 10>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x000003FF) >= 0x00000200) & ((prod & 0x800003FFu) != 0x80000200u)) return (prod >> 10) + 1;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> int32_t multshiftround<int32_t, 11>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x000007FF) >= 0x00000400) & ((prod & 0x800007FFu) != 0x80000400u)) return (prod >> 11) + 1;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> int32_t multshiftround<int32_t, 12>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00000FFF) >= 0x00000800) & ((prod & 0x80000FFFu) != 0x80000800u)) return (prod >> 12) + 1;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> int32_t multshiftround<int32_t, 13>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00001FFF) >= 0x00001000) & ((prod & 0x80001FFFu) != 0x80001000u)) return (prod >> 13) + 1;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> int32_t multshiftround<int32_t, 14>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00003FFF) >= 0x00002000) & ((prod & 0x80003FFFu) != 0x80002000u)) return (prod >> 14) + 1;
  return prod >> 14;
}

/* Returns ROUND((num * mul) / 2^15) */
template <> int32_t multshiftround<int32_t, 15>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00007FFF) >= 0x00004000) & ((prod & 0x80007FFFu) != 0x80004000u)) return (prod >> 15) + 1;
  return prod >> 15;
}

/* Returns ROUND((num * mul) / 2^16) */
template <> int32_t multshiftround<int32_t, 16>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0000FFFF) >= 0x00008000) & ((prod & 0x8000FFFFu) != 0x80008000u)) return (prod >> 16) + 1;
  return prod >> 16;
}

/* Returns ROUND((num * mul) / 2^17) */
template <> int32_t multshiftround<int32_t, 17>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0001FFFF) >= 0x00010000) & ((prod & 0x8001FFFFu) != 0x80010000u)) return (prod >> 17) + 1;
  return prod >> 17;
}

/* Returns ROUND((num * mul) / 2^18) */
template <> int32_t multshiftround<int32_t, 18>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0003FFFF) >= 0x00020000) & ((prod & 0x8003FFFFu) != 0x80020000u)) return (prod >> 18) + 1;
  return prod >> 18;
}

/* Returns ROUND((num * mul) / 2^19) */
template <> int32_t multshiftround<int32_t, 19>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0007FFFF) >= 0x00040000) & ((prod & 0x8007FFFFu) != 0x80040000u)) return (prod >> 19) + 1;
  return prod >> 19;
}

/* Returns ROUND((num * mul) / 2^20) */
template <> int32_t multshiftround<int32_t, 20>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x000FFFFF) >= 0x00080000) & ((prod & 0x800FFFFFu) != 0x80080000u)) return (prod >> 20) + 1;
  return prod >> 20;
}

/* Returns ROUND((num * mul) / 2^21) */
template <> int32_t multshiftround<int32_t, 21>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x001FFFFF) >= 0x00100000) & ((prod & 0x801FFFFFu) != 0x80100000u)) return (prod >> 21) + 1;
  return prod >> 21;
}

/* Returns ROUND((num * mul) / 2^22) */
template <> int32_t multshiftround<int32_t, 22>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x003FFFFF) >= 0x00200000) & ((prod & 0x803FFFFFu) != 0x80200000u)) return (prod >> 22) + 1;
  return prod >> 22;
}

/* Returns ROUND((num * mul) / 2^23) */
template <> int32_t multshiftround<int32_t, 23>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x007FFFFF) >= 0x00400000) & ((prod & 0x807FFFFFu) != 0x80400000u)) return (prod >> 23) + 1;
  return prod >> 23;
}

/* Returns ROUND((num * mul) / 2^24) */
template <> int32_t multshiftround<int32_t, 24>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x00FFFFFF) >= 0x00800000) & ((prod & 0x80FFFFFFu) != 0x80800000u)) return (prod >> 24) + 1;
  return prod >> 24;
}

/* Returns ROUND((num * mul) / 2^25) */
template <> int32_t multshiftround<int32_t, 25>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x01FFFFFF) >= 0x01000000) & ((prod & 0x81FFFFFFu) != 0x81000000u)) return (prod >> 25) + 1;
  return prod >> 25;
}

/* Returns ROUND((num * mul) / 2^26) */
template <> int32_t multshiftround<int32_t, 26>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x03FFFFFF) >= 0x02000000) & ((prod & 0x83FFFFFFu) != 0x82000000u)) return (prod >> 26) + 1;
  return prod >> 26;
}

/* Returns ROUND((num * mul) / 2^27) */
template <> int32_t multshiftround<int32_t, 27>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x07FFFFFF) >= 0x04000000) & ((prod & 0x87FFFFFFu) != 0x84000000u)) return (prod >> 27) + 1;
  return prod >> 27;
}

/* Returns ROUND((num * mul) / 2^28) */
template <> int32_t multshiftround<int32_t, 28>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x0FFFFFFF) >= 0x08000000) & ((prod & 0x8FFFFFFFu) != 0x88000000u)) return (prod >> 28) + 1;
  return prod >> 28;
}

/* Returns ROUND((num * mul) / 2^29) */
template <> int32_t multshiftround<int32_t, 29>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x1FFFFFFF) >= 0x10000000) & ((prod & 0x9FFFFFFFu) != 0x90000000u)) return (prod >> 29) + 1;
  return prod >> 29;
}

/* Returns ROUND((num * mul) / 2^30) */
template <> int32_t multshiftround<int32_t, 30>(const int32_t num, const int32_t mul) {
  int32_t prod = num * mul;
  if (((prod & 0x3FFFFFFF) >= 0x20000000) & ((prod & 0xBFFFFFFFu) != 0xA0000000u)) return (prod >> 30) + 1;
  return prod >> 30;
}

/********************************************************************************
 ********                int64_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> int64_t multshiftround<int64_t, 1>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if ((prod & 0x8000000000000001ull) == 0x0000000000000001ull) return (prod >> 1) + 1ll;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> int64_t multshiftround<int64_t, 2>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000000003ll) >= 0x0000000000000002ll) & ((prod & 0x8000000000000003ull) != 0x8000000000000002ull)) return (prod >> 2) + 1ll;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> int64_t multshiftround<int64_t, 3>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000000007ll) >= 0x0000000000000004ll) & ((prod & 0x8000000000000007ull) != 0x8000000000000004ull)) return (prod >> 3) + 1ll;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> int64_t multshiftround<int64_t, 4>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000000000Fll) >= 0x0000000000000008ll) & ((prod & 0x800000000000000Full) != 0x8000000000000008ull)) return (prod >> 4) + 1ll;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> int64_t multshiftround<int64_t, 5>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000000001Fll) >= 0x0000000000000010ll) & ((prod & 0x800000000000001Full) != 0x8000000000000010ull)) return (prod >> 5) + 1ll;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> int64_t multshiftround<int64_t, 6>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000000003Fll) >= 0x0000000000000020ll) & ((prod & 0x800000000000003Full) != 0x8000000000000020ull)) return (prod >> 6) + 1ll;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> int64_t multshiftround<int64_t, 7>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000000007Fll) >= 0x0000000000000040ll) & ((prod & 0x800000000000007Full) != 0x8000000000000040ull)) return (prod >> 7) + 1ll;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> int64_t multshiftround<int64_t, 8>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000000000FFll) >= 0x0000000000000080ll) & ((prod & 0x80000000000000FFull) != 0x8000000000000080ull)) return (prod >> 8) + 1ll;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> int64_t multshiftround<int64_t, 9>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000000001FFll) >= 0x0000000000000100ll) & ((prod & 0x80000000000001FFull) != 0x8000000000000100ull)) return (prod >> 9) + 1ll;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> int64_t multshiftround<int64_t, 10>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000000003FFll) >= 0x0000000000000200ll) & ((prod & 0x80000000000003FFull) != 0x8000000000000200ull)) return (prod >> 10) + 1ll;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> int64_t multshiftround<int64_t, 11>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000000007FFll) >= 0x0000000000000400ll) & ((prod & 0x80000000000007FFull) != 0x8000000000000400ull)) return (prod >> 11) + 1ll;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> int64_t multshiftround<int64_t, 12>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000000FFFll) >= 0x0000000000000800ll) & ((prod & 0x8000000000000FFFull) != 0x8000000000000800ull)) return (prod >> 12) + 1ll;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> int64_t multshiftround<int64_t, 13>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000001FFFll) >= 0x0000000000001000ll) & ((prod & 0x8000000000001FFFull) != 0x8000000000001000ull)) return (prod >> 13) + 1ll;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> int64_t multshiftround<int64_t, 14>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000003FFFll) >= 0x0000000000002000ll) & ((prod & 0x8000000000003FFFull) != 0x8000000000002000ull)) return (prod >> 14) + 1ll;
  return prod >> 14;
}

/* Returns ROUND((num * mul) / 2^15) */
template <> int64_t multshiftround<int64_t, 15>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000007FFFll) >= 0x0000000000004000ll) & ((prod & 0x8000000000007FFFull) != 0x8000000000004000ull)) return (prod >> 15) + 1ll;
  return prod >> 15;
}

/* Returns ROUND((num * mul) / 2^16) */
template <> int64_t multshiftround<int64_t, 16>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000000FFFFll) >= 0x0000000000008000ll) & ((prod & 0x800000000000FFFFull) != 0x8000000000008000ull)) return (prod >> 16) + 1ll;
  return prod >> 16;
}

/* Returns ROUND((num * mul) / 2^17) */
template <> int64_t multshiftround<int64_t, 17>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000001FFFFll) >= 0x0000000000010000ll) & ((prod & 0x800000000001FFFFull) != 0x8000000000010000ull)) return (prod >> 17) + 1ll;
  return prod >> 17;
}

/* Returns ROUND((num * mul) / 2^18) */
template <> int64_t multshiftround<int64_t, 18>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000003FFFFll) >= 0x0000000000020000ll) & ((prod & 0x800000000003FFFFull) != 0x8000000000020000ull)) return (prod >> 18) + 1ll;
  return prod >> 18;
}

/* Returns ROUND((num * mul) / 2^19) */
template <> int64_t multshiftround<int64_t, 19>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000007FFFFll) >= 0x0000000000040000ll) & ((prod & 0x800000000007FFFFull) != 0x8000000000040000ull)) return (prod >> 19) + 1ll;
  return prod >> 19;
}

/* Returns ROUND((num * mul) / 2^20) */
template <> int64_t multshiftround<int64_t, 20>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000000FFFFFll) >= 0x0000000000080000ll) & ((prod & 0x80000000000FFFFFull) != 0x8000000000080000ull)) return (prod >> 20) + 1ll;
  return prod >> 20;
}

/* Returns ROUND((num * mul) / 2^21) */
template <> int64_t multshiftround<int64_t, 21>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000001FFFFFll) >= 0x0000000000100000ll) & ((prod & 0x80000000001FFFFFull) != 0x8000000000100000ull)) return (prod >> 21) + 1ll;
  return prod >> 21;
}

/* Returns ROUND((num * mul) / 2^22) */
template <> int64_t multshiftround<int64_t, 22>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000003FFFFFll) >= 0x0000000000200000ll) & ((prod & 0x80000000003FFFFFull) != 0x8000000000200000ull)) return (prod >> 22) + 1ll;
  return prod >> 22;
}

/* Returns ROUND((num * mul) / 2^23) */
template <> int64_t multshiftround<int64_t, 23>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000007FFFFFll) >= 0x0000000000400000ll) & ((prod & 0x80000000007FFFFFull) != 0x8000000000400000ull)) return (prod >> 23) + 1ll;
  return prod >> 23;
}

/* Returns ROUND((num * mul) / 2^24) */
template <> int64_t multshiftround<int64_t, 24>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000000FFFFFFll) >= 0x0000000000800000ll) & ((prod & 0x8000000000FFFFFFull) != 0x8000000000800000ull)) return (prod >> 24) + 1ll;
  return prod >> 24;
}

/* Returns ROUND((num * mul) / 2^25) */
template <> int64_t multshiftround<int64_t, 25>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000001FFFFFFll) >= 0x0000000001000000ll) & ((prod & 0x8000000001FFFFFFull) != 0x8000000001000000ull)) return (prod >> 25) + 1ll;
  return prod >> 25;
}

/* Returns ROUND((num * mul) / 2^26) */
template <> int64_t multshiftround<int64_t, 26>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000003FFFFFFll) >= 0x0000000002000000ll) & ((prod & 0x8000000003FFFFFFull) != 0x8000000002000000ull)) return (prod >> 26) + 1ll;
  return prod >> 26;
}

/* Returns ROUND((num * mul) / 2^27) */
template <> int64_t multshiftround<int64_t, 27>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000007FFFFFFll) >= 0x0000000004000000ll) & ((prod & 0x8000000007FFFFFFull) != 0x8000000004000000ull)) return (prod >> 27) + 1ll;
  return prod >> 27;
}

/* Returns ROUND((num * mul) / 2^28) */
template <> int64_t multshiftround<int64_t, 28>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000000FFFFFFFll) >= 0x0000000008000000ll) & ((prod & 0x800000000FFFFFFFull) != 0x8000000008000000ull)) return (prod >> 28) + 1ll;
  return prod >> 28;
}

/* Returns ROUND((num * mul) / 2^29) */
template <> int64_t multshiftround<int64_t, 29>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000001FFFFFFFll) >= 0x0000000010000000ll) & ((prod & 0x800000001FFFFFFFull) != 0x8000000010000000ull)) return (prod >> 29) + 1ll;
  return prod >> 29;
}

/* Returns ROUND((num * mul) / 2^30) */
template <> int64_t multshiftround<int64_t, 30>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000003FFFFFFFll) >= 0x0000000020000000ll) & ((prod & 0x800000003FFFFFFFull) != 0x8000000020000000ull)) return (prod >> 30) + 1ll;
  return prod >> 30;
}

/* Returns ROUND((num * mul) / 2^31) */
template <> int64_t multshiftround<int64_t, 31>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000007FFFFFFFll) >= 0x0000000040000000ll) & ((prod & 0x800000007FFFFFFFull) != 0x8000000040000000ull)) return (prod >> 31) + 1ll;
  return prod >> 31;
}

/* Returns ROUND((num * mul) / 2^32) */
template <> int64_t multshiftround<int64_t, 32>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000000FFFFFFFFll) >= 0x0000000080000000ll) & ((prod & 0x80000000FFFFFFFFull) != 0x8000000080000000ull)) return (prod >> 32) + 1ll;
  return prod >> 32;
}

/* Returns ROUND((num * mul) / 2^33) */
template <> int64_t multshiftround<int64_t, 33>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000001FFFFFFFFll) >= 0x0000000100000000ll) & ((prod & 0x80000001FFFFFFFFull) != 0x8000000100000000ull)) return (prod >> 33) + 1ll;
  return prod >> 33;
}

/* Returns ROUND((num * mul) / 2^34) */
template <> int64_t multshiftround<int64_t, 34>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000003FFFFFFFFll) >= 0x0000000200000000ll) & ((prod & 0x80000003FFFFFFFFull) != 0x8000000200000000ull)) return (prod >> 34) + 1ll;
  return prod >> 34;
}

/* Returns ROUND((num * mul) / 2^35) */
template <> int64_t multshiftround<int64_t, 35>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000007FFFFFFFFll) >= 0x0000000400000000ll) & ((prod & 0x80000007FFFFFFFFull) != 0x8000000400000000ull)) return (prod >> 35) + 1ll;
  return prod >> 35;
}

/* Returns ROUND((num * mul) / 2^36) */
template <> int64_t multshiftround<int64_t, 36>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000000FFFFFFFFFll) >= 0x0000000800000000ll) & ((prod & 0x8000000FFFFFFFFFull) != 0x8000000800000000ull)) return (prod >> 36) + 1ll;
  return prod >> 36;
}

/* Returns ROUND((num * mul) / 2^37) */
template <> int64_t multshiftround<int64_t, 37>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000001FFFFFFFFFll) >= 0x0000001000000000ll) & ((prod & 0x8000001FFFFFFFFFull) != 0x8000001000000000ull)) return (prod >> 37) + 1ll;
  return prod >> 37;
}

/* Returns ROUND((num * mul) / 2^38) */
template <> int64_t multshiftround<int64_t, 38>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000003FFFFFFFFFll) >= 0x0000002000000000ll) & ((prod & 0x8000003FFFFFFFFFull) != 0x8000002000000000ull)) return (prod >> 38) + 1ll;
  return prod >> 38;
}

/* Returns ROUND((num * mul) / 2^39) */
template <> int64_t multshiftround<int64_t, 39>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000007FFFFFFFFFll) >= 0x0000004000000000ll) & ((prod & 0x8000007FFFFFFFFFull) != 0x8000004000000000ull)) return (prod >> 39) + 1ll;
  return prod >> 39;
}

/* Returns ROUND((num * mul) / 2^40) */
template <> int64_t multshiftround<int64_t, 40>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000000FFFFFFFFFFll) >= 0x0000008000000000ll) & ((prod & 0x800000FFFFFFFFFFull) != 0x8000008000000000ull)) return (prod >> 40) + 1ll;
  return prod >> 40;
}

/* Returns ROUND((num * mul) / 2^41) */
template <> int64_t multshiftround<int64_t, 41>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000001FFFFFFFFFFll) >= 0x0000010000000000ll) & ((prod & 0x800001FFFFFFFFFFull) != 0x8000010000000000ull)) return (prod >> 41) + 1ll;
  return prod >> 41;
}

/* Returns ROUND((num * mul) / 2^42) */
template <> int64_t multshiftround<int64_t, 42>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000003FFFFFFFFFFll) >= 0x0000020000000000ll) & ((prod & 0x800003FFFFFFFFFFull) != 0x8000020000000000ull)) return (prod >> 42) + 1ll;
  return prod >> 42;
}

/* Returns ROUND((num * mul) / 2^43) */
template <> int64_t multshiftround<int64_t, 43>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000007FFFFFFFFFFll) >= 0x0000040000000000ll) & ((prod & 0x800007FFFFFFFFFFull) != 0x8000040000000000ull)) return (prod >> 43) + 1ll;
  return prod >> 43;
}

/* Returns ROUND((num * mul) / 2^44) */
template <> int64_t multshiftround<int64_t, 44>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00000FFFFFFFFFFFll) >= 0x0000080000000000ll) & ((prod & 0x80000FFFFFFFFFFFull) != 0x8000080000000000ull)) return (prod >> 44) + 1ll;
  return prod >> 44;
}

/* Returns ROUND((num * mul) / 2^45) */
template <> int64_t multshiftround<int64_t, 45>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00001FFFFFFFFFFFll) >= 0x0000100000000000ll) & ((prod & 0x80001FFFFFFFFFFFull) != 0x8000100000000000ull)) return (prod >> 45) + 1ll;
  return prod >> 45;
}

/* Returns ROUND((num * mul) / 2^46) */
template <> int64_t multshiftround<int64_t, 46>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00003FFFFFFFFFFFll) >= 0x0000200000000000ll) & ((prod & 0x80003FFFFFFFFFFFull) != 0x8000200000000000ull)) return (prod >> 46) + 1ll;
  return prod >> 46;
}

/* Returns ROUND((num * mul) / 2^47) */
template <> int64_t multshiftround<int64_t, 47>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00007FFFFFFFFFFFll) >= 0x0000400000000000ll) & ((prod & 0x80007FFFFFFFFFFFull) != 0x8000400000000000ull)) return (prod >> 47) + 1ll;
  return prod >> 47;
}

/* Returns ROUND((num * mul) / 2^48) */
template <> int64_t multshiftround<int64_t, 48>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0000FFFFFFFFFFFFll) >= 0x0000800000000000ll) & ((prod & 0x8000FFFFFFFFFFFFull) != 0x8000800000000000ull)) return (prod >> 48) + 1ll;
  return prod >> 48;
}

/* Returns ROUND((num * mul) / 2^49) */
template <> int64_t multshiftround<int64_t, 49>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0001FFFFFFFFFFFFll) >= 0x0001000000000000ll) & ((prod & 0x8001FFFFFFFFFFFFull) != 0x8001000000000000ull)) return (prod >> 49) + 1ll;
  return prod >> 49;
}

/* Returns ROUND((num * mul) / 2^50) */
template <> int64_t multshiftround<int64_t, 50>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0003FFFFFFFFFFFFll) >= 0x0002000000000000ll) & ((prod & 0x8003FFFFFFFFFFFFull) != 0x8002000000000000ull)) return (prod >> 50) + 1ll;
  return prod >> 50;
}

/* Returns ROUND((num * mul) / 2^51) */
template <> int64_t multshiftround<int64_t, 51>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0007FFFFFFFFFFFFll) >= 0x0004000000000000ll) & ((prod & 0x8007FFFFFFFFFFFFull) != 0x8004000000000000ull)) return (prod >> 51) + 1ll;
  return prod >> 51;
}

/* Returns ROUND((num * mul) / 2^52) */
template <> int64_t multshiftround<int64_t, 52>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x000FFFFFFFFFFFFFll) >= 0x0008000000000000ll) & ((prod & 0x800FFFFFFFFFFFFFull) != 0x8008000000000000ull)) return (prod >> 52) + 1ll;
  return prod >> 52;
}

/* Returns ROUND((num * mul) / 2^53) */
template <> int64_t multshiftround<int64_t, 53>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x001FFFFFFFFFFFFFll) >= 0x0010000000000000ll) & ((prod & 0x801FFFFFFFFFFFFFull) != 0x8010000000000000ull)) return (prod >> 53) + 1ll;
  return prod >> 53;
}

/* Returns ROUND((num * mul) / 2^54) */
template <> int64_t multshiftround<int64_t, 54>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x003FFFFFFFFFFFFFll) >= 0x0020000000000000ll) & ((prod & 0x803FFFFFFFFFFFFFull) != 0x8020000000000000ull)) return (prod >> 54) + 1ll;
  return prod >> 54;
}

/* Returns ROUND((num * mul) / 2^55) */
template <> int64_t multshiftround<int64_t, 55>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x007FFFFFFFFFFFFFll) >= 0x0040000000000000ll) & ((prod & 0x807FFFFFFFFFFFFFull) != 0x8040000000000000ull)) return (prod >> 55) + 1ll;
  return prod >> 55;
}

/* Returns ROUND((num * mul) / 2^56) */
template <> int64_t multshiftround<int64_t, 56>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x00FFFFFFFFFFFFFFll) >= 0x0080000000000000ll) & ((prod & 0x80FFFFFFFFFFFFFFull) != 0x8080000000000000ull)) return (prod >> 56) + 1ll;
  return prod >> 56;
}

/* Returns ROUND((num * mul) / 2^57) */
template <> int64_t multshiftround<int64_t, 57>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x01FFFFFFFFFFFFFFll) >= 0x0100000000000000ll) & ((prod & 0x81FFFFFFFFFFFFFFull) != 0x8100000000000000ull)) return (prod >> 57) + 1ll;
  return prod >> 57;
}

/* Returns ROUND((num * mul) / 2^58) */
template <> int64_t multshiftround<int64_t, 58>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x03FFFFFFFFFFFFFFll) >= 0x0200000000000000ll) & ((prod & 0x83FFFFFFFFFFFFFFull) != 0x8200000000000000ull)) return (prod >> 58) + 1ll;
  return prod >> 58;
}

/* Returns ROUND((num * mul) / 2^59) */
template <> int64_t multshiftround<int64_t, 59>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x07FFFFFFFFFFFFFFll) >= 0x0400000000000000ll) & ((prod & 0x87FFFFFFFFFFFFFFull) != 0x8400000000000000ull)) return (prod >> 59) + 1ll;
  return prod >> 59;
}

/* Returns ROUND((num * mul) / 2^60) */
template <> int64_t multshiftround<int64_t, 60>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x0FFFFFFFFFFFFFFFll) >= 0x0800000000000000ll) & ((prod & 0x8FFFFFFFFFFFFFFFull) != 0x8800000000000000ull)) return (prod >> 60) + 1ll;
  return prod >> 60;
}

/* Returns ROUND((num * mul) / 2^61) */
template <> int64_t multshiftround<int64_t, 61>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x1FFFFFFFFFFFFFFFll) >= 0x1000000000000000ll) & ((prod & 0x9FFFFFFFFFFFFFFFull) != 0x9000000000000000ull)) return (prod >> 61) + 1ll;
  return prod >> 61;
}

/* Returns ROUND((num * mul) / 2^62) */
template <> int64_t multshiftround<int64_t, 62>(const int64_t num, const int64_t mul) {
  int64_t prod = num * mul;
  if (((prod & 0x3FFFFFFFFFFFFFFFll) >= 0x2000000000000000ll) & ((prod & 0xBFFFFFFFFFFFFFFFull) != 0xA000000000000000ull)) return (prod >> 62) + 1ll;
  return prod >> 62;
}

/********************************************************************************
 ********                uint8_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> uint8_t multshiftround<uint8_t, 1>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if (prod & 0x01u) return (prod >> 1) + 1u;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> uint8_t multshiftround<uint8_t, 2>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x03u) >= 0x02u) return (prod >> 2) + 1u;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> uint8_t multshiftround<uint8_t, 3>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x07u) >= 0x04u) return (prod >> 3) + 1u;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> uint8_t multshiftround<uint8_t, 4>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x0Fu) >= 0x08u) return (prod >> 4) + 1u;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> uint8_t multshiftround<uint8_t, 5>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x1Fu) >= 0x10u) return (prod >> 5) + 1u;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> uint8_t multshiftround<uint8_t, 6>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x3Fu) >= 0x20u) return (prod >> 6) + 1u;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> uint8_t multshiftround<uint8_t, 7>(const uint8_t num, const uint8_t mul) {
  uint8_t prod = num * mul;
  if ((prod & 0x7Fu) >= 0x40u) return (prod >> 7) + 1u;
  return prod >> 7;
}

/********************************************************************************
 ********               uint16_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> uint16_t multshiftround<uint16_t, 1>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if (prod & 0x0001u) return (prod >> 1) + 1u;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> uint16_t multshiftround<uint16_t, 2>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x0003u) >= 0x0002u) return (prod >> 2) + 1u;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> uint16_t multshiftround<uint16_t, 3>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x0007u) >= 0x0004u) return (prod >> 3) + 1u;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> uint16_t multshiftround<uint16_t, 4>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x000Fu) >= 0x0008u) return (prod >> 4) + 1u;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> uint16_t multshiftround<uint16_t, 5>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x001Fu) >= 0x0010u) return (prod >> 5) + 1u;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> uint16_t multshiftround<uint16_t, 6>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x003Fu) >= 0x0020u) return (prod >> 6) + 1u;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> uint16_t multshiftround<uint16_t, 7>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x007Fu) >= 0x0040u) return (prod >> 7) + 1u;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> uint16_t multshiftround<uint16_t, 8>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x00FFu) >= 0x0080u) return (prod >> 8) + 1u;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> uint16_t multshiftround<uint16_t, 9>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x01FFu) >= 0x0100u) return (prod >> 9) + 1u;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> uint16_t multshiftround<uint16_t, 10>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x03FFu) >= 0x0200u) return (prod >> 10) + 1u;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> uint16_t multshiftround<uint16_t, 11>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x07FFu) >= 0x0400u) return (prod >> 11) + 1u;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> uint16_t multshiftround<uint16_t, 12>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x0FFFu) >= 0x0800u) return (prod >> 12) + 1u;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> uint16_t multshiftround<uint16_t, 13>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x1FFFu) >= 0x1000u) return (prod >> 13) + 1u;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> uint16_t multshiftround<uint16_t, 14>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x3FFFu) >= 0x2000u) return (prod >> 14) + 1u;
  return prod >> 14;
}

/* Returns ROUND((num * mul) / 2^15) */
template <> uint16_t multshiftround<uint16_t, 15>(const uint16_t num, const uint16_t mul) {
  uint16_t prod = num * mul;
  if ((prod & 0x7FFFu) >= 0x4000u) return (prod >> 15) + 1u;
  return prod >> 15;
}

/********************************************************************************
 ********               uint32_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> uint32_t multshiftround<uint32_t, 1>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if (prod & 0x00000001u) return (prod >> 1) + 1u;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> uint32_t multshiftround<uint32_t, 2>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00000003u) >= 0x00000002u) return (prod >> 2) + 1u;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> uint32_t multshiftround<uint32_t, 3>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00000007u) >= 0x00000004u) return (prod >> 3) + 1u;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> uint32_t multshiftround<uint32_t, 4>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0000000Fu) >= 0x00000008u) return (prod >> 4) + 1u;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> uint32_t multshiftround<uint32_t, 5>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0000001Fu) >= 0x00000010u) return (prod >> 5) + 1u;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> uint32_t multshiftround<uint32_t, 6>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0000003Fu) >= 0x00000020u) return (prod >> 6) + 1u;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> uint32_t multshiftround<uint32_t, 7>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0000007Fu) >= 0x00000040u) return (prod >> 7) + 1u;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> uint32_t multshiftround<uint32_t, 8>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x000000FFu) >= 0x00000080u) return (prod >> 8) + 1u;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> uint32_t multshiftround<uint32_t, 9>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x000001FFu) >= 0x00000100u) return (prod >> 9) + 1u;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> uint32_t multshiftround<uint32_t, 10>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x000003FFu) >= 0x00000200u) return (prod >> 10) + 1u;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> uint32_t multshiftround<uint32_t, 11>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x000007FFu) >= 0x00000400u) return (prod >> 11) + 1u;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> uint32_t multshiftround<uint32_t, 12>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00000FFFu) >= 0x00000800u) return (prod >> 12) + 1u;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> uint32_t multshiftround<uint32_t, 13>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00001FFFu) >= 0x00001000u) return (prod >> 13) + 1u;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> uint32_t multshiftround<uint32_t, 14>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00003FFFu) >= 0x00002000u) return (prod >> 14) + 1u;
  return prod >> 14;
}

/* Returns ROUND((num * mul) / 2^15) */
template <> uint32_t multshiftround<uint32_t, 15>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00007FFFu) >= 0x00004000u) return (prod >> 15) + 1u;
  return prod >> 15;
}

/* Returns ROUND((num * mul) / 2^16) */
template <> uint32_t multshiftround<uint32_t, 16>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0000FFFFu) >= 0x00008000u) return (prod >> 16) + 1u;
  return prod >> 16;
}

/* Returns ROUND((num * mul) / 2^17) */
template <> uint32_t multshiftround<uint32_t, 17>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0001FFFFu) >= 0x00010000u) return (prod >> 17) + 1u;
  return prod >> 17;
}

/* Returns ROUND((num * mul) / 2^18) */
template <> uint32_t multshiftround<uint32_t, 18>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0003FFFFu) >= 0x00020000u) return (prod >> 18) + 1u;
  return prod >> 18;
}

/* Returns ROUND((num * mul) / 2^19) */
template <> uint32_t multshiftround<uint32_t, 19>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0007FFFFu) >= 0x00040000u) return (prod >> 19) + 1u;
  return prod >> 19;
}

/* Returns ROUND((num * mul) / 2^20) */
template <> uint32_t multshiftround<uint32_t, 20>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x000FFFFFu) >= 0x00080000u) return (prod >> 20) + 1u;
  return prod >> 20;
}

/* Returns ROUND((num * mul) / 2^21) */
template <> uint32_t multshiftround<uint32_t, 21>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x001FFFFFu) >= 0x00100000u) return (prod >> 21) + 1u;
  return prod >> 21;
}

/* Returns ROUND((num * mul) / 2^22) */
template <> uint32_t multshiftround<uint32_t, 22>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x003FFFFFu) >= 0x00200000u) return (prod >> 22) + 1u;
  return prod >> 22;
}

/* Returns ROUND((num * mul) / 2^23) */
template <> uint32_t multshiftround<uint32_t, 23>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x007FFFFFu) >= 0x00400000u) return (prod >> 23) + 1u;
  return prod >> 23;
}

/* Returns ROUND((num * mul) / 2^24) */
template <> uint32_t multshiftround<uint32_t, 24>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x00FFFFFFu) >= 0x00800000u) return (prod >> 24) + 1u;
  return prod >> 24;
}

/* Returns ROUND((num * mul) / 2^25) */
template <> uint32_t multshiftround<uint32_t, 25>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x01FFFFFFu) >= 0x01000000u) return (prod >> 25) + 1u;
  return prod >> 25;
}

/* Returns ROUND((num * mul) / 2^26) */
template <> uint32_t multshiftround<uint32_t, 26>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x03FFFFFFu) >= 0x02000000u) return (prod >> 26) + 1u;
  return prod >> 26;
}

/* Returns ROUND((num * mul) / 2^27) */
template <> uint32_t multshiftround<uint32_t, 27>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x07FFFFFFu) >= 0x04000000u) return (prod >> 27) + 1u;
  return prod >> 27;
}

/* Returns ROUND((num * mul) / 2^28) */
template <> uint32_t multshiftround<uint32_t, 28>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x0FFFFFFFu) >= 0x08000000u) return (prod >> 28) + 1u;
  return prod >> 28;
}

/* Returns ROUND((num * mul) / 2^29) */
template <> uint32_t multshiftround<uint32_t, 29>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x1FFFFFFFu) >= 0x10000000u) return (prod >> 29) + 1u;
  return prod >> 29;
}

/* Returns ROUND((num * mul) / 2^30) */
template <> uint32_t multshiftround<uint32_t, 30>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x3FFFFFFFu) >= 0x20000000u) return (prod >> 30) + 1u;
  return prod >> 30;
}

/* Returns ROUND((num * mul) / 2^31) */
template <> uint32_t multshiftround<uint32_t, 31>(const uint32_t num, const uint32_t mul) {
  uint32_t prod = num * mul;
  if ((prod & 0x7FFFFFFFu) >= 0x40000000u) return (prod >> 31) + 1u;
  return prod >> 31;
}

/********************************************************************************
 ********                uint64_t template specializations                ********
 ********************************************************************************/

/* Returns ROUND((num * mul) / 2^1) */
template <> uint64_t multshiftround<uint64_t, 1>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if (prod & 0x0000000000000001ull) return (prod >> 1) + 1ull;
  return prod >> 1;
}

/* Returns ROUND((num * mul) / 2^2) */
template <> uint64_t multshiftround<uint64_t, 2>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000000003ull) >= 0x0000000000000002ull) return (prod >> 2) + 1ull;
  return prod >> 2;
}

/* Returns ROUND((num * mul) / 2^3) */
template <> uint64_t multshiftround<uint64_t, 3>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000000007ull) >= 0x0000000000000004ull) return (prod >> 3) + 1ull;
  return prod >> 3;
}

/* Returns ROUND((num * mul) / 2^4) */
template <> uint64_t multshiftround<uint64_t, 4>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000000000Full) >= 0x0000000000000008ull) return (prod >> 4) + 1ull;
  return prod >> 4;
}

/* Returns ROUND((num * mul) / 2^5) */
template <> uint64_t multshiftround<uint64_t, 5>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000000001Full) >= 0x0000000000000010ull) return (prod >> 5) + 1ull;
  return prod >> 5;
}

/* Returns ROUND((num * mul) / 2^6) */
template <> uint64_t multshiftround<uint64_t, 6>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000000003Full) >= 0x0000000000000020ull) return (prod >> 6) + 1ull;
  return prod >> 6;
}

/* Returns ROUND((num * mul) / 2^7) */
template <> uint64_t multshiftround<uint64_t, 7>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000000007Full) >= 0x0000000000000040ull) return (prod >> 7) + 1ull;
  return prod >> 7;
}

/* Returns ROUND((num * mul) / 2^8) */
template <> uint64_t multshiftround<uint64_t, 8>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000000000FFull) >= 0x0000000000000080ull) return (prod >> 8) + 1ull;
  return prod >> 8;
}

/* Returns ROUND((num * mul) / 2^9) */
template <> uint64_t multshiftround<uint64_t, 9>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000000001FFull) >= 0x0000000000000100ull) return (prod >> 9) + 1ull;
  return prod >> 9;
}

/* Returns ROUND((num * mul) / 2^10) */
template <> uint64_t multshiftround<uint64_t, 10>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000000003FFull) >= 0x0000000000000200ull) return (prod >> 10) + 1ull;
  return prod >> 10;
}

/* Returns ROUND((num * mul) / 2^11) */
template <> uint64_t multshiftround<uint64_t, 11>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000000007FFull) >= 0x0000000000000400ull) return (prod >> 11) + 1ull;
  return prod >> 11;
}

/* Returns ROUND((num * mul) / 2^12) */
template <> uint64_t multshiftround<uint64_t, 12>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000000FFFull) >= 0x0000000000000800ull) return (prod >> 12) + 1ull;
  return prod >> 12;
}

/* Returns ROUND((num * mul) / 2^13) */
template <> uint64_t multshiftround<uint64_t, 13>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000001FFFull) >= 0x0000000000001000ull) return (prod >> 13) + 1ull;
  return prod >> 13;
}

/* Returns ROUND((num * mul) / 2^14) */
template <> uint64_t multshiftround<uint64_t, 14>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000003FFFull) >= 0x0000000000002000ull) return (prod >> 14) + 1ull;
  return prod >> 14;
}

/* Returns ROUND((num * mul) / 2^15) */
template <> uint64_t multshiftround<uint64_t, 15>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000007FFFull) >= 0x0000000000004000ull) return (prod >> 15) + 1ull;
  return prod >> 15;
}

/* Returns ROUND((num * mul) / 2^16) */
template <> uint64_t multshiftround<uint64_t, 16>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000000FFFFull) >= 0x0000000000008000ull) return (prod >> 16) + 1ull;
  return prod >> 16;
}

/* Returns ROUND((num * mul) / 2^17) */
template <> uint64_t multshiftround<uint64_t, 17>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000001FFFFull) >= 0x0000000000010000ull) return (prod >> 17) + 1ull;
  return prod >> 17;
}

/* Returns ROUND((num * mul) / 2^18) */
template <> uint64_t multshiftround<uint64_t, 18>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000003FFFFull) >= 0x0000000000020000ull) return (prod >> 18) + 1ull;
  return prod >> 18;
}

/* Returns ROUND((num * mul) / 2^19) */
template <> uint64_t multshiftround<uint64_t, 19>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000007FFFFull) >= 0x0000000000040000ull) return (prod >> 19) + 1ull;
  return prod >> 19;
}

/* Returns ROUND((num * mul) / 2^20) */
template <> uint64_t multshiftround<uint64_t, 20>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000000FFFFFull) >= 0x0000000000080000ull) return (prod >> 20) + 1ull;
  return prod >> 20;
}

/* Returns ROUND((num * mul) / 2^21) */
template <> uint64_t multshiftround<uint64_t, 21>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000001FFFFFull) >= 0x0000000000100000ull) return (prod >> 21) + 1ull;
  return prod >> 21;
}

/* Returns ROUND((num * mul) / 2^22) */
template <> uint64_t multshiftround<uint64_t, 22>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000003FFFFFull) >= 0x0000000000200000ull) return (prod >> 22) + 1ull;
  return prod >> 22;
}

/* Returns ROUND((num * mul) / 2^23) */
template <> uint64_t multshiftround<uint64_t, 23>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000007FFFFFull) >= 0x0000000000400000ull) return (prod >> 23) + 1ull;
  return prod >> 23;
}

/* Returns ROUND((num * mul) / 2^24) */
template <> uint64_t multshiftround<uint64_t, 24>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000000FFFFFFull) >= 0x0000000000800000ull) return (prod >> 24) + 1ull;
  return prod >> 24;
}

/* Returns ROUND((num * mul) / 2^25) */
template <> uint64_t multshiftround<uint64_t, 25>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000001FFFFFFull) >= 0x0000000001000000ull) return (prod >> 25) + 1ull;
  return prod >> 25;
}

/* Returns ROUND((num * mul) / 2^26) */
template <> uint64_t multshiftround<uint64_t, 26>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000003FFFFFFull) >= 0x0000000002000000ull) return (prod >> 26) + 1ull;
  return prod >> 26;
}

/* Returns ROUND((num * mul) / 2^27) */
template <> uint64_t multshiftround<uint64_t, 27>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000007FFFFFFull) >= 0x0000000004000000ull) return (prod >> 27) + 1ull;
  return prod >> 27;
}

/* Returns ROUND((num * mul) / 2^28) */
template <> uint64_t multshiftround<uint64_t, 28>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000000FFFFFFFull) >= 0x0000000008000000ull) return (prod >> 28) + 1ull;
  return prod >> 28;
}

/* Returns ROUND((num * mul) / 2^29) */
template <> uint64_t multshiftround<uint64_t, 29>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000001FFFFFFFull) >= 0x0000000010000000ull) return (prod >> 29) + 1ull;
  return prod >> 29;
}

/* Returns ROUND((num * mul) / 2^30) */
template <> uint64_t multshiftround<uint64_t, 30>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000003FFFFFFFull) >= 0x0000000020000000ull) return (prod >> 30) + 1ull;
  return prod >> 30;
}

/* Returns ROUND((num * mul) / 2^31) */
template <> uint64_t multshiftround<uint64_t, 31>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000007FFFFFFFull) >= 0x0000000040000000ull) return (prod >> 31) + 1ull;
  return prod >> 31;
}

/* Returns ROUND((num * mul) / 2^32) */
template <> uint64_t multshiftround<uint64_t, 32>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000000FFFFFFFFull) >= 0x0000000080000000ull) return (prod >> 32) + 1ull;
  return prod >> 32;
}

/* Returns ROUND((num * mul) / 2^33) */
template <> uint64_t multshiftround<uint64_t, 33>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000001FFFFFFFFull) >= 0x0000000100000000ull) return (prod >> 33) + 1ull;
  return prod >> 33;
}

/* Returns ROUND((num * mul) / 2^34) */
template <> uint64_t multshiftround<uint64_t, 34>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000003FFFFFFFFull) >= 0x0000000200000000ull) return (prod >> 34) + 1ull;
  return prod >> 34;
}

/* Returns ROUND((num * mul) / 2^35) */
template <> uint64_t multshiftround<uint64_t, 35>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000007FFFFFFFFull) >= 0x0000000400000000ull) return (prod >> 35) + 1ull;
  return prod >> 35;
}

/* Returns ROUND((num * mul) / 2^36) */
template <> uint64_t multshiftround<uint64_t, 36>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000000FFFFFFFFFull) >= 0x0000000800000000ull) return (prod >> 36) + 1ull;
  return prod >> 36;
}

/* Returns ROUND((num * mul) / 2^37) */
template <> uint64_t multshiftround<uint64_t, 37>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000001FFFFFFFFFull) >= 0x0000001000000000ull) return (prod >> 37) + 1ull;
  return prod >> 37;
}

/* Returns ROUND((num * mul) / 2^38) */
template <> uint64_t multshiftround<uint64_t, 38>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000003FFFFFFFFFull) >= 0x0000002000000000ull) return (prod >> 38) + 1ull;
  return prod >> 38;
}

/* Returns ROUND((num * mul) / 2^39) */
template <> uint64_t multshiftround<uint64_t, 39>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000007FFFFFFFFFull) >= 0x0000004000000000ull) return (prod >> 39) + 1ull;
  return prod >> 39;
}

/* Returns ROUND((num * mul) / 2^40) */
template <> uint64_t multshiftround<uint64_t, 40>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000000FFFFFFFFFFull) >= 0x0000008000000000ull) return (prod >> 40) + 1ull;
  return prod >> 40;
}

/* Returns ROUND((num * mul) / 2^41) */
template <> uint64_t multshiftround<uint64_t, 41>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000001FFFFFFFFFFull) >= 0x0000010000000000ull) return (prod >> 41) + 1ull;
  return prod >> 41;
}

/* Returns ROUND((num * mul) / 2^42) */
template <> uint64_t multshiftround<uint64_t, 42>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000003FFFFFFFFFFull) >= 0x0000020000000000ull) return (prod >> 42) + 1ull;
  return prod >> 42;
}

/* Returns ROUND((num * mul) / 2^43) */
template <> uint64_t multshiftround<uint64_t, 43>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000007FFFFFFFFFFull) >= 0x0000040000000000ull) return (prod >> 43) + 1ull;
  return prod >> 43;
}

/* Returns ROUND((num * mul) / 2^44) */
template <> uint64_t multshiftround<uint64_t, 44>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00000FFFFFFFFFFFull) >= 0x0000080000000000ull) return (prod >> 44) + 1ull;
  return prod >> 44;
}

/* Returns ROUND((num * mul) / 2^45) */
template <> uint64_t multshiftround<uint64_t, 45>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00001FFFFFFFFFFFull) >= 0x0000100000000000ull) return (prod >> 45) + 1ull;
  return prod >> 45;
}

/* Returns ROUND((num * mul) / 2^46) */
template <> uint64_t multshiftround<uint64_t, 46>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00003FFFFFFFFFFFull) >= 0x0000200000000000ull) return (prod >> 46) + 1ull;
  return prod >> 46;
}

/* Returns ROUND((num * mul) / 2^47) */
template <> uint64_t multshiftround<uint64_t, 47>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00007FFFFFFFFFFFull) >= 0x0000400000000000ull) return (prod >> 47) + 1ull;
  return prod >> 47;
}

/* Returns ROUND((num * mul) / 2^48) */
template <> uint64_t multshiftround<uint64_t, 48>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0000FFFFFFFFFFFFull) >= 0x0000800000000000ull) return (prod >> 48) + 1ull;
  return prod >> 48;
}

/* Returns ROUND((num * mul) / 2^49) */
template <> uint64_t multshiftround<uint64_t, 49>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0001FFFFFFFFFFFFull) >= 0x0001000000000000ull) return (prod >> 49) + 1ull;
  return prod >> 49;
}

/* Returns ROUND((num * mul) / 2^50) */
template <> uint64_t multshiftround<uint64_t, 50>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0003FFFFFFFFFFFFull) >= 0x0002000000000000ull) return (prod >> 50) + 1ull;
  return prod >> 50;
}

/* Returns ROUND((num * mul) / 2^51) */
template <> uint64_t multshiftround<uint64_t, 51>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0007FFFFFFFFFFFFull) >= 0x0004000000000000ull) return (prod >> 51) + 1ull;
  return prod >> 51;
}

/* Returns ROUND((num * mul) / 2^52) */
template <> uint64_t multshiftround<uint64_t, 52>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x000FFFFFFFFFFFFFull) >= 0x0008000000000000ull) return (prod >> 52) + 1ull;
  return prod >> 52;
}

/* Returns ROUND((num * mul) / 2^53) */
template <> uint64_t multshiftround<uint64_t, 53>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x001FFFFFFFFFFFFFull) >= 0x0010000000000000ull) return (prod >> 53) + 1ull;
  return prod >> 53;
}

/* Returns ROUND((num * mul) / 2^54) */
template <> uint64_t multshiftround<uint64_t, 54>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x003FFFFFFFFFFFFFull) >= 0x0020000000000000ull) return (prod >> 54) + 1ull;
  return prod >> 54;
}

/* Returns ROUND((num * mul) / 2^55) */
template <> uint64_t multshiftround<uint64_t, 55>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x007FFFFFFFFFFFFFull) >= 0x0040000000000000ull) return (prod >> 55) + 1ull;
  return prod >> 55;
}

/* Returns ROUND((num * mul) / 2^56) */
template <> uint64_t multshiftround<uint64_t, 56>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x00FFFFFFFFFFFFFFull) >= 0x0080000000000000ull) return (prod >> 56) + 1ull;
  return prod >> 56;
}

/* Returns ROUND((num * mul) / 2^57) */
template <> uint64_t multshiftround<uint64_t, 57>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x01FFFFFFFFFFFFFFull) >= 0x0100000000000000ull) return (prod >> 57) + 1ull;
  return prod >> 57;
}

/* Returns ROUND((num * mul) / 2^58) */
template <> uint64_t multshiftround<uint64_t, 58>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x03FFFFFFFFFFFFFFull) >= 0x0200000000000000ull) return (prod >> 58) + 1ull;
  return prod >> 58;
}

/* Returns ROUND((num * mul) / 2^59) */
template <> uint64_t multshiftround<uint64_t, 59>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x07FFFFFFFFFFFFFFull) >= 0x0400000000000000ull) return (prod >> 59) + 1ull;
  return prod >> 59;
}

/* Returns ROUND((num * mul) / 2^60) */
template <> uint64_t multshiftround<uint64_t, 60>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x0FFFFFFFFFFFFFFFull) >= 0x0800000000000000ull) return (prod >> 60) + 1ull;
  return prod >> 60;
}

/* Returns ROUND((num * mul) / 2^61) */
template <> uint64_t multshiftround<uint64_t, 61>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x1FFFFFFFFFFFFFFFull) >= 0x1000000000000000ull) return (prod >> 61) + 1ull;
  return prod >> 61;
}

/* Returns ROUND((num * mul) / 2^62) */
template <> uint64_t multshiftround<uint64_t, 62>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x3FFFFFFFFFFFFFFFull) >= 0x2000000000000000ull) return (prod >> 62) + 1ull;
  return prod >> 62;
}

/* Returns ROUND((num * mul) / 2^63) */
template <> uint64_t multshiftround<uint64_t, 63>(const uint64_t num, const uint64_t mul) {
  uint64_t prod = num * mul;
  if ((prod & 0x7FFFFFFFFFFFFFFFull) >= 0x4000000000000000ull) return (prod >> 63) + 1ull;
  return prod >> 63;
}

#endif /* #ifndef MULSHFTRND_HPP_ */

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