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
 * Originally distributed at https://github.com/slugrustle/numerical_routines
 *
 * To the extent possible under law, the author has dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide.This software is distributed without any warranty.
 * The text of the CC0 Public Domain Dedication should be reproduced at the
 * end of this file. If not, see http ://creativecommons.org/publicdomain/zero/1.0/
 */

#include "multshiftround_shiftround_masks.h"

const uint8_t masks_8bit[8] = {
  0x00u, 0x01u, 0x02u, 0x04u, 0x08u, 
         0x10u, 0x20u, 0x40u
};

const uint16_t masks_16bit[16] = {
  0x0000u, 0x0001u, 0x0002u, 0x0004u, 0x0008u,
           0x0010u, 0x0020u, 0x0040u, 0x0080u,
           0x0100u, 0x0200u, 0x0400u, 0x0800u,
           0x1000u, 0x2000u, 0x4000u
};

const uint32_t masks_32bit[32] = {
  0x00000000u, 0x00000001u, 0x00000002u, 0x00000004u, 0x00000008u,
               0x00000010u, 0x00000020u, 0x00000040u, 0x00000080u,
               0x00000100u, 0x00000200u, 0x00000400u, 0x00000800u,
               0x00001000u, 0x00002000u, 0x00004000u, 0x00008000u,
               0x00010000u, 0x00020000u, 0x00040000u, 0x00080000u,
               0x00100000u, 0x00200000u, 0x00400000u, 0x00800000u,
               0x01000000u, 0x02000000u, 0x04000000u, 0x08000000u,
               0x10000000u, 0x20000000u, 0x40000000u
};

const uint64_t masks_64bit[64] = { 
  0x0000000000000000ull, 0x0000000000000001ull, 0x0000000000000002ull, 0x0000000000000004ull, 0x0000000000000008ull,
                         0x0000000000000010ull, 0x0000000000000020ull, 0x0000000000000040ull, 0x0000000000000080ull,
                         0x0000000000000100ull, 0x0000000000000200ull, 0x0000000000000400ull, 0x0000000000000800ull,
                         0x0000000000001000ull, 0x0000000000002000ull, 0x0000000000004000ull, 0x0000000000008000ull,
                         0x0000000000010000ull, 0x0000000000020000ull, 0x0000000000040000ull, 0x0000000000080000ull,
                         0x0000000000100000ull, 0x0000000000200000ull, 0x0000000000400000ull, 0x0000000000800000ull,
                         0x0000000001000000ull, 0x0000000002000000ull, 0x0000000004000000ull, 0x0000000008000000ull,
                         0x0000000010000000ull, 0x0000000020000000ull, 0x0000000040000000ull, 0x0000000080000000ull,
                         0x0000000100000000ull, 0x0000000200000000ull, 0x0000000400000000ull, 0x0000000800000000ull,
                         0x0000001000000000ull, 0x0000002000000000ull, 0x0000004000000000ull, 0x0000008000000000ull,
                         0x0000010000000000ull, 0x0000020000000000ull, 0x0000040000000000ull, 0x0000080000000000ull,
                         0x0000100000000000ull, 0x0000200000000000ull, 0x0000400000000000ull, 0x0000800000000000ull,
                         0x0001000000000000ull, 0x0002000000000000ull, 0x0004000000000000ull, 0x0008000000000000ull,
                         0x0010000000000000ull, 0x0020000000000000ull, 0x0040000000000000ull, 0x0080000000000000ull,
                         0x0100000000000000ull, 0x0200000000000000ull, 0x0400000000000000ull, 0x0800000000000000ull,
                         0x1000000000000000ull, 0x2000000000000000ull, 0x4000000000000000ull
};

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