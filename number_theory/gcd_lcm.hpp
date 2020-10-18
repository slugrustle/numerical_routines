/**
 * gcd_lcm.hpp
 * Implementations of the greatest common divisor and least common multiple
 * algorithms with specializations for uint8_t, uint16_t, uint32_t, and
 * uint64_t types.
 * 
 * The lcm() functions throw an std::overflow_error if the result would
 * overflow the range of the input/output type.
 * 
 * Two-argument and multiple-argument (>= 2, via initializer list or iterators)
 * versions of both gcd() and lcm() are defined for all numeric types listed
 * above.
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
#ifndef GCD_LCM_HPP_
#define GCD_LCM_HPP_

#include <cinttypes>
#include <limits>
#include <exception>
#include <stdexcept>

extern "C"
{
  #include "detect_product_overflow.h"
}

/* Allows static_assert message in multshiftround primary template to compile. */
template <typename type> static bool always_false_gcd_lcm(void) { return false; }

/**
 * This two-argument gcd primary template is a catch-all for invalid
 * and/or presently unimplemented unsigned integer types.
 */
template <typename type> 
typename std::enable_if<std::is_unsigned<type>::value, type>::type gcd(type a, type b) {
  static_assert(always_false_gcd_lcm<type>(), "type gcd<type>(type a, type b) is not defined for the specified type.");
}

/**
 * Specialization of the greatest common divisor algorithm
 * for uint8_t.
 */
template <> inline uint8_t gcd<uint8_t>(uint8_t a, uint8_t b) {
  uint8_t tmp;
  
  if (b > a) {
    tmp = a;
    a = b;
    b = tmp;
  }

  while (b != static_cast<uint8_t>(0)) {
    tmp = b;
    b = a % b;
    a = tmp;
  }

  return a;
}

/**
 * Specialization of the greatest common divisor algorithm
 * for uint16_t.
 */
