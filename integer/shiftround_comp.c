/**
 * shiftround_comp.c
 * Defines functions of the form
 *     type shiftround_X_Y(const type num);
 * where X is a type abbreviation and Y is a shift value. These functions
 * return the value ROUND(num / 2^Y) without using the division operator.
 * The _comp in shiftround_comp.c indicates the shift value must be known
 * at compile time.
 *
 * These functions are implemented for the types int8_t, int16_t, int32_t,
 * int64_t, uint8_t, uint16_t, uint32_t, and uint64_t.
 *
 * Y ranges from 1 to one less than the the word length of the integer type
 * for unsigned types. Y ranges from 1 to two less than the word length of
 * the integer type for signed types.
 *
 * Correct operation for negative signed inputs requires two things:
 * 1. The representation of signed integers must be 2's complement.
 * 2. The compiler must encode right shifts on signed types as arithmetic
 *    right shifts rather than logical right shifts.
 *
 * Written in 2018 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */

#include "shiftround_comp.h"

/********************************************************************************
 ********                        int8_t functions                        ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
int8_t shiftround_i8_1(const int8_t num) {
  if ((num & (uint8_t)0x81) == (int8_t)0x01) return (num >> 1) + (int8_t)1;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
int8_t shiftround_i8_2(const int8_t num) {
  if ((num & (int8_t)0x02) && ((num & (uint8_t)0x83) != (uint8_t)0x82)) return (num >> 2) + (int8_t)1;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
int8_t shiftround_i8_3(const int8_t num) {
  if ((num & (int8_t)0x04) && ((num & (uint8_t)0x87) != (uint8_t)0x84)) return (num >> 3) + (int8_t)1;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
int8_t shiftround_i8_4(const int8_t num) {
  if ((num & (int8_t)0x08) && ((num & (uint8_t)0x8F) != (uint8_t)0x88)) return (num >> 4) + (int8_t)1;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
int8_t shiftround_i8_5(const int8_t num) {
  if ((num & (int8_t)0x10) && ((num & (uint8_t)0x9F) != (uint8_t)0x90)) return (num >> 5) + (int8_t)1;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
int8_t shiftround_i8_6(const int8_t num) {
  if ((num & (int8_t)0x20) && ((num & (uint8_t)0xBF) != (uint8_t)0xA0)) return (num >> 6) + (int8_t)1;
  return num >> 6;
}

/********************************************************************************
 ********                       int16_t functions                        ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
int16_t shiftround_i16_1(const int16_t num) {
  if ((num & (uint16_t)0x8001) == (int16_t)0x0001) return (num >> 1) + (int16_t)1;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
int16_t shiftround_i16_2(const int16_t num) {
  if ((num & (int16_t)0x0002) && ((num & (uint16_t)0x8003) != (uint16_t)0x8002)) return (num >> 2) + (int16_t)1;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
int16_t shiftround_i16_3(const int16_t num) {
  if ((num & (int16_t)0x0004) && ((num & (uint16_t)0x8007) != (uint16_t)0x8004)) return (num >> 3) + (int16_t)1;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
int16_t shiftround_i16_4(const int16_t num) {
  if ((num & (int16_t)0x0008) && ((num & (uint16_t)0x800F) != (uint16_t)0x8008)) return (num >> 4) + (int16_t)1;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
int16_t shiftround_i16_5(const int16_t num) {
  if ((num & (int16_t)0x0010) && ((num & (uint16_t)0x801F) != (uint16_t)0x8010)) return (num >> 5) + (int16_t)1;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
int16_t shiftround_i16_6(const int16_t num) {
  if ((num & (int16_t)0x0020) && ((num & (uint16_t)0x803F) != (uint16_t)0x8020)) return (num >> 6) + (int16_t)1;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
int16_t shiftround_i16_7(const int16_t num) {
  if ((num & (int16_t)0x0040) && ((num & (uint16_t)0x807F) != (uint16_t)0x8040)) return (num >> 7) + (int16_t)1;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
int16_t shiftround_i16_8(const int16_t num) {
  if ((num & (int16_t)0x0080) && ((num & (uint16_t)0x80FF) != (uint16_t)0x8080)) return (num >> 8) + (int16_t)1;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
int16_t shiftround_i16_9(const int16_t num) {
  if ((num & (int16_t)0x0100) && ((num & (uint16_t)0x81FF) != (uint16_t)0x8100)) return (num >> 9) + (int16_t)1;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
int16_t shiftround_i16_10(const int16_t num) {
  if ((num & (int16_t)0x0200) && ((num & (uint16_t)0x83FF) != (uint16_t)0x8200)) return (num >> 10) + (int16_t)1;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
int16_t shiftround_i16_11(const int16_t num) {
  if ((num & (int16_t)0x0400) && ((num & (uint16_t)0x87FF) != (uint16_t)0x8400)) return (num >> 11) + (int16_t)1;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
int16_t shiftround_i16_12(const int16_t num) {
  if ((num & (int16_t)0x0800) && ((num & (uint16_t)0x8FFF) != (uint16_t)0x8800)) return (num >> 12) + (int16_t)1;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
int16_t shiftround_i16_13(const int16_t num) {
  if ((num & (int16_t)0x1000) && ((num & (uint16_t)0x9FFF) != (uint16_t)0x9000)) return (num >> 13) + (int16_t)1;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
int16_t shiftround_i16_14(const int16_t num) {
  if ((num & (int16_t)0x2000) && ((num & (uint16_t)0xBFFF) != (uint16_t)0xA000)) return (num >> 14) + (int16_t)1;
  return num >> 14;
}

/********************************************************************************
 ********                       int32_t functions                        ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
int32_t shiftround_i32_1(const int32_t num) {
  if ((num & 0x80000001u) == 0x00000001) return (num >> 1) + 1;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
int32_t shiftround_i32_2(const int32_t num) {
  if ((num & 0x00000002) && ((num & 0x80000003u) != 0x80000002u)) return (num >> 2) + 1;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
int32_t shiftround_i32_3(const int32_t num) {
  if ((num & 0x00000004) && ((num & 0x80000007u) != 0x80000004u)) return (num >> 3) + 1;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
int32_t shiftround_i32_4(const int32_t num) {
  if ((num & 0x00000008) && ((num & 0x8000000Fu) != 0x80000008u)) return (num >> 4) + 1;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
int32_t shiftround_i32_5(const int32_t num) {
  if ((num & 0x00000010) && ((num & 0x8000001Fu) != 0x80000010u)) return (num >> 5) + 1;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
int32_t shiftround_i32_6(const int32_t num) {
  if ((num & 0x00000020) && ((num & 0x8000003Fu) != 0x80000020u)) return (num >> 6) + 1;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
int32_t shiftround_i32_7(const int32_t num) {
  if ((num & 0x00000040) && ((num & 0x8000007Fu) != 0x80000040u)) return (num >> 7) + 1;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
int32_t shiftround_i32_8(const int32_t num) {
  if ((num & 0x00000080) && ((num & 0x800000FFu) != 0x80000080u)) return (num >> 8) + 1;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
int32_t shiftround_i32_9(const int32_t num) {
  if ((num & 0x00000100) && ((num & 0x800001FFu) != 0x80000100u)) return (num >> 9) + 1;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
int32_t shiftround_i32_10(const int32_t num) {
  if ((num & 0x00000200) && ((num & 0x800003FFu) != 0x80000200u)) return (num >> 10) + 1;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
int32_t shiftround_i32_11(const int32_t num) {
  if ((num & 0x00000400) && ((num & 0x800007FFu) != 0x80000400u)) return (num >> 11) + 1;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
int32_t shiftround_i32_12(const int32_t num) {
  if ((num & 0x00000800) && ((num & 0x80000FFFu) != 0x80000800u)) return (num >> 12) + 1;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
int32_t shiftround_i32_13(const int32_t num) {
  if ((num & 0x00001000) && ((num & 0x80001FFFu) != 0x80001000u)) return (num >> 13) + 1;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
int32_t shiftround_i32_14(const int32_t num) {
  if ((num & 0x00002000) && ((num & 0x80003FFFu) != 0x80002000u)) return (num >> 14) + 1;
  return num >> 14;
}

/* Returns ROUND(num / 2^15) */
int32_t shiftround_i32_15(const int32_t num) {
  if ((num & 0x00004000) && ((num & 0x80007FFFu) != 0x80004000u)) return (num >> 15) + 1;
  return num >> 15;
}

