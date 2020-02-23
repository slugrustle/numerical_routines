/**
 * heapSort.cpp
 *
 * Contains the heapSort() function and related helper functions. These are
 * used to sort the rows of the regressor matrix of the least squares problem
 * in QRleast_squares.cpp in descending order by maximum elementwise absolute
 * value.
 *
 * Written in 2019 by Ben Tesch.
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include "heapSort.h"
#include <algorithm>
#include <cmath>

/**
 * Helper functions that compute indices of parent and
 * child elements in a heap that is stored in a linear
 * array.
 */
static inline uint16_t parentIndex(uint16_t index) {
  return (index-static_cast<uint16_t>(1)) >> 1;
}

static inline uint16_t leftChildIndex(uint16_t index) {
  return static_cast<uint16_t>(2)*index + static_cast<uint16_t>(1);
}

static inline uint16_t rightChildIndex(uint16_t index) {
  return static_cast<uint16_t>(2)*index + static_cast<uint16_t>(2);
}

/**
 * Helper function to compute the maximum absolute value
 * in a row of the regressor matrix part of the least squares
 * problem data storage.
 */
static inline double rowAbsMax(least_squares_row_t *heapArray, uint16_t index) {
  return std::max(std::fabs(heapArray[index].columns[0]), std::fabs(heapArray[index].columns[1]));
}

/**
 * Repairs the min heap stored in heapArray starting
 * from the index of a parent node with valid children.
 */
static void repairHeap(least_squares_row_t *heapArray, uint16_t parent_index, uint16_t last_index)
{
  uint16_t left_child_index = leftChildIndex(parent_index);
  uint16_t right_child_index = left_child_index + static_cast<uint16_t>(1);
  uint16_t swap_index = parent_index;
  double swap_row_abs_max = rowAbsMax(heapArray, swap_index);
  double parent_row_abs_max = swap_row_abs_max;

  while (left_child_index <= last_index)
  { 
    double left_child_row_abs_max = rowAbsMax(heapArray, left_child_index);
    if (swap_row_abs_max > left_child_row_abs_max)
    {
      swap_index = left_child_index;
      swap_row_abs_max = left_child_row_abs_max;
    }

    double right_child_row_abs_max = rowAbsMax(heapArray, right_child_index);
    if (right_child_index <= last_index && swap_row_abs_max > right_child_row_abs_max)
    {
      swap_index = right_child_index;
      swap_row_abs_max = right_child_row_abs_max;
    }

    if (swap_index == parent_index)
    {
      return;
    }
    else
    {
      least_squares_row_t tmp_row = heapArray[parent_index];
      heapArray[parent_index] = heapArray[swap_index];
      heapArray[swap_index] = tmp_row;
      parent_index = swap_index;

      swap_row_abs_max = parent_row_abs_max;

      left_child_index = leftChildIndex(parent_index);
      right_child_index = left_child_index + static_cast<uint16_t>(1);
    }
  }
}

/**
 * Reorders the elements of heapArray so that they are stored
 * as a min heap.
 */
static void createHeap(least_squares_row_t *heapArray, uint16_t last_index)
{
  uint16_t parent_index = parentIndex(last_index);

  while (true)
  {
    repairHeap(heapArray, parent_index, last_index);
    if (parent_index == static_cast<uint16_t>(0)) break;
    parent_index--;
  }
}

/**
 * Sorts an array of least_squares_row_t by the maximum absolute
 * value in columns[] in descending order.
 */
void heapSort(least_squares_row_t *inputArray, uint16_t last_index)
{
  createHeap(inputArray, last_index);

  while (true)
  {
    least_squares_row_t tmp = inputArray[0];
    inputArray[0] = inputArray[last_index];
    inputArray[last_index] = tmp;
    if (last_index == static_cast<uint16_t>(0)) break;
    last_index--;
    repairHeap(inputArray, 0, last_index);
  }
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
