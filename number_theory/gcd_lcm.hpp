/**
 * gcd_lcm.hpp
 * Implementations of the greatest common divisor and least common multiple
 * algorithms with specializations for uint16_t, uint32_t, and uint64_t
 * types.
 * 
 * The lcm() functions throw an std::overflow_error if the result would
 * overflow the range of the input/output type.
 * 
 * Written in 2019 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */
#ifndef GCD_LCM_HPP_
#define GCD_LCM_HPP_

#include <cinttypes>
#include <limits>
#include <exception>
#include <stdexcept>

/* Allows static_assert message in multshiftround primary template to compile. */
template <typename type> static bool always_false_gcd_lcm(void) { return false; }

/**
 * The gcd primary template is a catch-all for invalid
 * and/or presently unimplemented template arguments.
 */
template <typename type> type gcd(type a, type b) {
  static_assert(always_false_gcd_lcm<type>(), "type gcd<type>(type a, type b) is not defined for the specified type.");
}

/**
 * Specialization of the greatest common divisor algorithm
 * for uint16_t.
 */
template <> uint16_t gcd<uint16_t>(uint16_t a, uint16_t b) {
  uint16_t tmp;
  
  if (b > a) {
    tmp = a;
    a = b;
    b = tmp;
  }

  while (b != static_cast<uint16_t>(0)) {
    tmp = b;
    b = a % b;
    a = tmp;
  }

  return a;
}

/**
 * Specialization of the greatest common divisor algorithm
 * for uint32_t.
 */
template <> uint32_t gcd<uint32_t>(uint32_t a, uint32_t b) {
  uint32_t tmp;
  
  if (b > a) {
    tmp = a;
    a = b;
    b = tmp;
  }

  while (b != 0u) {
    tmp = b;
    b = a % b;
    a = tmp;
  }

  return a;
}

/**
 * Specialization of the greatest common divisor algorithm
 * for uint64_t.
 */
template <> uint64_t gcd<uint64_t>(uint64_t a, uint64_t b) {
  uint64_t tmp;
  
  if (b > a) {
    tmp = a;
    a = b;
    b = tmp;
  }

  while (b != 0ull) {
    tmp = b;
    b = a % b;
    a = tmp;
  }

  return a;
}

/**
 * The lcm primary template is a catch-all for invalid
 * and/or presently unimplemented template arguments.
 */
template <typename type> type lcm(type a, type b) {
  static_assert(always_false_gcd_lcm<type>(), "type lcm<type>(type a, type b) is not defined for the specified type.");
}

/**
 * Specialization of the least common multiple algorithm
 * for uint16_t. Throws an exception if the result would
 * exceed the range of a uint16_t.
 */
template <> uint16_t lcm<uint16_t>(uint16_t a, uint16_t b) {
  if ((a == static_cast<uint16_t>(0)) | (b == static_cast<uint16_t>(0))) return static_cast<uint16_t>(0);

  uint16_t gcd_a_b = gcd<uint16_t>(a, b);
  
  if (a > b) {
    uint16_t factor = a / gcd_a_b;
    if (static_cast<uint32_t>(factor) * static_cast<uint32_t>(b) > std::numeric_limits<uint16_t>::max())
      throw std::overflow_error("overflow in lcm<uint16_t>()");

    return factor * b;
  } else {
    uint16_t factor = b / gcd_a_b;
    if (static_cast<uint32_t>(factor) * static_cast<uint32_t>(a) > std::numeric_limits<uint16_t>::max())
      throw std::overflow_error("overflow in lcm<uint16_t>()");

    return factor * a;
  }
}

/**
 * Specialization of the least common multiple algorithm
 * for uint32_t. Throws an exception if the result would
 * exceed the range of a uint32_t.
 */
template <> uint32_t lcm<uint32_t>(uint32_t a, uint32_t b) {
  if ((a == 0u) | (b == 0u)) return 0u;

  uint32_t gcd_a_b = gcd<uint32_t>(a, b);
  
  if (a > b) {
    uint32_t factor = a / gcd_a_b;
    if (static_cast<uint64_t>(factor) * static_cast<uint64_t>(b) > std::numeric_limits<uint32_t>::max())
      throw std::overflow_error("overflow in lcm<uint32_t>()");

    return factor * b;
  } else {
    uint32_t factor = b / gcd_a_b;
    if (static_cast<uint64_t>(factor) * static_cast<uint64_t>(a) > std::numeric_limits<uint32_t>::max())
      throw std::overflow_error("overflow in lcm<uint32_t>()");

    return factor * a;
  }
}

/**
 * Returns true if the product a * b would overflow the range
 * of a uin64_t and false otherwise.
 */
static bool detect_product_overflow_u64(const uint64_t a, const uint64_t b) {
  /**
   * a * b = 2^64 * (a_hi * b_hi) + 
   *         2^32 * (a_hi * b_lo + a_lo * b_hi) +
   *         a_lo * b_lo
   * All of the products a_hi*b_hi, a_hi*b_lo,
   * a_lo*b_hi, and a_lo*b_lo will be on the range
   * [0,2^64-2^33+1].
   */
  uint64_t a_hi = (a & 0xFFFFFFFF00000000ull) >> 32;
  uint64_t a_lo =  a & 0x00000000FFFFFFFFull;
  uint64_t b_hi = (b & 0xFFFFFFFF00000000ull) >> 32;
  uint64_t b_lo =  b & 0x00000000FFFFFFFFull;

  if (a_hi * b_hi > 0ull) return true;
  /**
   * Now we know that a_hi * b_hi == 0.
   * Consequently,
   * a * b = 2^32 * (a_hi * b_lo + a_lo * b_hi) +
   *         a_lo * b_lo
   */

  uint64_t mid_prod_1 = a_hi * b_lo;
  uint64_t mid_prod_2 = a_lo * b_hi;

  if (std::numeric_limits<uint64_t>::max() - mid_prod_2 < mid_prod_1) return true;
  /* Now we know that mid_prod_1 + mid_prod_2 <= std::numeric_limits<uint64_t>::max(). */
  uint64_t mid_prod = mid_prod_1 + mid_prod_2;
  
  if (mid_prod >= (1ull << 32)) return true;
  /* Now we know that 2^32 * mid_prod < std::numeric_limits<uint64_t>::max(). */
  if (std::numeric_limits<uint64_t>::max() - a_lo * b_lo < mid_prod << 32) return true;

  return false;
}

/**
 * Specialization of the least common multiple algorithm
 * for uint64_t. Throws an exception if the result would
 * exceed the range of a uint64_t.
 */
template <> uint64_t lcm<uint64_t>(uint64_t a, uint64_t b) {
  if ((a == 0ull) | (b == 0ull)) return 0ull;

  uint64_t gcd_a_b = gcd<uint64_t>(a, b);
  
  if (a > b) {
    uint64_t factor = a / gcd_a_b;
    if (detect_product_overflow_u64(factor, b))
      throw std::overflow_error("overflow in lcm<uint64_t>()");

    return factor * b;
  } else {
    uint32_t factor = b / gcd_a_b;
    if (detect_product_overflow_u64(factor, a))
      throw std::overflow_error("overflow in lcm<uint64_t>()");

    return factor * a;
  }
}

#endif /* #ifndef GCD_LCM_HPP_ */

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