/* Returns ROUND(num / 2^16) */
int32_t shiftround_i32_16(const int32_t num) {
  if ((num & 0x00008000) && ((num & 0x8000FFFFu) != 0x80008000u)) return (num >> 16) + 1;
  return num >> 16;
}

/* Returns ROUND(num / 2^17) */
int32_t shiftround_i32_17(const int32_t num) {
  if ((num & 0x00010000) && ((num & 0x8001FFFFu) != 0x80010000u)) return (num >> 17) + 1;
  return num >> 17;
}

/* Returns ROUND(num / 2^18) */
int32_t shiftround_i32_18(const int32_t num) {
  if ((num & 0x00020000) && ((num & 0x8003FFFFu) != 0x80020000u)) return (num >> 18) + 1;
  return num >> 18;
}

/* Returns ROUND(num / 2^19) */
int32_t shiftround_i32_19(const int32_t num) {
  if ((num & 0x00040000) && ((num & 0x8007FFFFu) != 0x80040000u)) return (num >> 19) + 1;
  return num >> 19;
}

/* Returns ROUND(num / 2^20) */
int32_t shiftround_i32_20(const int32_t num) {
  if ((num & 0x00080000) && ((num & 0x800FFFFFu) != 0x80080000u)) return (num >> 20) + 1;
  return num >> 20;
}

/* Returns ROUND(num / 2^21) */
int32_t shiftround_i32_21(const int32_t num) {
  if ((num & 0x00100000) && ((num & 0x801FFFFFu) != 0x80100000u)) return (num >> 21) + 1;
  return num >> 21;
}

/* Returns ROUND(num / 2^22) */
int32_t shiftround_i32_22(const int32_t num) {
  if ((num & 0x00200000) && ((num & 0x803FFFFFu) != 0x80200000u)) return (num >> 22) + 1;
  return num >> 22;
}

/* Returns ROUND(num / 2^23) */
int32_t shiftround_i32_23(const int32_t num) {
  if ((num & 0x00400000) && ((num & 0x807FFFFFu) != 0x80400000u)) return (num >> 23) + 1;
  return num >> 23;
}

/* Returns ROUND(num / 2^24) */
int32_t shiftround_i32_24(const int32_t num) {
  if ((num & 0x00800000) && ((num & 0x80FFFFFFu) != 0x80800000u)) return (num >> 24) + 1;
  return num >> 24;
}

/* Returns ROUND(num / 2^25) */
int32_t shiftround_i32_25(const int32_t num) {
  if ((num & 0x01000000) && ((num & 0x81FFFFFFu) != 0x81000000u)) return (num >> 25) + 1;
  return num >> 25;
}

/* Returns ROUND(num / 2^26) */
int32_t shiftround_i32_26(const int32_t num) {
  if ((num & 0x02000000) && ((num & 0x83FFFFFFu) != 0x82000000u)) return (num >> 26) + 1;
  return num >> 26;
}

/* Returns ROUND(num / 2^27) */
int32_t shiftround_i32_27(const int32_t num) {
  if ((num & 0x04000000) && ((num & 0x87FFFFFFu) != 0x84000000u)) return (num >> 27) + 1;
  return num >> 27;
}

/* Returns ROUND(num / 2^28) */
int32_t shiftround_i32_28(const int32_t num) {
  if ((num & 0x08000000) && ((num & 0x8FFFFFFFu) != 0x88000000u)) return (num >> 28) + 1;
  return num >> 28;
}

/* Returns ROUND(num / 2^29) */
int32_t shiftround_i32_29(const int32_t num) {
  if ((num & 0x10000000) && ((num & 0x9FFFFFFFu) != 0x90000000u)) return (num >> 29) + 1;
  return num >> 29;
}

/* Returns ROUND(num / 2^30) */
int32_t shiftround_i32_30(const int32_t num) {
  if ((num & 0x20000000) && ((num & 0xBFFFFFFFu) != 0xA0000000u)) return (num >> 30) + 1;
  return num >> 30;
}

