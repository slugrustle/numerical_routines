/**
 * shiftround_run.hpp
 * Specifies the templated function
 *     type shiftround<typename type>(const type num, const int8_t shift);
 * which returns the value ROUND(num / 2^shift) without using the division
 * operator. The _run in shiftround_run.hpp indicates that the shift argument
 * need not be known until runtime.
 *
 * type may be int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t,
 * uint64_t, or any type equivalent to these.
 *
 * shift may range from 0 to one less than the the word length of type for unsigned
 * types. shift may range from 0 to two less than the word length of type for
 * signed types.
 *
 * Correct operation for negative signed inputs requires two things:
 * 1. The representation of signed integers must be 2's complement.
 * 2. The compiler must encode right shifts on signed types as arithmetic
 *    right shifts rather than logical right shifts.
 * 
 * If you #define DEBUG_INTMATH, checks for invalid shift arguments will be
 * enabled. This requires the availability of stderr and fprintf() on the
 * target system and is most appropriate for testing purposes.
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
#ifndef SHIFTROUND_RUN_HPP_
#define SHIFTROUND_RUN_HPP_

#include <cinttypes>
#include "run_masks_type.h"

#ifdef ARRAY_MASKS
  #ifdef __cplusplus
    extern "C"
    {
  #endif
      #include "multshiftround_shiftround_masks.h"
  #ifdef __cplusplus
    }
  #endif
#endif

#ifdef DEBUG_INTMATH
  #include <cstdio>
#endif

/* Allows static_assert message in shiftround primary template to compile. */
template <typename type> static bool always_false_shiftround_run(void) { return false; }

/**
 * This shiftround primary template is a catch-all for presently unimplemented
 * template arguments.
 */
template <typename type> type shiftround(const type num, const uint8_t shift) {
  static_assert(always_false_shiftround_run<type>(), "type shiftround(const type num, const uint8_t shift); is not defined for the specified type.");
}

/********************************************************************************
 ********           int8_t and uint8_t template specializations          ********
 ********************************************************************************/

/* Returns ROUND(num / 2^shift). shift must be on the range [0,6]. */
template <> int8_t shiftround<int8_t>(const int8_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(6))
	    std::fprintf(stderr, "ERROR: shiftround<int8_t>(%i, %u), shift = %u is invalid; it must be on the range [0,6].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(6)) return static_cast<int8_t>(0);
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    uint8_t half_remainder = masks_8bit[shift];
  #elif defined(COMPUTED_MASKS)
    uint8_t half_remainder = static_cast<uint8_t>(1) << (shift - static_cast<uint8_t>(1));
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  if ((num & half_remainder) &&
      (num >= static_cast<int8_t>(0) ||
       (num & ((half_remainder << 1) - static_cast<uint8_t>(1))) != half_remainder))
    return (num >> shift) + static_cast<int8_t>(1);

  return num >> shift;
}

/* Returns ROUND(num / 2^shift). shift must be on the range [0,7]. */
template <> uint8_t shiftround<uint8_t>(const uint8_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(7))
	    std::fprintf(stderr, "ERROR: shiftround<uint8_t>(%u, %u), shift = %u is invalid; it must be on the range [0,7].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(7)) return static_cast<uint8_t>(0);
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    if (num & masks_8bit[shift])
      return (num >> shift) + static_cast<uint8_t>(1);
  #elif defined(COMPUTED_MASKS)
    if (num & static_cast<uint8_t>(1) << (shift - static_cast<uint8_t>(1)))
      return (num >> shift) + static_cast<uint8_t>(1);
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif
  
  return num >> shift;
}

/********************************************************************************
 ********          int16_t and uint16_t template specializations         ********
 ********************************************************************************/

/* Returns ROUND(num / 2^shift). shift must be on the range [0,14]. */
template <> int16_t shiftround<int16_t>(const int16_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(14))
	    std::fprintf(stderr, "ERROR: shiftround<int16_t>(%i, %u), shift = %u is invalid; it must be on the range [0,14].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(14)) return static_cast<int16_t>(0);
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    uint16_t half_remainder = masks_16bit[shift];
  #elif defined(COMPUTED_MASKS)
    uint16_t half_remainder = static_cast<uint16_t>(1) << (shift - static_cast<uint8_t>(1));
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  if ((num & half_remainder) &&
      (num >= static_cast<int16_t>(0) ||
       (num & ((half_remainder << 1) - static_cast<uint16_t>(1))) != half_remainder))
    return (num >> shift) + static_cast<int16_t>(1);

  return num >> shift;
}

