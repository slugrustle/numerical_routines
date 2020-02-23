/**
 * shiftround_comp.h
 * Declares functions of the form
 *     type shiftround_X(const type num);
 * where X is a type abbreviation and Y is a shift value. These functions
 * return the value ROUND(num / 2^Y) without using the division operator.
 * The _comp in shiftround_comp.h indicates the shift value must be known
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
#ifndef SHIFTROUND_COMP_H_
#define SHIFTROUND_COMP_H_

#include "inttypes.h"

/********************************************************************************
 ********                        int8_t functions                        ********
 ********************************************************************************/

int8_t shiftround_i8_1(const int8_t num);
int8_t shiftround_i8_2(const int8_t num);
int8_t shiftround_i8_3(const int8_t num);
int8_t shiftround_i8_4(const int8_t num);
int8_t shiftround_i8_5(const int8_t num);
int8_t shiftround_i8_6(const int8_t num);

/********************************************************************************
 ********                       int16_t functions                        ********
 ********************************************************************************/

int16_t shiftround_i16_1(const int16_t num);
int16_t shiftround_i16_2(const int16_t num);
int16_t shiftround_i16_3(const int16_t num);
int16_t shiftround_i16_4(const int16_t num);
int16_t shiftround_i16_5(const int16_t num);
int16_t shiftround_i16_6(const int16_t num);
int16_t shiftround_i16_7(const int16_t num);
int16_t shiftround_i16_8(const int16_t num);
int16_t shiftround_i16_9(const int16_t num);
int16_t shiftround_i16_10(const int16_t num);
int16_t shiftround_i16_11(const int16_t num);
int16_t shiftround_i16_12(const int16_t num);
int16_t shiftround_i16_13(const int16_t num);
int16_t shiftround_i16_14(const int16_t num);

/********************************************************************************
 ********                       int32_t functions                        ********
 ********************************************************************************/

int32_t shiftround_i32_1(const int32_t num);
int32_t shiftround_i32_2(const int32_t num);
int32_t shiftround_i32_3(const int32_t num);
int32_t shiftround_i32_4(const int32_t num);
int32_t shiftround_i32_5(const int32_t num);
int32_t shiftround_i32_6(const int32_t num);
int32_t shiftround_i32_7(const int32_t num);
int32_t shiftround_i32_8(const int32_t num);
int32_t shiftround_i32_9(const int32_t num);
int32_t shiftround_i32_10(const int32_t num);
int32_t shiftround_i32_11(const int32_t num);
int32_t shiftround_i32_12(const int32_t num);
int32_t shiftround_i32_13(const int32_t num);
int32_t shiftround_i32_14(const int32_t num);
int32_t shiftround_i32_15(const int32_t num);
int32_t shiftround_i32_16(const int32_t num);
int32_t shiftround_i32_17(const int32_t num);
int32_t shiftround_i32_18(const int32_t num);
int32_t shiftround_i32_19(const int32_t num);
int32_t shiftround_i32_20(const int32_t num);
int32_t shiftround_i32_21(const int32_t num);
int32_t shiftround_i32_22(const int32_t num);
int32_t shiftround_i32_23(const int32_t num);
int32_t shiftround_i32_24(const int32_t num);
int32_t shiftround_i32_25(const int32_t num);
int32_t shiftround_i32_26(const int32_t num);
int32_t shiftround_i32_27(const int32_t num);
int32_t shiftround_i32_28(const int32_t num);
int32_t shiftround_i32_29(const int32_t num);
int32_t shiftround_i32_30(const int32_t num);

/********************************************************************************
 ********                       int64_t functions                        ********
 ********************************************************************************/