/********************************************************************************
 ********                       int64_t functions                        ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
int64_t shiftround_i64_1(const int64_t num) {
  if ((num & 0x8000000000000001ull) == 0x0000000000000001ull) return (num >> 1) + 1ll;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
int64_t shiftround_i64_2(const int64_t num) {
  if ((num & 0x0000000000000002ll) && ((num & 0x8000000000000003ull) != 0x8000000000000002ull)) return (num >> 2) + 1ll;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
int64_t shiftround_i64_3(const int64_t num) {
  if ((num & 0x0000000000000004ll) && ((num & 0x8000000000000007ull) != 0x8000000000000004ull)) return (num >> 3) + 1ll;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
int64_t shiftround_i64_4(const int64_t num) {
  if ((num & 0x0000000000000008ll) && ((num & 0x800000000000000Full) != 0x8000000000000008ull)) return (num >> 4) + 1ll;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
int64_t shiftround_i64_5(const int64_t num) {
  if ((num & 0x0000000000000010ll) && ((num & 0x800000000000001Full) != 0x8000000000000010ull)) return (num >> 5) + 1ll;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
int64_t shiftround_i64_6(const int64_t num) {
  if ((num & 0x0000000000000020ll) && ((num & 0x800000000000003Full) != 0x8000000000000020ull)) return (num >> 6) + 1ll;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
int64_t shiftround_i64_7(const int64_t num) {
  if ((num & 0x0000000000000040ll) && ((num & 0x800000000000007Full) != 0x8000000000000040ull)) return (num >> 7) + 1ll;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
int64_t shiftround_i64_8(const int64_t num) {
  if ((num & 0x0000000000000080ll) && ((num & 0x80000000000000FFull) != 0x8000000000000080ull)) return (num >> 8) + 1ll;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
int64_t shiftround_i64_9(const int64_t num) {
  if ((num & 0x0000000000000100ll) && ((num & 0x80000000000001FFull) != 0x8000000000000100ull)) return (num >> 9) + 1ll;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
int64_t shiftround_i64_10(const int64_t num) {
  if ((num & 0x0000000000000200ll) && ((num & 0x80000000000003FFull) != 0x8000000000000200ull)) return (num >> 10) + 1ll;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
int64_t shiftround_i64_11(const int64_t num) {
  if ((num & 0x0000000000000400ll) && ((num & 0x80000000000007FFull) != 0x8000000000000400ull)) return (num >> 11) + 1ll;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
int64_t shiftround_i64_12(const int64_t num) {
  if ((num & 0x0000000000000800ll) && ((num & 0x8000000000000FFFull) != 0x8000000000000800ull)) return (num >> 12) + 1ll;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
int64_t shiftround_i64_13(const int64_t num) {
  if ((num & 0x0000000000001000ll) && ((num & 0x8000000000001FFFull) != 0x8000000000001000ull)) return (num >> 13) + 1ll;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
int64_t shiftround_i64_14(const int64_t num) {
  if ((num & 0x0000000000002000ll) && ((num & 0x8000000000003FFFull) != 0x8000000000002000ull)) return (num >> 14) + 1ll;
  return num >> 14;
}

/* Returns ROUND(num / 2^15) */
int64_t shiftround_i64_15(const int64_t num) {
  if ((num & 0x0000000000004000ll) && ((num & 0x8000000000007FFFull) != 0x8000000000004000ull)) return (num >> 15) + 1ll;
  return num >> 15;
}

/* Returns ROUND(num / 2^16) */
int64_t shiftround_i64_16(const int64_t num) {
  if ((num & 0x0000000000008000ll) && ((num & 0x800000000000FFFFull) != 0x8000000000008000ull)) return (num >> 16) + 1ll;
  return num >> 16;
}

/* Returns ROUND(num / 2^17) */
int64_t shiftround_i64_17(const int64_t num) {
  if ((num & 0x0000000000010000ll) && ((num & 0x800000000001FFFFull) != 0x8000000000010000ull)) return (num >> 17) + 1ll;
  return num >> 17;
}

/* Returns ROUND(num / 2^18) */
int64_t shiftround_i64_18(const int64_t num) {
  if ((num & 0x0000000000020000ll) && ((num & 0x800000000003FFFFull) != 0x8000000000020000ull)) return (num >> 18) + 1ll;
  return num >> 18;
}

/* Returns ROUND(num / 2^19) */
int64_t shiftround_i64_19(const int64_t num) {
  if ((num & 0x0000000000040000ll) && ((num & 0x800000000007FFFFull) != 0x8000000000040000ull)) return (num >> 19) + 1ll;
  return num >> 19;
}

/* Returns ROUND(num / 2^20) */
int64_t shiftround_i64_20(const int64_t num) {
  if ((num & 0x0000000000080000ll) && ((num & 0x80000000000FFFFFull) != 0x8000000000080000ull)) return (num >> 20) + 1ll;
  return num >> 20;
}

/* Returns ROUND(num / 2^21) */
int64_t shiftround_i64_21(const int64_t num) {
  if ((num & 0x0000000000100000ll) && ((num & 0x80000000001FFFFFull) != 0x8000000000100000ull)) return (num >> 21) + 1ll;
  return num >> 21;
}

/* Returns ROUND(num / 2^22) */
int64_t shiftround_i64_22(const int64_t num) {
  if ((num & 0x0000000000200000ll) && ((num & 0x80000000003FFFFFull) != 0x8000000000200000ull)) return (num >> 22) + 1ll;
  return num >> 22;
}

/* Returns ROUND(num / 2^23) */
int64_t shiftround_i64_23(const int64_t num) {
  if ((num & 0x0000000000400000ll) && ((num & 0x80000000007FFFFFull) != 0x8000000000400000ull)) return (num >> 23) + 1ll;
  return num >> 23;
}

/* Returns ROUND(num / 2^24) */
int64_t shiftround_i64_24(const int64_t num) {
  if ((num & 0x0000000000800000ll) && ((num & 0x8000000000FFFFFFull) != 0x8000000000800000ull)) return (num >> 24) + 1ll;
  return num >> 24;
}

/* Returns ROUND(num / 2^25) */
int64_t shiftround_i64_25(const int64_t num) {
  if ((num & 0x0000000001000000ll) && ((num & 0x8000000001FFFFFFull) != 0x8000000001000000ull)) return (num >> 25) + 1ll;
  return num >> 25;
}

/* Returns ROUND(num / 2^26) */
int64_t shiftround_i64_26(const int64_t num) {
  if ((num & 0x0000000002000000ll) && ((num & 0x8000000003FFFFFFull) != 0x8000000002000000ull)) return (num >> 26) + 1ll;
  return num >> 26;
}

/* Returns ROUND(num / 2^27) */
int64_t shiftround_i64_27(const int64_t num) {
  if ((num & 0x0000000004000000ll) && ((num & 0x8000000007FFFFFFull) != 0x8000000004000000ull)) return (num >> 27) + 1ll;
  return num >> 27;
}

/* Returns ROUND(num / 2^28) */
int64_t shiftround_i64_28(const int64_t num) {
  if ((num & 0x0000000008000000ll) && ((num & 0x800000000FFFFFFFull) != 0x8000000008000000ull)) return (num >> 28) + 1ll;
  return num >> 28;
}

/* Returns ROUND(num / 2^29) */
int64_t shiftround_i64_29(const int64_t num) {
  if ((num & 0x0000000010000000ll) && ((num & 0x800000001FFFFFFFull) != 0x8000000010000000ull)) return (num >> 29) + 1ll;
  return num >> 29;
}

