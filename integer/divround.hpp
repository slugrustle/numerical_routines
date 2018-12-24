/**
 * divround.hpp
 * Specifies the templated function
 *     type divround<typename type>(const type dividend, const type divisor);
 * which returns the value ROUND(dividend / divisor).
 *
 * type may be int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t,
 * uint64_t, or any type equivalent to these.
 *
 * divisor must not be 0.
 *
 * For signed types, the most negative number must not be divided by -1 in
 * order to avoid overflow. Explicitly, the following must be avoided:
 * int8_t:  -128 / -1                   or   -2^7 / -1
 * int16_t: -32768 / -1                 or   -2^15 / -1
 * int32_t: -2147483648 / -1            or   -2^31 / -1
 * int64_t: -9223372036854775808 / -1   or   -2^63 / -1
 * This assumes a 2's complement representation of signed integers.
 *
 * Correct rounding for negative signed divisor arguments requires two things:
 * 1. The representation of signed integers must be 2's complement.
 * 2. The compiler must encode right shifts on signed types as arithmetic
 *    right shifts rather than logical right shifts.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file.If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef DIVROUND_HPP_
#define DIVROUND_HPP_

#include <cinttypes>

/**
 * This divround primary template is a catch-all for presently unimplemented
 * template arguments.
 */
template <typename type> type divround(const type dividend, const type divisor) {
  static_assert(false, "type multshiftround(const type dividend, const type divisor); is not defined for the specified type.");
}

/********************************************************************************
 ********           int8_t and uint8_t template specializations          ********
 ********************************************************************************/

/** 
 * Returns ROUND(dividend / divisor). divisor must never be 0. 
 * divisor must not be -1 when dividend is -127 (-2^7).
 */
template <> int8_t divround<int8_t>(const int8_t dividend, const int8_t divisor) {
  int8_t quotient = dividend / divisor;
  int8_t remainder = dividend - (quotient * divisor);
  int8_t div_half = divisor >> 1;
  if ((divisor & static_cast<uint8_t>(0x81)) == static_cast<int8_t>(0x01)) div_half++;
  if (remainder < static_cast<int8_t>(0)) {
    if (div_half < static_cast<int8_t>(0)) {
      if (remainder <= div_half) return quotient + static_cast<int8_t>(1);
      return quotient;
    }
    else {
      if (-remainder >= div_half) return quotient - static_cast<int8_t>(1);
      return quotient;
    }
  }
  else {
    if (div_half >= static_cast<int8_t>(0)) {
      if (remainder >= div_half) return quotient + static_cast<int8_t>(1);
      return quotient;
    }
    else {
      if (-remainder <= div_half) return quotient - static_cast<int8_t>(1);
      return quotient;
    }
  }
}

/* Returns ROUND(dividend / divisor). divisor must not be 0. */
template <> uint8_t divround<uint8_t>(const uint8_t dividend, const uint8_t divisor) {
  uint8_t quotient = dividend / divisor;
  uint8_t remainder = dividend - (quotient * divisor);
  uint8_t div_half = divisor >> 1;
  if (divisor & static_cast<uint8_t>(0x01)) div_half++;
  if (remainder >= div_half) return quotient + static_cast<uint8_t>(1);
  return quotient;
}

/********************************************************************************
 ********          int16_t and uint16_t template specializations         ********
 ********************************************************************************/

/**
 * Returns ROUND(dividend / divisor). divisor must never be 0.
 * divisor must not be -1 when dividend is -32768 (-2^15).
 */
template <> int16_t divround<int16_t>(const int16_t dividend, const int16_t divisor) {
  int16_t quotient = dividend / divisor;
  int16_t remainder = dividend - (quotient * divisor);
  int16_t div_half = divisor >> 1;
  if ((divisor & static_cast<uint16_t>(0x8001)) == static_cast<int16_t>(0x0001)) div_half++;
  if (remainder < static_cast<int16_t>(0)) {
    if (div_half < static_cast<int16_t>(0)) {
      if (remainder <= div_half) return quotient + static_cast<int16_t>(1);
      return quotient;
    }
    else {
      if (-remainder >= div_half) return quotient - static_cast<int16_t>(1);
      return quotient;
    }
  }
  else {
    if (div_half >= static_cast<int16_t>(0)) {
      if (remainder >= div_half) return quotient + static_cast<int16_t>(1);
      return quotient;
    }
    else {
      if (-remainder <= div_half) return quotient - static_cast<int16_t>(1);
      return quotient;
    }
  }
}

