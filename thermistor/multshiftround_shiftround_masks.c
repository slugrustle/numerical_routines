/**
 * multshiftround_shiftround_masks.c
 * The _run versions of multshiftround and shiftround evaluate the shift
 * argument at runtime. Consequently, the masks used for rounding are not
 * hardcoded in the functions themselves and are instead defined in this file.
 *
 * multshiftround_run.c, multshiftround_run.hpp, shiftround_run.c, and
 * shiftround_run.hpp all use the masks_Xbit arrays defined below.
 *
 * Written in 2018 by Ben Tesch.
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file.If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include "multshiftround_shiftround_masks.h"

const uint8_t masks_8bit[7] = { 0x01u, 0x03u, 0x07u, 0x0Fu, 0x1Fu, 0x3Fu, 0x7Fu };

const uint16_t masks_16bit[15] = { 0x0001u, 0x0003u, 0x0007u, 0x000Fu, 0x001Fu,
  0x003Fu, 0x007Fu, 0x00FFu, 0x01FFu, 0x03FFu, 0x07FFu, 0x0FFFu, 0x1FFFu, 0x3FFFu,
  0x7FFFu };

const uint32_t masks_32bit[31] = { 0x00000001u, 0x00000003u, 0x00000007u,
  0x0000000Fu, 0x0000001Fu, 0x0000003Fu, 0x0000007Fu, 0x000000FFu, 0x000001FFu,
  0x000003FFu, 0x000007FFu, 0x00000FFFu, 0x00001FFFu, 0x00003FFFu, 0x00007FFFu,
  0x0000FFFFu, 0x0001FFFFu, 0x0003FFFFu, 0x0007FFFFu, 0x000FFFFFu, 0x001FFFFFu,
  0x003FFFFFu, 0x007FFFFFu, 0x00FFFFFFu, 0x01FFFFFFu, 0x03FFFFFFu, 0x07FFFFFFu,
  0x0FFFFFFFu, 0x1FFFFFFFu, 0x3FFFFFFFu, 0x7FFFFFFFu };

const uint64_t masks_64bit[63] = { 0x0000000000000001ull, 0x0000000000000003ull,
  0x0000000000000007ull, 0x000000000000000Full, 0x000000000000001Full, 0x000000000000003Full,
  0x000000000000007Full, 0x00000000000000FFull, 0x00000000000001FFull, 0x00000000000003FFull,
  0x00000000000007FFull, 0x0000000000000FFFull, 0x0000000000001FFFull, 0x0000000000003FFFull,
  0x0000000000007FFFull, 0x000000000000FFFFull, 0x000000000001FFFFull, 0x000000000003FFFFull,
  0x000000000007FFFFull, 0x00000000000FFFFFull, 0x00000000001FFFFFull, 0x00000000003FFFFFull,
  0x00000000007FFFFFull, 0x0000000000FFFFFFull, 0x0000000001FFFFFFull, 0x0000000003FFFFFFull,
  0x0000000007FFFFFFull, 0x000000000FFFFFFFull, 0x000000001FFFFFFFull, 0x000000003FFFFFFFull,
  0x000000007FFFFFFFull, 0x00000000FFFFFFFFull, 0x00000001FFFFFFFFull, 0x00000003FFFFFFFFull,
  0x00000007FFFFFFFFull, 0x0000000FFFFFFFFFull, 0x0000001FFFFFFFFFull, 0x0000003FFFFFFFFFull,
  0x0000007FFFFFFFFFull, 0x000000FFFFFFFFFFull, 0x000001FFFFFFFFFFull, 0x000003FFFFFFFFFFull,
  0x000007FFFFFFFFFFull, 0x00000FFFFFFFFFFFull, 0x00001FFFFFFFFFFFull, 0x00003FFFFFFFFFFFull,
  0x00007FFFFFFFFFFFull, 0x0000FFFFFFFFFFFFull, 0x0001FFFFFFFFFFFFull, 0x0003FFFFFFFFFFFFull,
  0x0007FFFFFFFFFFFFull, 0x000FFFFFFFFFFFFFull, 0x001FFFFFFFFFFFFFull, 0x003FFFFFFFFFFFFFull,
  0x007FFFFFFFFFFFFFull, 0x00FFFFFFFFFFFFFFull, 0x01FFFFFFFFFFFFFFull, 0x03FFFFFFFFFFFFFFull,
  0x07FFFFFFFFFFFFFFull, 0x0FFFFFFFFFFFFFFFull, 0x1FFFFFFFFFFFFFFFull, 0x3FFFFFFFFFFFFFFFull,
  0x7FFFFFFFFFFFFFFFull };

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