template <> inline uint16_t gcd<uint16_t>(uint16_t a, uint16_t b) {
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
template <> inline uint32_t gcd<uint32_t>(uint32_t a, uint32_t b) {
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
template <> inline uint64_t gcd<uint64_t>(uint64_t a, uint64_t b) {
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
 * This initializer_list-based multi-argument gcd primary template
 * is a catch-all for invalid and/or presently unimplemented unsigned
 * integer types.
 * 
 * This form accepts multiple (>= 2) arguments via
 * initializer list.
 * 
 * Example: uint32_t out = gcd<uint32_t>({10u, 90u, 3u});
 * 
 * This form is only valid for input types for which a two-argument gcd
 * function is defined above.
 */
template <typename type>
typename std::enable_if<std::is_unsigned<type>::value, type>::type gcd(const std::initializer_list<type> &i_l) {
  static_assert(always_false_gcd_lcm<type>(), "type gcd<type>(initializer_list<type> &i_l) is not defined for the specified type.");
}

/**
 * Specialization of multiple-input gcd for uint8_t.
 */
template <> inline uint8_t gcd<uint8_t>(const std::initializer_list<uint8_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("gcd<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint8_t>::const_iterator itr = i_l.begin();
  uint8_t running_output = gcd<uint8_t>(*itr, *(itr+1));
  itr++;

  while ((++itr) != i_l.end()) running_output = gcd<uint8_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input gcd for uint16_t.
 */
template <> inline uint16_t gcd<uint16_t>(const std::initializer_list<uint16_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("gcd<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint16_t>::const_iterator itr = i_l.begin();
  uint16_t running_output = gcd<uint16_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = gcd<uint16_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input gcd for uint32_t.
 */
template <> inline uint32_t gcd<uint32_t>(const std::initializer_list<uint32_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("gcd<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint32_t>::const_iterator itr = i_l.begin();
  uint32_t running_output = gcd<uint32_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = gcd<uint32_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input gcd for uint64_t.
 */
template <> inline uint64_t gcd<uint64_t>(const std::initializer_list<uint64_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("gcd<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint64_t>::const_iterator itr = i_l.begin();
  uint64_t running_output = gcd<uint64_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = gcd<uint64_t>(running_output, *itr);

  return running_output;
}

/**
 * Iterator-based multi-argument gcd function.
 * 
 * This form accepts multiple (>= 2) arguments via any container
 * with at least forward iterators.
 * 
 * Example: 
 * std::vector<uint32_t> v{10, 90, 3};
 * uint32_t out = gcd(v.cbegin(), v.cend());
 * 
 * This form is only valid for input types for which a two-argument gcd
 * function is defined above.
 */
template <class Iterator>
typename std::enable_if<std::is_unsigned<typename std::iterator_traits<Iterator>::value_type>::value, typename std::iterator_traits<Iterator>::value_type>::type 
gcd(const Iterator &first, const Iterator &last) {
  static_assert(std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value,
    "gcd(Iterator first, Iterator last) requires forward iterator (or more capable iterator) arguments.");
  
  if ((last - first) < 2) throw std::invalid_argument("gcd(Iterator first, Iterator last) requires at least two input values.");

  Iterator itr = first;
  typename std::iterator_traits<Iterator>::value_type running_output = gcd<typename std::iterator_traits<Iterator>::value_type>(*itr, *(itr+1));
  itr++;

  while (++itr != last) running_output = gcd<typename std::iterator_traits<Iterator>::value_type>(running_output, *itr);

  return running_output;
}

/**
 * The lcm primary template is a catch-all for invalid
 * and/or presently unimplemented unsigned integer types.
 */
template <typename type>
typename std::enable_if<std::is_unsigned<type>::value, type>::type lcm(type a, type b) {
  static_assert(always_false_gcd_lcm<type>(), "type lcm<type>(type a, type b) is not defined for the specified type.");
}

/**
 * Specialization of the least common multiple algorithm
 * for uint8_t. Throws an exception if the result would
 * exceed the range of a uint8_t.
 */
template <> inline uint8_t lcm<uint8_t>(uint8_t a, uint8_t b) {
  if ((a == static_cast<uint8_t>(0)) | (b == static_cast<uint8_t>(0))) return static_cast<uint8_t>(0);

  uint8_t gcd_a_b = gcd<uint8_t>(a, b);
  if (a > b) a = a / gcd_a_b;
  else b = b / gcd_a_b;

  uint16_t lcm = static_cast<uint16_t>(a) * static_cast<uint16_t>(b);
  if (lcm > static_cast<uint16_t>(std::numeric_limits<uint8_t>::max())) throw std::overflow_error("overflow in lcm<uint8_t>()");
  
  return static_cast<uint8_t>(lcm);
}

/**
 * Specialization of the least common multiple algorithm
 * for uint16_t. Throws an exception if the result would
 * exceed the range of a uint16_t.
 */
template <> inline uint16_t lcm<uint16_t>(uint16_t a, uint16_t b) {
  if ((a == static_cast<uint16_t>(0)) | (b == static_cast<uint16_t>(0))) return static_cast<uint16_t>(0);

  uint16_t gcd_a_b = gcd<uint16_t>(a, b);
  if (a > b) a = a / gcd_a_b;
  else b = b / gcd_a_b;

  uint32_t lcm = static_cast<uint32_t>(a) * static_cast<uint32_t>(b);
  if (lcm > static_cast<uint32_t>(std::numeric_limits<uint16_t>::max())) throw std::overflow_error("overflow in lcm<uint16_t>()");
  
  return static_cast<uint16_t>(lcm);
}

/**
 * Specialization of the least common multiple algorithm
 * for uint32_t. Throws an exception if the result would
 * exceed the range of a uint32_t.
 */
template <> inline uint32_t lcm<uint32_t>(uint32_t a, uint32_t b) {
  if ((a == 0u) | (b == 0u)) return 0u;

  uint32_t gcd_a_b = gcd<uint32_t>(a, b);
  if (a > b) a = a / gcd_a_b;
  else b = b / gcd_a_b;

  uint64_t lcm = static_cast<uint64_t>(a) * static_cast<uint64_t>(b);
  if (lcm > static_cast<uint64_t>(std::numeric_limits<uint32_t>::max())) throw std::overflow_error("overflow in lcm<uint32_t>()");
  
  return static_cast<uint32_t>(lcm);
}

/**
 * Specialization of the least common multiple algorithm
 * for uint64_t. Throws an exception if the result would
 * exceed the range of a uint64_t.
 * 
 * Requires detect_product_overflow_u64 from
 * detect_product_overflow.c / detect_product_overflow.h.
 */
template <> inline uint64_t lcm<uint64_t>(uint64_t a, uint64_t b) {
  if ((a == 0ull) | (b == 0ull)) return 0ull;

  uint64_t gcd_a_b = gcd<uint64_t>(a, b);
  if (a > b) a = a / gcd_a_b;
  else b = b / gcd_a_b;

  if (detect_product_overflow_u64(a, b)) throw std::overflow_error("overflow in lcm<uint64_t>()");

  return a * b;
}

/**
 * This initializer_list-based multi-argument lcm primary template
 * is a catch-all for invalid and/or presently unimplemented 
 * unsigned integer types.
 * 
 * This form accepts multiple (>= 2) arguments via
 * initializer list.
 * 
 * Example: uint32_t out = lcm<uint32_t>({200u, 15u, 39u, 80u});
 * 
 * This form is only valid for input types for which a two-argument lcm
 * function is defined above.
 * 
 * It throws an exception if the result would exceed the range
 * of the input argument type.
 */
template <typename type>
typename std::enable_if<std::is_unsigned<type>::value, type>::type lcm(const std::initializer_list<type> &i_l) {
  static_assert(always_false_gcd_lcm<type>(), "type lcm<type>(initializer_list<type> &i_l) is not defined for the specified type.");
}

/**
 * Specialization of multiple-input lcm for uint8_t.
 */
template <> inline uint8_t lcm<uint8_t>(const std::initializer_list<uint8_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("lcm<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint8_t>::const_iterator itr = i_l.begin();
  uint8_t running_output = lcm<uint8_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = lcm<uint8_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input lcm for uint16_t.
 */
template <> inline uint16_t lcm<uint16_t>(const std::initializer_list<uint16_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("lcm<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint16_t>::const_iterator itr = i_l.begin();
  uint16_t running_output = lcm<uint16_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = lcm<uint16_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input lcm for uint32_t.
 */
template <> inline uint32_t lcm<uint32_t>(const std::initializer_list<uint32_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("lcm<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint32_t>::const_iterator itr = i_l.begin();
  uint32_t running_output = lcm<uint32_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = lcm<uint32_t>(running_output, *itr);

  return running_output;
}

/**
 * Specialization of multiple-input lcm for uint64_t.
 */
template <> inline uint64_t lcm<uint64_t>(const std::initializer_list<uint64_t> &i_l) {
  if (i_l.size() < 2) throw std::invalid_argument("lcm<type>(initializer_list<type> &i_l) requires at least two input values.");

  typename std::initializer_list<uint64_t>::const_iterator itr = i_l.begin();
  uint64_t running_output = lcm<uint64_t>(*itr, *(itr+1));
  itr++;

  while (++itr != i_l.end()) running_output = lcm<uint64_t>(running_output, *itr);

  return running_output;
}

/**
 * Iterator-based multi-argument lcm function.
 * 
 * This form accepts multiple (>= 2) arguments via any container
 * with at least forward iterators.
 * 
 * Example: 
 * std::vector<uint32_t> v{200, 15, 39, 80};
 * uint32_t out = lcm(v.cbegin(), v.cend());
 * 
 * This form is only valid for input types for which a two-argument lcm
 * function is defined above.
 * 
 * It throws an exception if the result would exceed the range
 * of the input argument type.
 */
template <class Iterator>
typename std::enable_if<std::is_unsigned<typename std::iterator_traits<Iterator>::value_type>::value, typename std::iterator_traits<Iterator>::value_type>::type 
lcm(const Iterator &first, const Iterator &last) {
  static_assert(std::is_convertible<typename std::iterator_traits<Iterator>::iterator_category, std::forward_iterator_tag>::value,
    "lcm(Iterator first, Iterator last) requires forward iterator (or more capable iterator) arguments.");
  
  if ((last - first) < 2) throw std::invalid_argument("lcm(Iterator first, Iterator last) requires at least two input values.");

  Iterator itr = first;
  typename std::iterator_traits<Iterator>::value_type running_output = lcm<typename std::iterator_traits<Iterator>::value_type>(*itr, *(itr+1));
  itr++;

  while (++itr != last) running_output = lcm<typename std::iterator_traits<Iterator>::value_type>(running_output, *itr);

  return running_output;
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