/* Returns ROUND(num / 2^30) */
int64_t shiftround_i64_30(const int64_t num) {
  if ((num & 0x0000000020000000ll) && ((num & 0x800000003FFFFFFFull) != 0x8000000020000000ull)) return (num >> 30) + 1ll;
  return num >> 30;
}

/* Returns ROUND(num / 2^31) */
int64_t shiftround_i64_31(const int64_t num) {
  if ((num & 0x0000000040000000ll) && ((num & 0x800000007FFFFFFFull) != 0x8000000040000000ull)) return (num >> 31) + 1ll;
  return num >> 31;
}

/* Returns ROUND(num / 2^32) */
int64_t shiftround_i64_32(const int64_t num) {
  if ((num & 0x0000000080000000ll) && ((num & 0x80000000FFFFFFFFull) != 0x8000000080000000ull)) return (num >> 32) + 1ll;
  return num >> 32;
}

/* Returns ROUND(num / 2^33) */
int64_t shiftround_i64_33(const int64_t num) {
  if ((num & 0x0000000100000000ll) && ((num & 0x80000001FFFFFFFFull) != 0x8000000100000000ull)) return (num >> 33) + 1ll;
  return num >> 33;
}

/* Returns ROUND(num / 2^34) */
int64_t shiftround_i64_34(const int64_t num) {
  if ((num & 0x0000000200000000ll) && ((num & 0x80000003FFFFFFFFull) != 0x8000000200000000ull)) return (num >> 34) + 1ll;
  return num >> 34;
}

/* Returns ROUND(num / 2^35) */
int64_t shiftround_i64_35(const int64_t num) {
  if ((num & 0x0000000400000000ll) && ((num & 0x80000007FFFFFFFFull) != 0x8000000400000000ull)) return (num >> 35) + 1ll;
  return num >> 35;
}

/* Returns ROUND(num / 2^36) */
int64_t shiftround_i64_36(const int64_t num) {
  if ((num & 0x0000000800000000ll) && ((num & 0x8000000FFFFFFFFFull) != 0x8000000800000000ull)) return (num >> 36) + 1ll;
  return num >> 36;
}

/* Returns ROUND(num / 2^37) */
int64_t shiftround_i64_37(const int64_t num) {
  if ((num & 0x0000001000000000ll) && ((num & 0x8000001FFFFFFFFFull) != 0x8000001000000000ull)) return (num >> 37) + 1ll;
  return num >> 37;
}

/* Returns ROUND(num / 2^38) */
int64_t shiftround_i64_38(const int64_t num) {
  if ((num & 0x0000002000000000ll) && ((num & 0x8000003FFFFFFFFFull) != 0x8000002000000000ull)) return (num >> 38) + 1ll;
  return num >> 38;
}

/* Returns ROUND(num / 2^39) */
int64_t shiftround_i64_39(const int64_t num) {
  if ((num & 0x0000004000000000ll) && ((num & 0x8000007FFFFFFFFFull) != 0x8000004000000000ull)) return (num >> 39) + 1ll;
  return num >> 39;
}

/* Returns ROUND(num / 2^40) */
int64_t shiftround_i64_40(const int64_t num) {
  if ((num & 0x0000008000000000ll) && ((num & 0x800000FFFFFFFFFFull) != 0x8000008000000000ull)) return (num >> 40) + 1ll;
  return num >> 40;
}

/* Returns ROUND(num / 2^41) */
int64_t shiftround_i64_41(const int64_t num) {
  if ((num & 0x0000010000000000ll) && ((num & 0x800001FFFFFFFFFFull) != 0x8000010000000000ull)) return (num >> 41) + 1ll;
  return num >> 41;
}

/* Returns ROUND(num / 2^42) */
int64_t shiftround_i64_42(const int64_t num) {
  if ((num & 0x0000020000000000ll) && ((num & 0x800003FFFFFFFFFFull) != 0x8000020000000000ull)) return (num >> 42) + 1ll;
  return num >> 42;
}

/* Returns ROUND(num / 2^43) */
int64_t shiftround_i64_43(const int64_t num) {
  if ((num & 0x0000040000000000ll) && ((num & 0x800007FFFFFFFFFFull) != 0x8000040000000000ull)) return (num >> 43) + 1ll;
  return num >> 43;
}

/* Returns ROUND(num / 2^44) */
int64_t shiftround_i64_44(const int64_t num) {
  if ((num & 0x0000080000000000ll) && ((num & 0x80000FFFFFFFFFFFull) != 0x8000080000000000ull)) return (num >> 44) + 1ll;
  return num >> 44;
}

/* Returns ROUND(num / 2^45) */
int64_t shiftround_i64_45(const int64_t num) {
  if ((num & 0x0000100000000000ll) && ((num & 0x80001FFFFFFFFFFFull) != 0x8000100000000000ull)) return (num >> 45) + 1ll;
  return num >> 45;
}

/* Returns ROUND(num / 2^46) */
int64_t shiftround_i64_46(const int64_t num) {
  if ((num & 0x0000200000000000ll) && ((num & 0x80003FFFFFFFFFFFull) != 0x8000200000000000ull)) return (num >> 46) + 1ll;
  return num >> 46;
}

/* Returns ROUND(num / 2^47) */
int64_t shiftround_i64_47(const int64_t num) {
  if ((num & 0x0000400000000000ll) && ((num & 0x80007FFFFFFFFFFFull) != 0x8000400000000000ull)) return (num >> 47) + 1ll;
  return num >> 47;
}

/* Returns ROUND(num / 2^48) */
int64_t shiftround_i64_48(const int64_t num) {
  if ((num & 0x0000800000000000ll) && ((num & 0x8000FFFFFFFFFFFFull) != 0x8000800000000000ull)) return (num >> 48) + 1ll;
  return num >> 48;
}

/* Returns ROUND(num / 2^49) */
int64_t shiftround_i64_49(const int64_t num) {
  if ((num & 0x0001000000000000ll) && ((num & 0x8001FFFFFFFFFFFFull) != 0x8001000000000000ull)) return (num >> 49) + 1ll;
  return num >> 49;
}

/* Returns ROUND(num / 2^50) */
int64_t shiftround_i64_50(const int64_t num) {
  if ((num & 0x0002000000000000ll) && ((num & 0x8003FFFFFFFFFFFFull) != 0x8002000000000000ull)) return (num >> 50) + 1ll;
  return num >> 50;
}

