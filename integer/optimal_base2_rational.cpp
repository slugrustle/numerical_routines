/**
 * optimal_base2_rational.cpp
 * This program finds a rational number of the form
 *     mult / 2^shift
 * which approximates a user-supplied fraction to within the roundoff
 * of multshiftround when multiplied by integers on a user-specified
 * range.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <cinttypes>
#include <iostream>
#include <limits>
#include <string>
#include <stdexcept>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include <boost/math/special_functions/round.hpp>

typedef boost::multiprecision::number<boost::multiprecision::backends::cpp_bin_float<80, boost::multiprecision::backends::digit_base_2, void, boost::int16_t, -16382, 16383>, boost::multiprecision::et_off> cpp_bin_float_80;

int main(int argc, char *argv[])
{
  std::cout << "\n cmdline arguments: [range min] [range max] [fraction]\n" << std::endl;

  if (argc != 4)
  {
    std::cout << " ERROR: this program must be run with 3 arguments.\n" << std::endl;
    return 0;
  }

  cpp_bin_float_80 range_min;
  try { range_min = cpp_bin_float_80(argv[1]); }
  catch (const std::runtime_error &e)
  {
    std::cout << " ERROR: range min argument could not be converted to an 80-bit mantissa float.\n" << std::endl;
    return 0;
  }
  if ((range_min < cpp_bin_float_80(std::numeric_limits<int64_t>::lowest())) | 
      (range_min > cpp_bin_float_80(std::numeric_limits<uint64_t>::max())))
  {
    std::cout << " ERROR: range min argument is outside the allowed range [-2^63,2^64-1]" << std::endl;
    std::cout << "   which is [" << std::numeric_limits<int64_t>::lowest() << ", " << std::numeric_limits<uint64_t>::max() << "]\n" << std::endl;
    return 0;
  }
  if (range_min != boost::math::round(range_min))
  {
    std::cout << " ERROR: range min argument must be an integer.\n" << std::endl;
    return 0;
  }

  cpp_bin_float_80 range_max;
  try { range_max = cpp_bin_float_80(argv[2]); }
  catch (const std::runtime_error &e)
  {
    std::cout << " ERROR: range max argument could not be converted to an 80-bit mantissa float.\n" << std::endl;
    return 0;
  }
  if ((range_max < cpp_bin_float_80(std::numeric_limits<int64_t>::lowest())) |
      (range_max > cpp_bin_float_80(std::numeric_limits<uint64_t>::max())))
  {
    std::cout << " ERROR: range max argument is outside the allowed range [-2^63,2^64-1]" << std::endl;
    std::cout << "   which is [" << std::numeric_limits<int64_t>::lowest() << ", " << std::numeric_limits<uint64_t>::max() << "]\n" << std::endl;
    return 0;
  }
  if (range_max != boost::math::round(range_max))
  {
    std::cout << " ERROR: range max argument must be an integer.\n" << std::endl;
    return 0;
  }

  if (range_max < range_min)
  {
    std::cout << " ERROR: range max must be greater than or equal to range min.\n" << std::endl;
    return 0;
  }

  cpp_bin_float_80 fraction;
  try { fraction = cpp_bin_float_80(argv[3]); }
  catch (const std::runtime_error &e)
  {
    std::cout << " ERROR: fraction argument could not be converted to an 80-bit mantissa float.\n" << std::endl;
    return 0;
  }
  if ((fraction < cpp_bin_float_80(0.0)) |
      (fraction > cpp_bin_float_80(std::numeric_limits<uint64_t>::max())))
  {
    std::cout << " ERROR: fraction argument is outside the allowed range [0.0,2.0^64-1.0]" << std::endl;
    std::cout << "   which is [0.0, " << std::numeric_limits<uint64_t>::max() << ".0]\n" << std::endl;
    return 0;
  }

  for (int8_t shift = 1; shift <= 63; shift++)
  {
    cpp_bin_float_80 two_exp     = cpp_bin_float_80(1ull << shift);
    cpp_bin_float_80 approx_mult = boost::math::round(fraction * two_exp);
    cpp_bin_float_80 min_prod    = range_min * approx_mult;
    cpp_bin_float_80 approx_min  = min_prod / two_exp;
    cpp_bin_float_80 max_prod    = range_max * approx_mult;
    cpp_bin_float_80 approx_max  = max_prod / two_exp;
    cpp_bin_float_80 ratio_min   = range_min * fraction;
    cpp_bin_float_80 ratio_max   = range_max * fraction;

    if (boost::multiprecision::fabs(approx_min-ratio_min) < cpp_bin_float_80(0.5) &&
        boost::multiprecision::fabs(approx_max-ratio_max) < cpp_bin_float_80(0.5))
    {
      std::cout << " The rational " << std::setprecision(24) << approx_mult << " / 2^" << static_cast<int16_t>(shift) << " = ";
      std::cout << std::setprecision(24) << approx_mult / two_exp << std::endl;
      std::cout << "   approximates fraction = " << std::setprecision(24) << fraction << std::endl;
      std::cout << "   to within roundoff when multiplied by numbers on the range" << std::endl;
      std::cout << "   [" << range_min << ", " << range_max << "].\n" << std::endl;

      std::cout << " The internal product" << std::endl;
      std::cout << "   " << range_min << " * " << approx_mult << " = " << std::setprecision(24) << min_prod << std::endl;
      if (min_prod == cpp_bin_float_80(0.0))
      {
        std::cout << "   will not underflow or overflow any integer type.\n" << std::endl;
      }
      else if (min_prod < cpp_bin_float_80(0.0))
      {
        if (min_prod < cpp_bin_float_80(std::numeric_limits<int64_t>::lowest()))
        {
          std::cout << "   will underflow an int64_t.\n" << std::endl;
        }
        else if (min_prod < cpp_bin_float_80(std::numeric_limits<int32_t>::lowest()))
        {
          std::cout << "   will underflow an int32_t but not an int64_t.\n" << std::endl;
        }
        else if (min_prod < cpp_bin_float_80(std::numeric_limits<int16_t>::lowest()))
        {
          std::cout << "   will underflow an int16_t but not an int32_t.\n" << std::endl;
        }
        else if (min_prod < cpp_bin_float_80(std::numeric_limits<int8_t>::lowest()))
        {
          std::cout << "   will underflow an int8_t but not an int16_t.\n" << std::endl;
        }
        else
        {
          std::cout << "   will not underflow an int8_t.\n" << std::endl;
        }
      }
      else if (min_prod > cpp_bin_float_80(0.0))
      {
        if (min_prod > cpp_bin_float_80(std::numeric_limits<uint64_t>::max()))
        {
          std::cout << "   will overflow a uint64_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<int64_t>::max()))
        {
          std::cout << "   will overflow an int64_t but not a uint64_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<uint32_t>::max()))
        {
          std::cout << "   will overflow a uint32_t but not an int64_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<int32_t>::max()))
        {
          std::cout << "   will overflow an int32_t but not a uint32_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<uint16_t>::max()))
        {
          std::cout << "   will overflow a uint16_t but not an int32_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<int16_t>::max()))
        {
          std::cout << "   will overflow an int16_t but not a uint16_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<uint8_t>::max()))
        {
          std::cout << "   will overflow a uint8_t but not an int16_t.\n" << std::endl;
        }
        else if (min_prod > cpp_bin_float_80(std::numeric_limits<int8_t>::max()))
        {
          std::cout << "   will overflow an int8_t but not a uint8_t.\n" << std::endl;
        }
        else
        {
          std::cout << "   will not overflow an int8_t.\n" << std::endl;
        }
      }

      std::cout << " The internal product" << std::endl;
      std::cout << "   " << range_max << " * " << approx_mult << " = " << std::setprecision(24) << max_prod << std::endl;
      if (max_prod == cpp_bin_float_80(0.0))
      {
        std::cout << "   will not underflow or overflow any integer type.\n" << std::endl;
      }
      else if (max_prod < cpp_bin_float_80(0.0))
      {
        if (max_prod < cpp_bin_float_80(std::numeric_limits<int64_t>::lowest()))
        {
          std::cout << "   will underflow an int64_t.\n" << std::endl;
        }
        else if (max_prod < cpp_bin_float_80(std::numeric_limits<int32_t>::lowest()))
        {
          std::cout << "   will underflow an int32_t but not an int64_t.\n" << std::endl;
        }
        else if (max_prod < cpp_bin_float_80(std::numeric_limits<int16_t>::lowest()))
        {
          std::cout << "   will underflow an int16_t but not an int32_t.\n" << std::endl;
        }
        else if (max_prod < cpp_bin_float_80(std::numeric_limits<int8_t>::lowest()))
        {
          std::cout << "   will underflow an int8_t but not an int16_t.\n" << std::endl;
        }
        else
        {
          std::cout << "   will not underflow an int8_t.\n" << std::endl;
        }
      }
      else if (max_prod > cpp_bin_float_80(0.0))
      {
        if (max_prod > cpp_bin_float_80(std::numeric_limits<uint64_t>::max()))
        {
          std::cout << "   will overflow a uint64_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<int64_t>::max()))
        {
          std::cout << "   will overflow an int64_t but not a uint64_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<uint32_t>::max()))
        {
          std::cout << "   will overflow a uint32_t but not an int64_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<int32_t>::max()))
        {
          std::cout << "   will overflow an int32_t but not a uint32_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<uint16_t>::max()))
        {
          std::cout << "   will overflow a uint16_t but not an int32_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<int16_t>::max()))
        {
          std::cout << "   will overflow an int16_t but not a uint16_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<uint8_t>::max()))
        {
          std::cout << "   will overflow a uint8_t but not an int16_t.\n" << std::endl;
        }
        else if (max_prod > cpp_bin_float_80(std::numeric_limits<int8_t>::max()))
        {
          std::cout << "   will overflow an int8_t but not a uint8_t.\n" << std::endl;
        }
        else
        {
          std::cout << "   will not overflow an int8_t.\n" << std::endl;
        }
      }
      return 0;
    }
  }

  std::cout << " No rational with base 2 denominator was found that" << std::endl;
  std::cout << "   approximates fraction = " << std::setprecision(24) << fraction << std::endl;
  std::cout << "   to within roundoff when multiplied by numbers on the range" << std::endl;
  std::cout << "   [" << range_min << ", " << range_max << "]" << std::endl;
  std::cout << "   for denominators ranging from 2 to 2^63 inclusive.\n" << std::endl;
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