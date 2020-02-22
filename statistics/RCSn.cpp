#/**
  * RCSn.cpp
  * RCSn computes Sn, a robust estimate of scale.
  * This algorithm has been translated to C++ from the Fortran77 version
  * found in the paper
  * "Time-efficient algorithms for two highly robust estimators of scale"
  * by Christophe Croux and Peter J. Rousseeuw
  * Published in Computational Statistics,
  * Volume 1: Proceedings of the 10th Symposium on Computational Statistics
  * Pages 411-428, Editors Yadolah Dodge and Joe Whittaker
  * Publisher Physica, Heidleberg. Year 1992
  * DOI https://doi.org/10.1007/978-3-662-26811-7_58
  *
  * Written in 2020 by Ben Tesch.
  *
  * To the extent possible under law, the author has dedicated all copyright
  * and related and neighboring rights to this software to the public domain
  * worldwide. This software is distributed without any warranty.
  * The text of the CC0 Public Domain Dedication should be reproduced at the
  * end of this file. If not, see http://creativecommons.org/publicdomain/zero/1.0/
  */

#include "RCSn.h"
#include <cmath>
#include <algorithm>
#include <cinttypes>

double RCSn(const std::vector<double> &vData)
{
  ptrdiff_t n = static_cast<ptrdiff_t>(vData.size());

  if (n <= 1)
  {
    return 0.0;
  }

  std::vector<double> a2(n, 0.0);

  /* Initialize y with a copy of vData and sort it. */
  std::vector<double> y(vData);
  std::sort(y.begin(), y.end());
  a2.at(0) = static_cast<double>(y.at(n >> 1) - y.at(0));
  
  for (ptrdiff_t i = 2; i <= ((n+1) >> 1); i++)
  {
    ptrdiff_t nA = i - 1;
    ptrdiff_t nB = n - i;
    ptrdiff_t diff = nB - nA;
    ptrdiff_t leftA = 1;
    ptrdiff_t leftB = 1;
    ptrdiff_t rightA = nB;
    ptrdiff_t rightB = nB;
    ptrdiff_t Amin = 1 + (diff >> 1);
    ptrdiff_t Amax = nA + (diff >> 1);

    while (leftA < rightA)
    {
      ptrdiff_t length = rightA - leftA + 1;
      ptrdiff_t even = 1 - (length & (ptrdiff_t)1);
      ptrdiff_t half = (length - 1) >> 1;
      ptrdiff_t tryA = leftA + half;
      ptrdiff_t tryB = leftB + half;

      if (tryA < Amin)
      {
        rightB = tryB;
        leftA = tryA + even;
      }
      else
      {
        if (tryA > Amax)
        {
          rightA = tryA;
          leftB = tryB + even;
        }
        else
        {
          double medA = y.at(i-1) - y.at(i - tryA + Amin - 2);
          double medB = y.at(tryB + i - 1) - y.at(i - 1);

          if (medA >= medB)
          {
            rightA = tryA;
            leftB = tryB + even;
          }
          else
          {
            rightB = tryB;
            leftA = tryA + even;
          }
        }
      }
    }

    if (leftA > Amax)
    {
      a2.at(i-1) = y.at(leftB + i - 1) - y.at(i - 1);
    }
    else
    {
      double medA = y.at(i - 1) - y.at(i - leftA + Amin - 2);
      double medB = y.at(leftB + i - 1) - y.at(i - 1);
      a2.at(i - 1) = std::min(medA, medB);
    }
  }

  for (ptrdiff_t i = 1 + ((n + 1) >> 1); i <= n - 1; i++)
  {
    ptrdiff_t nA = n - i;
    ptrdiff_t nB = i - 1;
    ptrdiff_t diff = nB - nA;
    ptrdiff_t leftA = 1;
    ptrdiff_t leftB = 1;
    ptrdiff_t rightA = nB;
    ptrdiff_t rightB = nB;
    ptrdiff_t Amin = 1 + (diff >> 1);
    ptrdiff_t Amax = nA + (diff >> 1);

    while (leftA < rightA)
    {
      ptrdiff_t length = rightA - leftA + 1;
      ptrdiff_t even = 1 - (length & (ptrdiff_t)1);
      ptrdiff_t half = (length - 1) >> 1;
      ptrdiff_t tryA = leftA + half;
      ptrdiff_t tryB = leftB + half;

      if (tryA < Amin)
      {
        rightB = tryB;
        leftA = tryA + even;
      }
      else
      {
        if (tryA > Amax)
        {
          rightA = tryA;
          leftB = tryB + even;
        }
        else
        {
          double medA = y.at(i + tryA - Amin) - y.at(i - 1);
          double medB = y.at(i - 1) - y.at(i - tryB - 1);

          if (medA >= medB)
          {
            rightA = tryA;
            leftB = tryB + even;
          }
          else
          {
            rightB = tryB;
            leftA = tryA + even;
          }
        }
      }
    }

    if (leftA > Amax)
    {
      a2.at(i - 1) = y.at(i - 1) - y.at(i - leftB - 1);
    }
    else
    {
      double medA = y.at(i + leftA - Amin) - y.at(i - 1);
      double medB = y.at(i - 1) - y.at(i - leftB - 1);
      a2.at(i - 1) = std::min(medA, medB);
    }
  }

  a2.at(n - 1) = y.at(n - 1) - y.at(((n + 1) >> 1) - 1);
  double cn = 1.0;

  if (n <= 9)
  {
    const double cn_array[11] = { 0.0, 0.0, 0.743, 1.851, 0.954, 1.351, 0.993, 1.198, 1.005, 1.131 };
    cn = cn_array[n];
  }
  else if ((n & (ptrdiff_t)1) == 1)
  {
    double n_d = static_cast<double>(n);
    cn = n_d / (n_d - 0.9);
  }
  
  /**
   * Put the the (n+1)/2 th order statistic of a2 at index
   * (n + 1)/2 - 1 in a2.
   */
  ptrdiff_t order_stat_index = ((n + 1) >> 1) - 1;
  std::nth_element(a2.begin(), a2.begin() + order_stat_index, a2.end());

  /* Return Sn. */
  return (1.1926 * cn * a2.at(order_stat_index));
}