/* Returns ROUND(num / 2^51) */
int64_t shiftround_i64_51(const int64_t num) {
  if ((num & 0x0004000000000000ll) && ((num & 0x8007FFFFFFFFFFFFull) != 0x8004000000000000ull)) return (num >> 51) + 1ll;
  return num >> 51;
}

/* Returns ROUND(num / 2^52) */
int64_t shiftround_i64_52(const int64_t num) {
  if ((num & 0x0008000000000000ll) && ((num & 0x800FFFFFFFFFFFFFull) != 0x8008000000000000ull)) return (num >> 52) + 1ll;
  return num >> 52;
}

/* Returns ROUND(num / 2^53) */
int64_t shiftround_i64_53(const int64_t num) {
  if ((num & 0x0010000000000000ll) && ((num & 0x801FFFFFFFFFFFFFull) != 0x8010000000000000ull)) return (num >> 53) + 1ll;
  return num >> 53;
}

/* Returns ROUND(num / 2^54) */
int64_t shiftround_i64_54(const int64_t num) {
  if ((num & 0x0020000000000000ll) && ((num & 0x803FFFFFFFFFFFFFull) != 0x8020000000000000ull)) return (num >> 54) + 1ll;
  return num >> 54;
}

/* Returns ROUND(num / 2^55) */
int64_t shiftround_i64_55(const int64_t num) {
  if ((num & 0x0040000000000000ll) && ((num & 0x807FFFFFFFFFFFFFull) != 0x8040000000000000ull)) return (num >> 55) + 1ll;
  return num >> 55;
}

/* Returns ROUND(num / 2^56) */
int64_t shiftround_i64_56(const int64_t num) {
  if ((num & 0x0080000000000000ll) && ((num & 0x80FFFFFFFFFFFFFFull) != 0x8080000000000000ull)) return (num >> 56) + 1ll;
  return num >> 56;
}

/* Returns ROUND(num / 2^57) */
int64_t shiftround_i64_57(const int64_t num) {
  if ((num & 0x0100000000000000ll) && ((num & 0x81FFFFFFFFFFFFFFull) != 0x8100000000000000ull)) return (num >> 57) + 1ll;
  return num >> 57;
}

/* Returns ROUND(num / 2^58) */
int64_t shiftround_i64_58(const int64_t num) {
  if ((num & 0x0200000000000000ll) && ((num & 0x83FFFFFFFFFFFFFFull) != 0x8200000000000000ull)) return (num >> 58) + 1ll;
  return num >> 58;
}

/* Returns ROUND(num / 2^59) */
int64_t shiftround_i64_59(const int64_t num) {
  if ((num & 0x0400000000000000ll) && ((num & 0x87FFFFFFFFFFFFFFull) != 0x8400000000000000ull)) return (num >> 59) + 1ll;
  return num >> 59;
}

/* Returns ROUND(num / 2^60) */
int64_t shiftround_i64_60(const int64_t num) {
  if ((num & 0x0800000000000000ll) && ((num & 0x8FFFFFFFFFFFFFFFull) != 0x8800000000000000ull)) return (num >> 60) + 1ll;
  return num >> 60;
}

/* Returns ROUND(num / 2^61) */
int64_t shiftround_i64_61(const int64_t num) {
  if ((num & 0x1000000000000000ll) && ((num & 0x9FFFFFFFFFFFFFFFull) != 0x9000000000000000ull)) return (num >> 61) + 1ll;
  return num >> 61;
}

/* Returns ROUND(num / 2^62) */
int64_t shiftround_i64_62(const int64_t num) {
  if ((num & 0x2000000000000000ll) && ((num & 0xBFFFFFFFFFFFFFFFull) != 0xA000000000000000ull)) return (num >> 62) + 1ll;
  return num >> 62;
}

/********************************************************************************
 ********                       uint8_t functions                        ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
uint8_t shiftround_u8_1(const uint8_t num) {
  if (num & (uint8_t)0x01) return (num >> 1) + (uint8_t)1;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
uint8_t shiftround_u8_2(const uint8_t num) {
  if (num & (uint8_t)0x02) return (num >> 2) + (uint8_t)1;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
uint8_t shiftround_u8_3(const uint8_t num) {
  if (num & (uint8_t)0x04) return (num >> 3) + (uint8_t)1;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
uint8_t shiftround_u8_4(const uint8_t num) {
  if (num & (uint8_t)0x08) return (num >> 4) + (uint8_t)1;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
uint8_t shiftround_u8_5(const uint8_t num) {
  if (num & (uint8_t)0x10) return (num >> 5) + (uint8_t)1;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
uint8_t shiftround_u8_6(const uint8_t num) {
  if (num & (uint8_t)0x20) return (num >> 6) + (uint8_t)1;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
uint8_t shiftround_u8_7(const uint8_t num) {
  if (num & (uint8_t)0x40) return (num >> 7) + (uint8_t)1;
  return num >> 7;
}

/********************************************************************************
 ********                       uint16_t functions                       ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
uint16_t shiftround_u16_1(const uint16_t num) {
  if (num & (uint16_t)0x0001) return (num >> 1) + (uint16_t)1;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
uint16_t shiftround_u16_2(const uint16_t num) {
  if (num & (uint16_t)0x0002) return (num >> 2) + (uint16_t)1;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
uint16_t shiftround_u16_3(const uint16_t num) {
  if (num & (uint16_t)0x0004) return (num >> 3) + (uint16_t)1;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
uint16_t shiftround_u16_4(const uint16_t num) {
  if (num & (uint16_t)0x0008) return (num >> 4) + (uint16_t)1;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
uint16_t shiftround_u16_5(const uint16_t num) {
  if (num & (uint16_t)0x0010) return (num >> 5) + (uint16_t)1;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
uint16_t shiftround_u16_6(const uint16_t num) {
  if (num & (uint16_t)0x0020) return (num >> 6) + (uint16_t)1;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
uint16_t shiftround_u16_7(const uint16_t num) {
  if (num & (uint16_t)0x0040) return (num >> 7) + (uint16_t)1;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
uint16_t shiftround_u16_8(const uint16_t num) {
  if (num & (uint16_t)0x0080) return (num >> 8) + (uint16_t)1;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
uint16_t shiftround_u16_9(const uint16_t num) {
  if (num & (uint16_t)0x0100) return (num >> 9) + (uint16_t)1;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
uint16_t shiftround_u16_10(const uint16_t num) {
  if (num & (uint16_t)0x0200) return (num >> 10) + (uint16_t)1;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
uint16_t shiftround_u16_11(const uint16_t num) {
  if (num & (uint16_t)0x0400) return (num >> 11) + (uint16_t)1;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
uint16_t shiftround_u16_12(const uint16_t num) {
  if (num & (uint16_t)0x0800) return (num >> 12) + (uint16_t)1;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
uint16_t shiftround_u16_13(const uint16_t num) {
  if (num & (uint16_t)0x1000) return (num >> 13) + (uint16_t)1;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
uint16_t shiftround_u16_14(const uint16_t num) {
  if (num & (uint16_t)0x2000) return (num >> 14) + (uint16_t)1;
  return num >> 14;
}

/* Returns ROUND(num / 2^15) */
uint16_t shiftround_u16_15(const uint16_t num) {
  if (num & (uint16_t)0x4000) return (num >> 15) + (uint16_t)1;
  return num >> 15;
}