int64_t shiftround_i64_1(const int64_t num);
int64_t shiftround_i64_2(const int64_t num);
int64_t shiftround_i64_3(const int64_t num);
int64_t shiftround_i64_4(const int64_t num);
int64_t shiftround_i64_5(const int64_t num);
int64_t shiftround_i64_6(const int64_t num);
int64_t shiftround_i64_7(const int64_t num);
int64_t shiftround_i64_8(const int64_t num);
int64_t shiftround_i64_9(const int64_t num);
int64_t shiftround_i64_10(const int64_t num);
int64_t shiftround_i64_11(const int64_t num);
int64_t shiftround_i64_12(const int64_t num);
int64_t shiftround_i64_13(const int64_t num);
int64_t shiftround_i64_14(const int64_t num);
int64_t shiftround_i64_15(const int64_t num);
int64_t shiftround_i64_16(const int64_t num);
int64_t shiftround_i64_17(const int64_t num);
int64_t shiftround_i64_18(const int64_t num);
int64_t shiftround_i64_19(const int64_t num);
int64_t shiftround_i64_20(const int64_t num);
int64_t shiftround_i64_21(const int64_t num);
int64_t shiftround_i64_22(const int64_t num);
int64_t shiftround_i64_23(const int64_t num);
int64_t shiftround_i64_24(const int64_t num);
int64_t shiftround_i64_25(const int64_t num);
int64_t shiftround_i64_26(const int64_t num);
int64_t shiftround_i64_27(const int64_t num);
int64_t shiftround_i64_28(const int64_t num);
int64_t shiftround_i64_29(const int64_t num);
int64_t shiftround_i64_30(const int64_t num);
int64_t shiftround_i64_31(const int64_t num);
int64_t shiftround_i64_32(const int64_t num);
int64_t shiftround_i64_33(const int64_t num);
int64_t shiftround_i64_34(const int64_t num);
int64_t shiftround_i64_35(const int64_t num);
int64_t shiftround_i64_36(const int64_t num);
int64_t shiftround_i64_37(const int64_t num);
int64_t shiftround_i64_38(const int64_t num);
int64_t shiftround_i64_39(const int64_t num);
int64_t shiftround_i64_40(const int64_t num);
int64_t shiftround_i64_41(const int64_t num);
int64_t shiftround_i64_42(const int64_t num);
int64_t shiftround_i64_43(const int64_t num);
int64_t shiftround_i64_44(const int64_t num);
int64_t shiftround_i64_45(const int64_t num);
int64_t shiftround_i64_46(const int64_t num);
int64_t shiftround_i64_47(const int64_t num);
int64_t shiftround_i64_48(const int64_t num);
int64_t shiftround_i64_49(const int64_t num);
int64_t shiftround_i64_50(const int64_t num);
int64_t shiftround_i64_51(const int64_t num);
int64_t shiftround_i64_52(const int64_t num);
int64_t shiftround_i64_53(const int64_t num);
int64_t shiftround_i64_54(const int64_t num);
int64_t shiftround_i64_55(const int64_t num);
int64_t shiftround_i64_56(const int64_t num);
int64_t shiftround_i64_57(const int64_t num);
int64_t shiftround_i64_58(const int64_t num);
int64_t shiftround_i64_59(const int64_t num);
int64_t shiftround_i64_60(const int64_t num);
int64_t shiftround_i64_61(const int64_t num);
int64_t shiftround_i64_62(const int64_t num);

/********************************************************************************
 ********                       uint8_t functions                        ********
 ********************************************************************************/

uint8_t shiftround_u8_1(const uint8_t num);
uint8_t shiftround_u8_2(const uint8_t num);
uint8_t shiftround_u8_3(const uint8_t num);
uint8_t shiftround_u8_4(const uint8_t num);
uint8_t shiftround_u8_5(const uint8_t num);
uint8_t shiftround_u8_6(const uint8_t num);
uint8_t shiftround_u8_7(const uint8_t num);

/********************************************************************************
 ********                       uint16_t functions                       ********
 ********************************************************************************/

uint16_t shiftround_u16_1(const uint16_t num);
uint16_t shiftround_u16_2(const uint16_t num);
uint16_t shiftround_u16_3(const uint16_t num);
uint16_t shiftround_u16_4(const uint16_t num);
uint16_t shiftround_u16_5(const uint16_t num);
uint16_t shiftround_u16_6(const uint16_t num);
uint16_t shiftround_u16_7(const uint16_t num);
uint16_t shiftround_u16_8(const uint16_t num);
uint16_t shiftround_u16_9(const uint16_t num);
uint16_t shiftround_u16_10(const uint16_t num);
uint16_t shiftround_u16_11(const uint16_t num);
uint16_t shiftround_u16_12(const uint16_t num);
uint16_t shiftround_u16_13(const uint16_t num);
uint16_t shiftround_u16_14(const uint16_t num);
uint16_t shiftround_u16_15(const uint16_t num);

/********************************************************************************
 ********                       uint32_t functions                       ********
 ********************************************************************************/

uint32_t shiftround_u32_1(const uint32_t num);
uint32_t shiftround_u32_2(const uint32_t num);
uint32_t shiftround_u32_3(const uint32_t num);
uint32_t shiftround_u32_4(const uint32_t num);
uint32_t shiftround_u32_5(const uint32_t num);
uint32_t shiftround_u32_6(const uint32_t num);
uint32_t shiftround_u32_7(const uint32_t num);
uint32_t shiftround_u32_8(const uint32_t num);
uint32_t shiftround_u32_9(const uint32_t num);
uint32_t shiftround_u32_10(const uint32_t num);
uint32_t shiftround_u32_11(const uint32_t num);
uint32_t shiftround_u32_12(const uint32_t num);
uint32_t shiftround_u32_13(const uint32_t num);
uint32_t shiftround_u32_14(const uint32_t num);
uint32_t shiftround_u32_15(const uint32_t num);
uint32_t shiftround_u32_16(const uint32_t num);
uint32_t shiftround_u32_17(const uint32_t num);
uint32_t shiftround_u32_18(const uint32_t num);
uint32_t shiftround_u32_19(const uint32_t num);
uint32_t shiftround_u32_20(const uint32_t num);
uint32_t shiftround_u32_21(const uint32_t num);
uint32_t shiftround_u32_22(const uint32_t num);
uint32_t shiftround_u32_23(const uint32_t num);
uint32_t shiftround_u32_24(const uint32_t num);
uint32_t shiftround_u32_25(const uint32_t num);
uint32_t shiftround_u32_26(const uint32_t num);
uint32_t shiftround_u32_27(const uint32_t num);
uint32_t shiftround_u32_28(const uint32_t num);
uint32_t shiftround_u32_29(const uint32_t num);
uint32_t shiftround_u32_30(const uint32_t num);
uint32_t shiftround_u32_31(const uint32_t num);