/**
 * RCSn_naive is the straightforward O(n^2) algorithm to compute Sn.
 * It is useful for testing the O(n*log(n)) version above.
 */
double RCSn_naive(const std::vector<double> &vData)
{
  ptrdiff_t n = static_cast<ptrdiff_t>(vData.size());

  if (n <= 1)
  {
    return 0.0;
  }

  std::vector<double> vInner(n, 0.0);
  std::vector<double> vOuter(n, 0.0);

  for (ptrdiff_t i = 0; i < n; i++)
  {
    double i_val = vData.at(i);

    for (ptrdiff_t j = 0; j < n; j++)
    {
      vInner.at(j) = std::fabs(i_val - vData.at(j));
    }

    /**
     * Place the high median ( (n / 2) + 1 th order statistic ) of vInner
     * at index n / 2 in vInner.
     */
    ptrdiff_t order_stat_index = n / 2;
    std::nth_element(vInner.begin(), vInner.begin() + order_stat_index, vInner.end());

    vOuter.at(i) = vInner.at(order_stat_index);
  }

  double cn = 1.0;

  if (n <= 9)
  {
    const double cn_array[11] = { 0.0, 0.0, 0.743, 1.851, 0.954, 1.351, 0.993, 1.198, 1.005, 1.131 };
    cn = cn_array[n];
  }
  else if ((n % 2) == 1)
  {
    double n_d = static_cast<double>(n);
    cn = n_d / (n_d - 0.9);
  }

  /**
  * Place the low median ( (n + 1) / 2 th order statistic ) of vOuter
  * at index (n + 1) / 2 - 1 in vOuter.
  */
  ptrdiff_t order_stat_index = (n + 1) / 2 - 1;
  std::nth_element(vOuter.begin(), vOuter.begin() + order_stat_index, vOuter.end());

  /* Return Sn. */
  return (1.1926 * cn * vOuter.at(order_stat_index));
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