/********************************************************************************
 ********                       uint32_t functions                       ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
uint32_t shiftround_u32_1(const uint32_t num) {
  if (num & 0x00000001u) return (num >> 1) + 1u;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
uint32_t shiftround_u32_2(const uint32_t num) {
  if (num & 0x00000002u) return (num >> 2) + 1u;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
uint32_t shiftround_u32_3(const uint32_t num) {
  if (num & 0x00000004u) return (num >> 3) + 1u;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
uint32_t shiftround_u32_4(const uint32_t num) {
  if (num & 0x00000008u) return (num >> 4) + 1u;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
uint32_t shiftround_u32_5(const uint32_t num) {
  if (num & 0x00000010u) return (num >> 5) + 1u;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
uint32_t shiftround_u32_6(const uint32_t num) {
  if (num & 0x00000020u) return (num >> 6) + 1u;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
uint32_t shiftround_u32_7(const uint32_t num) {
  if (num & 0x00000040u) return (num >> 7) + 1u;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
uint32_t shiftround_u32_8(const uint32_t num) {
  if (num & 0x00000080u) return (num >> 8) + 1u;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
uint32_t shiftround_u32_9(const uint32_t num) {
  if (num & 0x00000100u) return (num >> 9) + 1u;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
uint32_t shiftround_u32_10(const uint32_t num) {
  if (num & 0x00000200u) return (num >> 10) + 1u;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
uint32_t shiftround_u32_11(const uint32_t num) {
  if (num & 0x00000400u) return (num >> 11) + 1u;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
uint32_t shiftround_u32_12(const uint32_t num) {
  if (num & 0x00000800u) return (num >> 12) + 1u;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
uint32_t shiftround_u32_13(const uint32_t num) {
  if (num & 0x00001000u) return (num >> 13) + 1u;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
uint32_t shiftround_u32_14(const uint32_t num) {
  if (num & 0x00002000u) return (num >> 14) + 1u;
  return num >> 14;
}

/* Returns ROUND(num / 2^15) */
uint32_t shiftround_u32_15(const uint32_t num) {
  if (num & 0x00004000u) return (num >> 15) + 1u;
  return num >> 15;
}

/* Returns ROUND(num / 2^16) */
uint32_t shiftround_u32_16(const uint32_t num) {
  if (num & 0x00008000u) return (num >> 16) + 1u;
  return num >> 16;
}

/* Returns ROUND(num / 2^17) */
uint32_t shiftround_u32_17(const uint32_t num) {
  if (num & 0x00010000u) return (num >> 17) + 1u;
  return num >> 17;
}

/* Returns ROUND(num / 2^18) */
uint32_t shiftround_u32_18(const uint32_t num) {
  if (num & 0x00020000u) return (num >> 18) + 1u;
  return num >> 18;
}

/* Returns ROUND(num / 2^19) */
uint32_t shiftround_u32_19(const uint32_t num) {
  if (num & 0x00040000u) return (num >> 19) + 1u;
  return num >> 19;
}

/* Returns ROUND(num / 2^20) */
uint32_t shiftround_u32_20(const uint32_t num) {
  if (num & 0x00080000u) return (num >> 20) + 1u;
  return num >> 20;
}

/* Returns ROUND(num / 2^21) */
uint32_t shiftround_u32_21(const uint32_t num) {
  if (num & 0x00100000u) return (num >> 21) + 1u;
  return num >> 21;
}

/* Returns ROUND(num / 2^22) */
uint32_t shiftround_u32_22(const uint32_t num) {
  if (num & 0x00200000u) return (num >> 22) + 1u;
  return num >> 22;
}

/* Returns ROUND(num / 2^23) */
uint32_t shiftround_u32_23(const uint32_t num) {
  if (num & 0x00400000u) return (num >> 23) + 1u;
  return num >> 23;
}

/* Returns ROUND(num / 2^24) */
uint32_t shiftround_u32_24(const uint32_t num) {
  if (num & 0x00800000u) return (num >> 24) + 1u;
  return num >> 24;
}

/* Returns ROUND(num / 2^25) */
uint32_t shiftround_u32_25(const uint32_t num) {
  if (num & 0x01000000u) return (num >> 25) + 1u;
  return num >> 25;
}

/* Returns ROUND(num / 2^26) */
uint32_t shiftround_u32_26(const uint32_t num) {
  if (num & 0x02000000u) return (num >> 26) + 1u;
  return num >> 26;
}

/* Returns ROUND(num / 2^27) */
uint32_t shiftround_u32_27(const uint32_t num) {
  if (num & 0x04000000u) return (num >> 27) + 1u;
  return num >> 27;
}

/* Returns ROUND(num / 2^28) */
uint32_t shiftround_u32_28(const uint32_t num) {
  if (num & 0x08000000u) return (num >> 28) + 1u;
  return num >> 28;
}

/* Returns ROUND(num / 2^29) */
uint32_t shiftround_u32_29(const uint32_t num) {
  if (num & 0x10000000u) return (num >> 29) + 1u;
  return num >> 29;
}

/* Returns ROUND(num / 2^30) */
uint32_t shiftround_u32_30(const uint32_t num) {
  if (num & 0x20000000u) return (num >> 30) + 1u;
  return num >> 30;
}

/* Returns ROUND(num / 2^31) */
uint32_t shiftround_u32_31(const uint32_t num) {
  if (num & 0x40000000u) return (num >> 31) + 1u;
  return num >> 31;
}

/********************************************************************************
 ********                       uint64_t functions                       ********
 ********************************************************************************/

/* Returns ROUND(num / 2^1) */
uint64_t shiftround_u64_1(const uint64_t num) {
  if (num & 0x0000000000000001ull) return (num >> 1) + 1ull;
  return num >> 1;
}

/* Returns ROUND(num / 2^2) */
uint64_t shiftround_u64_2(const uint64_t num) {
  if (num & 0x0000000000000002ull) return (num >> 2) + 1ull;
  return num >> 2;
}