/********************************************************************************
 ********                       uint64_t functions                       ********
 ********************************************************************************/

uint64_t shiftround_u64_1(const uint64_t num);
uint64_t shiftround_u64_2(const uint64_t num);
uint64_t shiftround_u64_3(const uint64_t num);
uint64_t shiftround_u64_4(const uint64_t num);
uint64_t shiftround_u64_5(const uint64_t num);
uint64_t shiftround_u64_6(const uint64_t num);
uint64_t shiftround_u64_7(const uint64_t num);
uint64_t shiftround_u64_8(const uint64_t num);
uint64_t shiftround_u64_9(const uint64_t num);
uint64_t shiftround_u64_10(const uint64_t num);
uint64_t shiftround_u64_11(const uint64_t num);
uint64_t shiftround_u64_12(const uint64_t num);
uint64_t shiftround_u64_13(const uint64_t num);
uint64_t shiftround_u64_14(const uint64_t num);
uint64_t shiftround_u64_15(const uint64_t num);
uint64_t shiftround_u64_16(const uint64_t num);
uint64_t shiftround_u64_17(const uint64_t num);
uint64_t shiftround_u64_18(const uint64_t num);
uint64_t shiftround_u64_19(const uint64_t num);
uint64_t shiftround_u64_20(const uint64_t num);
uint64_t shiftround_u64_21(const uint64_t num);
uint64_t shiftround_u64_22(const uint64_t num);
uint64_t shiftround_u64_23(const uint64_t num);
uint64_t shiftround_u64_24(const uint64_t num);
uint64_t shiftround_u64_25(const uint64_t num);
uint64_t shiftround_u64_26(const uint64_t num);
uint64_t shiftround_u64_27(const uint64_t num);
uint64_t shiftround_u64_28(const uint64_t num);
uint64_t shiftround_u64_29(const uint64_t num);
uint64_t shiftround_u64_30(const uint64_t num);
uint64_t shiftround_u64_31(const uint64_t num);
uint64_t shiftround_u64_32(const uint64_t num);
uint64_t shiftround_u64_33(const uint64_t num);
uint64_t shiftround_u64_34(const uint64_t num);
uint64_t shiftround_u64_35(const uint64_t num);
uint64_t shiftround_u64_36(const uint64_t num);
uint64_t shiftround_u64_37(const uint64_t num);
uint64_t shiftround_u64_38(const uint64_t num);
uint64_t shiftround_u64_39(const uint64_t num);
uint64_t shiftround_u64_40(const uint64_t num);
uint64_t shiftround_u64_41(const uint64_t num);
uint64_t shiftround_u64_42(const uint64_t num);
uint64_t shiftround_u64_43(const uint64_t num);
uint64_t shiftround_u64_44(const uint64_t num);
uint64_t shiftround_u64_45(const uint64_t num);
uint64_t shiftround_u64_46(const uint64_t num);
uint64_t shiftround_u64_47(const uint64_t num);
uint64_t shiftround_u64_48(const uint64_t num);
uint64_t shiftround_u64_49(const uint64_t num);
uint64_t shiftround_u64_50(const uint64_t num);
uint64_t shiftround_u64_51(const uint64_t num);
uint64_t shiftround_u64_52(const uint64_t num);
uint64_t shiftround_u64_53(const uint64_t num);
uint64_t shiftround_u64_54(const uint64_t num);
uint64_t shiftround_u64_55(const uint64_t num);
uint64_t shiftround_u64_56(const uint64_t num);
uint64_t shiftround_u64_57(const uint64_t num);
uint64_t shiftround_u64_58(const uint64_t num);
uint64_t shiftround_u64_59(const uint64_t num);
uint64_t shiftround_u64_60(const uint64_t num);
uint64_t shiftround_u64_61(const uint64_t num);
uint64_t shiftround_u64_62(const uint64_t num);
uint64_t shiftround_u64_63(const uint64_t num);

#endif /* #ifndef SHIFTROUND_COMP_H_ */

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