/* Returns ROUND(num / 2^shift). shift must be on the range [0,15]. */
template <> uint16_t shiftround<uint16_t>(const uint16_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(15))
	    std::fprintf(stderr, "ERROR: shiftround<uint16_t>(%u, %u), shift = %u is invalid; it must be on the range [0,15].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(15)) return static_cast<uint16_t>(0);
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    if (num & masks_16bit[shift])
      return (num >> shift) + static_cast<uint16_t>(1);
  #elif defined(COMPUTED_MASKS)
    if (num & static_cast<uint16_t>(1) << (shift - static_cast<uint8_t>(1)))
      return (num >> shift) + static_cast<uint16_t>(1);
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  return num >> shift;
}

/********************************************************************************
 ********          int32_t and uint32_t template specializations         ********
 ********************************************************************************/

/* Returns ROUND(num / 2^shift). shift must be on the range [0,30]. */
template <> int32_t shiftround<int32_t>(const int32_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(30))
	    std::fprintf(stderr, "ERROR: shiftround<int32_t>(%i, %u), shift = %u is invalid; it must be on the range [0,30].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(30)) return 0;
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    uint32_t half_remainder = masks_32bit[shift];
  #elif defined(COMPUTED_MASKS)
    uint32_t half_remainder = 1u << (shift - static_cast<uint8_t>(1));
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif
  
  if ((num & half_remainder) &&
      (num >= 0 || (num & ((half_remainder << 1) - 1u)) != half_remainder))
    return (num >> shift) + 1;

  return num >> shift;
}

/* Returns ROUND(num / 2^shift). shift must be on the range [0,31]. */
template <> uint32_t shiftround<uint32_t>(const uint32_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(31))
	    std::fprintf(stderr, "ERROR: shiftround<uint32_t>(%u, %u), shift = %u is invalid; it must be on the range [0,31].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(31)) return 0u;
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    if (num & masks_32bit[shift])
      return (num >> shift) + 1u;
  #elif defined(COMPUTED_MASKS)
    if (num & 1u << (shift - static_cast<uint8_t>(1)))
      return (num >> shift) + 1u;
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  return num >> shift;
}

/********************************************************************************
 ********          int64_t and uint64_t template specializations         ********
 ********************************************************************************/

/* Returns ROUND(num / 2^shift). shift must be on the range [0,62]. */
template <> int64_t shiftround<int64_t>(const int64_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(62))
	    std::fprintf(stderr, "ERROR: shiftround<int64_t>(%" PRIi64 ", %u), shift = %u is invalid; it must be on the range [0,62].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(62)) return 0ll;
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    uint64_t half_remainder = masks_64bit[shift];
  #elif defined(COMPUTED_MASKS)
    uint64_t half_remainder = 1ull << (shift - static_cast<uint8_t>(1));
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  if ((num & half_remainder) &&
      (num >= 0ll || (num & ((half_remainder << 1) - 1ull)) != half_remainder))
    return (num >> shift) + 1ll;

  return num >> shift;
}

/* Returns ROUND(num / 2^shift). shift must be on the range [0,63]. */
template <> uint64_t shiftround<uint64_t>(const uint64_t num, const uint8_t shift) {
  #ifdef DEBUG_INTMATH
    if (shift > static_cast<uint8_t>(63))
	    std::fprintf(stderr, "ERROR: multshiftround<uint64_t>(%" PRIu64 ", %u), shift = %u is invalid; it must be on the range [0,63].\n", num, shift, shift);
  #endif

  if (shift > static_cast<uint8_t>(63)) return 0ull;
  if (shift == static_cast<uint8_t>(0)) return num;

  #ifdef ARRAY_MASKS
    if (num & masks_64bit[shift])
      return (num >> shift) + 1ull;
  #elif defined(COMPUTED_MASKS)
    if (num & 1ull << (shift - static_cast<uint8_t>(1)))
      return (num >> shift) + 1ull;
  #else
    #error "Exactly one of ARRAY_MASKS or COMPUTED_MASKS must be defined."
  #endif

  return num >> shift;
}

#endif /* #ifndef SHIFTROUND_RUN_HPP_ */

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