/* Returns ROUND(num / 2^3) */
uint64_t shiftround_u64_3(const uint64_t num) {
  if (num & 0x0000000000000004ull) return (num >> 3) + 1ull;
  return num >> 3;
}

/* Returns ROUND(num / 2^4) */
uint64_t shiftround_u64_4(const uint64_t num) {
  if (num & 0x0000000000000008ull) return (num >> 4) + 1ull;
  return num >> 4;
}

/* Returns ROUND(num / 2^5) */
uint64_t shiftround_u64_5(const uint64_t num) {
  if (num & 0x0000000000000010ull) return (num >> 5) + 1ull;
  return num >> 5;
}

/* Returns ROUND(num / 2^6) */
uint64_t shiftround_u64_6(const uint64_t num) {
  if (num & 0x0000000000000020ull) return (num >> 6) + 1ull;
  return num >> 6;
}

/* Returns ROUND(num / 2^7) */
uint64_t shiftround_u64_7(const uint64_t num) {
  if (num & 0x0000000000000040ull) return (num >> 7) + 1ull;
  return num >> 7;
}

/* Returns ROUND(num / 2^8) */
uint64_t shiftround_u64_8(const uint64_t num) {
  if (num & 0x0000000000000080ull) return (num >> 8) + 1ull;
  return num >> 8;
}

/* Returns ROUND(num / 2^9) */
uint64_t shiftround_u64_9(const uint64_t num) {
  if (num & 0x0000000000000100ull) return (num >> 9) + 1ull;
  return num >> 9;
}

/* Returns ROUND(num / 2^10) */
uint64_t shiftround_u64_10(const uint64_t num) {
  if (num & 0x0000000000000200ull) return (num >> 10) + 1ull;
  return num >> 10;
}

/* Returns ROUND(num / 2^11) */
uint64_t shiftround_u64_11(const uint64_t num) {
  if (num & 0x0000000000000400ull) return (num >> 11) + 1ull;
  return num >> 11;
}

/* Returns ROUND(num / 2^12) */
uint64_t shiftround_u64_12(const uint64_t num) {
  if (num & 0x0000000000000800ull) return (num >> 12) + 1ull;
  return num >> 12;
}

/* Returns ROUND(num / 2^13) */
uint64_t shiftround_u64_13(const uint64_t num) {
  if (num & 0x0000000000001000ull) return (num >> 13) + 1ull;
  return num >> 13;
}

/* Returns ROUND(num / 2^14) */
uint64_t shiftround_u64_14(const uint64_t num) {
  if (num & 0x0000000000002000ull) return (num >> 14) + 1ull;
  return num >> 14;
}

/* Returns ROUND(num / 2^15) */
uint64_t shiftround_u64_15(const uint64_t num) {
  if (num & 0x0000000000004000ull) return (num >> 15) + 1ull;
  return num >> 15;
}

/* Returns ROUND(num / 2^16) */
uint64_t shiftround_u64_16(const uint64_t num) {
  if (num & 0x0000000000008000ull) return (num >> 16) + 1ull;
  return num >> 16;
}

/* Returns ROUND(num / 2^17) */
uint64_t shiftround_u64_17(const uint64_t num) {
  if (num & 0x0000000000010000ull) return (num >> 17) + 1ull;
  return num >> 17;
}

/* Returns ROUND(num / 2^18) */
uint64_t shiftround_u64_18(const uint64_t num) {
  if (num & 0x0000000000020000ull) return (num >> 18) + 1ull;
  return num >> 18;
}

/* Returns ROUND(num / 2^19) */
uint64_t shiftround_u64_19(const uint64_t num) {
  if (num & 0x0000000000040000ull) return (num >> 19) + 1ull;
  return num >> 19;
}

/* Returns ROUND(num / 2^20) */
uint64_t shiftround_u64_20(const uint64_t num) {
  if (num & 0x0000000000080000ull) return (num >> 20) + 1ull;
  return num >> 20;
}

/* Returns ROUND(num / 2^21) */
uint64_t shiftround_u64_21(const uint64_t num) {
  if (num & 0x0000000000100000ull) return (num >> 21) + 1ull;
  return num >> 21;
}

/* Returns ROUND(num / 2^22) */
uint64_t shiftround_u64_22(const uint64_t num) {
  if (num & 0x0000000000200000ull) return (num >> 22) + 1ull;
  return num >> 22;
}

/* Returns ROUND(num / 2^23) */
uint64_t shiftround_u64_23(const uint64_t num) {
  if (num & 0x0000000000400000ull) return (num >> 23) + 1ull;
  return num >> 23;
}

/* Returns ROUND(num / 2^24) */
uint64_t shiftround_u64_24(const uint64_t num) {
  if (num & 0x0000000000800000ull) return (num >> 24) + 1ull;
  return num >> 24;
}

/* Returns ROUND(num / 2^25) */
uint64_t shiftround_u64_25(const uint64_t num) {
  if (num & 0x0000000001000000ull) return (num >> 25) + 1ull;
  return num >> 25;
}

/* Returns ROUND(num / 2^26) */
uint64_t shiftround_u64_26(const uint64_t num) {
  if (num & 0x0000000002000000ull) return (num >> 26) + 1ull;
  return num >> 26;
}

/* Returns ROUND(num / 2^27) */
uint64_t shiftround_u64_27(const uint64_t num) {
  if (num & 0x0000000004000000ull) return (num >> 27) + 1ull;
  return num >> 27;
}

/* Returns ROUND(num / 2^28) */
uint64_t shiftround_u64_28(const uint64_t num) {
  if (num & 0x0000000008000000ull) return (num >> 28) + 1ull;
  return num >> 28;
}

/* Returns ROUND(num / 2^29) */
uint64_t shiftround_u64_29(const uint64_t num) {
  if (num & 0x0000000010000000ull) return (num >> 29) + 1ull;
  return num >> 29;
}

/* Returns ROUND(num / 2^30) */
uint64_t shiftround_u64_30(const uint64_t num) {
  if (num & 0x0000000020000000ull) return (num >> 30) + 1ull;
  return num >> 30;
}

/* Returns ROUND(num / 2^31) */
uint64_t shiftround_u64_31(const uint64_t num) {
  if (num & 0x0000000040000000ull) return (num >> 31) + 1ull;
  return num >> 31;
}

/* Returns ROUND(num / 2^32) */
uint64_t shiftround_u64_32(const uint64_t num) {
  if (num & 0x0000000080000000ull) return (num >> 32) + 1ull;
  return num >> 32;
}

/* Returns ROUND(num / 2^33) */
uint64_t shiftround_u64_33(const uint64_t num) {
  if (num & 0x0000000100000000ull) return (num >> 33) + 1ull;
  return num >> 33;
}