/* Returns ROUND(dividend / divisor). divisor must not be 0. */
template <> uint16_t divround<uint16_t>(const uint16_t dividend, const uint16_t divisor) {
  uint16_t quotient = dividend / divisor;
  uint16_t remainder = dividend - (quotient * divisor);
  uint16_t div_half = divisor >> 1;
  if (divisor & static_cast<uint16_t>(0x0001)) div_half++;
  if (remainder >= div_half) return quotient + static_cast<uint16_t>(1);
  return quotient;
}

/********************************************************************************
 ********          int32_t and uint32_t template specializations         ********
 ********************************************************************************/

/**
 * Returns ROUND(dividend / divisor). divisor must never be 0. 
 * divisor must not be -1 when dividend is -2147483648 (-2^31).
 */
template <> int32_t divround<int32_t>(const int32_t dividend, const int32_t divisor) {
  int32_t quotient = dividend / divisor;
  int32_t remainder = dividend - (quotient * divisor);
  int32_t div_half = divisor >> 1;
  if ((divisor & 0x80000001u) == 0x00000001) div_half++;
  if (remainder < 0) {
    if (div_half < 0) {
      if (remainder <= div_half) return quotient + 1;
      return quotient;
    }
    else {
      if (-remainder >= div_half) return quotient - 1;
      return quotient;
    }
  }
  else {
    if (div_half >= 0) {
      if (remainder >= div_half) return quotient + 1;
      return quotient;
    }
    else {
      if (-remainder <= div_half) return quotient - 1;
      return quotient;
    }
  }
}

/* Returns ROUND(dividend / divisor). divisor must not be 0. */
template <> uint32_t divround<uint32_t>(const uint32_t dividend, const uint32_t divisor) {
  uint32_t quotient = dividend / divisor;
  uint32_t remainder = dividend - (quotient * divisor);
  uint32_t div_half = divisor >> 1;
  if (divisor & 0x00000001u) div_half++;
  if (remainder >= div_half) return quotient + 1u;
  return quotient;
}

/********************************************************************************
 ********          int64_t and uint64_t template specializations         ********
 ********************************************************************************/

/**
 * Returns ROUND(dividend / divisor). divisor must never be 0. 
 * divisor must not be -1 when dividend is -9223372036854775808 (-2^63).
 */
template <> int64_t divround<int64_t>(const int64_t dividend, const int64_t divisor) {
  int64_t quotient = dividend / divisor;
  int64_t remainder = dividend - (quotient * divisor);
  int64_t div_half = divisor >> 1;
  if ((divisor & 0x8000000000000001ull) == 0x0000000000000001ll) div_half++;
  if (remainder < 0ll) {
    if (div_half < 0ll) {
      if (remainder <= div_half) return quotient + 1ll;
      return quotient;
    }
    else {
      if (-remainder >= div_half) return quotient - 1ll;
      return quotient;
    }
  }
  else {
    if (div_half >= 0ll) {
      if (remainder >= div_half) return quotient + 1ll;
      return quotient;
    }
    else {
      if (-remainder <= div_half) return quotient - 1ll;
      return quotient;
    }
  }
}

/* Returns ROUND(dividend / divisor). divisor must not be 0. */
template <> uint64_t divround<uint64_t>(const uint64_t dividend, const uint64_t divisor) {
  uint64_t quotient = dividend / divisor;
  uint64_t remainder = dividend - (quotient * divisor);
  uint64_t div_half = divisor >> 1;
  if (divisor & 0x0000000000000001ull) div_half++;
  if (remainder >= div_half) return quotient + 1ull;
  return quotient;
}

#endif /* #ifndef DIVROUND_HPP_ */

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