/* Returns ROUND(num / 2^34) */
uint64_t shiftround_u64_34(const uint64_t num) {
  if (num & 0x0000000200000000ull) return (num >> 34) + 1ull;
  return num >> 34;
}

/* Returns ROUND(num / 2^35) */
uint64_t shiftround_u64_35(const uint64_t num) {
  if (num & 0x0000000400000000ull) return (num >> 35) + 1ull;
  return num >> 35;
}

/* Returns ROUND(num / 2^36) */
uint64_t shiftround_u64_36(const uint64_t num) {
  if (num & 0x0000000800000000ull) return (num >> 36) + 1ull;
  return num >> 36;
}

/* Returns ROUND(num / 2^37) */
uint64_t shiftround_u64_37(const uint64_t num) {
  if (num & 0x0000001000000000ull) return (num >> 37) + 1ull;
  return num >> 37;
}

/* Returns ROUND(num / 2^38) */
uint64_t shiftround_u64_38(const uint64_t num) {
  if (num & 0x0000002000000000ull) return (num >> 38) + 1ull;
  return num >> 38;
}

/* Returns ROUND(num / 2^39) */
uint64_t shiftround_u64_39(const uint64_t num) {
  if (num & 0x0000004000000000ull) return (num >> 39) + 1ull;
  return num >> 39;
}

/* Returns ROUND(num / 2^40) */
uint64_t shiftround_u64_40(const uint64_t num) {
  if (num & 0x0000008000000000ull) return (num >> 40) + 1ull;
  return num >> 40;
}

/* Returns ROUND(num / 2^41) */
uint64_t shiftround_u64_41(const uint64_t num) {
  if (num & 0x0000010000000000ull) return (num >> 41) + 1ull;
  return num >> 41;
}

/* Returns ROUND(num / 2^42) */
uint64_t shiftround_u64_42(const uint64_t num) {
  if (num & 0x0000020000000000ull) return (num >> 42) + 1ull;
  return num >> 42;
}

/* Returns ROUND(num / 2^43) */
uint64_t shiftround_u64_43(const uint64_t num) {
  if (num & 0x0000040000000000ull) return (num >> 43) + 1ull;
  return num >> 43;
}

/* Returns ROUND(num / 2^44) */
uint64_t shiftround_u64_44(const uint64_t num) {
  if (num & 0x0000080000000000ull) return (num >> 44) + 1ull;
  return num >> 44;
}

/* Returns ROUND(num / 2^45) */
uint64_t shiftround_u64_45(const uint64_t num) {
  if (num & 0x0000100000000000ull) return (num >> 45) + 1ull;
  return num >> 45;
}

/* Returns ROUND(num / 2^46) */
uint64_t shiftround_u64_46(const uint64_t num) {
  if (num & 0x0000200000000000ull) return (num >> 46) + 1ull;
  return num >> 46;
}

/* Returns ROUND(num / 2^47) */
uint64_t shiftround_u64_47(const uint64_t num) {
  if (num & 0x0000400000000000ull) return (num >> 47) + 1ull;
  return num >> 47;
}

/* Returns ROUND(num / 2^48) */
uint64_t shiftround_u64_48(const uint64_t num) {
  if (num & 0x0000800000000000ull) return (num >> 48) + 1ull;
  return num >> 48;
}

/* Returns ROUND(num / 2^49) */
uint64_t shiftround_u64_49(const uint64_t num) {
  if (num & 0x0001000000000000ull) return (num >> 49) + 1ull;
  return num >> 49;
}

/* Returns ROUND(num / 2^50) */
uint64_t shiftround_u64_50(const uint64_t num) {
  if (num & 0x0002000000000000ull) return (num >> 50) + 1ull;
  return num >> 50;
}

/* Returns ROUND(num / 2^51) */
uint64_t shiftround_u64_51(const uint64_t num) {
  if (num & 0x0004000000000000ull) return (num >> 51) + 1ull;
  return num >> 51;
}

/* Returns ROUND(num / 2^52) */
uint64_t shiftround_u64_52(const uint64_t num) {
  if (num & 0x0008000000000000ull) return (num >> 52) + 1ull;
  return num >> 52;
}

/* Returns ROUND(num / 2^53) */
uint64_t shiftround_u64_53(const uint64_t num) {
  if (num & 0x0010000000000000ull) return (num >> 53) + 1ull;
  return num >> 53;
}

/* Returns ROUND(num / 2^54) */
uint64_t shiftround_u64_54(const uint64_t num) {
  if (num & 0x0020000000000000ull) return (num >> 54) + 1ull;
  return num >> 54;
}

/* Returns ROUND(num / 2^55) */
uint64_t shiftround_u64_55(const uint64_t num) {
  if (num & 0x0040000000000000ull) return (num >> 55) + 1ull;
  return num >> 55;
}

/* Returns ROUND(num / 2^56) */
uint64_t shiftround_u64_56(const uint64_t num) {
  if (num & 0x0080000000000000ull) return (num >> 56) + 1ull;
  return num >> 56;
}

/* Returns ROUND(num / 2^57) */
uint64_t shiftround_u64_57(const uint64_t num) {
  if (num & 0x0100000000000000ull) return (num >> 57) + 1ull;
  return num >> 57;
}

/* Returns ROUND(num / 2^58) */
uint64_t shiftround_u64_58(const uint64_t num) {
  if (num & 0x0200000000000000ull) return (num >> 58) + 1ull;
  return num >> 58;
}

/* Returns ROUND(num / 2^59) */
uint64_t shiftround_u64_59(const uint64_t num) {
  if (num & 0x0400000000000000ull) return (num >> 59) + 1ull;
  return num >> 59;
}

/* Returns ROUND(num / 2^60) */
uint64_t shiftround_u64_60(const uint64_t num) {
  if (num & 0x0800000000000000ull) return (num >> 60) + 1ull;
  return num >> 60;
}

/* Returns ROUND(num / 2^61) */
uint64_t shiftround_u64_61(const uint64_t num) {
  if (num & 0x1000000000000000ull) return (num >> 61) + 1ull;
  return num >> 61;
}

/* Returns ROUND(num / 2^62) */
uint64_t shiftround_u64_62(const uint64_t num) {
  if (num & 0x2000000000000000ull) return (num >> 62) + 1ull;
  return num >> 62;
}

/* Returns ROUND(num / 2^63) */
uint64_t shiftround_u64_63(const uint64_t num) {
  if (num & 0x4000000000000000ull) return (num >> 63) + 1ull;
  return num >> 63;
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