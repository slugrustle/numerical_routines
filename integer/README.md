## saturate\_value

The saturate\_value functions modify the value argument, if necessary, so that it lies on the range [lower\_bound,&#xa0;upper\_bound].

## divround

The divround functions perform the operation ROUND(dividend&#xa0;/&#xa0;divisor).

## shiftround

The shiftround functions perform the operation ROUND(num&#xa0;/&#xa0;2^shift) without using the division operator.

## multshiftround

The multshiftround functions perform the operation ROUND((num&#xa0;*&#xa0;mul)&#xa0;/&#xa0;2^shift) without using the division operator.

## general

The divround, shiftround, and multshiftround functions all employ the "round away from zero" rounding strategy, which is shared by the C++ std::round() function.

All functions are defined for the int8\_t, int16\_t, int32\_t, int64\_t, uint8\_t, uint16\_t, uint32\_t, and uint64\_t types. 

C&#x2011;style functions are found in the .c and .h files. Templated C++&#x2011;style functions are found in the .hpp files.

Test code for both C&#x2011;style and C++&#x2011;style saturate\_value functions is in test\_saturate\_value.cpp.

Test code for both C&#x2011;style and C++&#x2011;style divround functions is in test\_divround.cpp. 

Test code for both C&#x2011;style and C++&#x2011;style shiftround and multshiftround functions with runtime shift argument evaluation is in test\_multshiftround\_shiftround\_run.cpp.

Test code for both C&#x2011;style and C++&#x2011;style shiftround and multshiftround functions that require the shift argument at compile time is in test\_multshiftround\_shiftround\_comp.cpp.

Test code for optional debug checks is in test\_debug\_code.cpp.

All .exe files are 64&#x2011;bit windows executables meant to be run from the command line.

boost code is only used in test\_divround.cpp, test\_multshiftround\_shiftround\_comp.cpp, test\_multshiftround\_shiftround\_run.cpp, and optimal\_pow2\_rational.cpp. The saturate\_value, divround, shiftround, and multshiftround routines only depend on `<limits>` and `"inttypes.h"` or `<cinttypes>`. If debug code is enabled by the directive `#define DEBUG_INTMATH`, these routines may also require some or all of `"stdio.h"`, `<cstdio>`, detect_product_overflow.c, `"detect_product_overflow.h"`, and `"stdbool.h"`.

## \_run and \_comp, ARRAY\_MASKS and COMPUTED\_MASKS

Functions in shiftround\_run.h, shiftround\_run.c, shiftround\_run.hpp, multshiftround\_run.h, multshiftround\_run.c, and multshiftround\_run.hpp take the shift value as a function argument and evaluate it at runtime. If ARRAY\_MASKS is `#define`d in run\_masks\_type.h, rounding masks are found by array lookup and the files multshiftround\_shiftround\_masks.h and multshiftround\_shiftround\_masks.c are required. If COMPUTED\_MASKS is `#define`d instead (also in run\_masks\_type.h), the rounding masks are created by bit shifting, and the files multshiftround\_shiftround\_masks.h and multshiftround\_shiftround\_masks.c are not required.

Functions in shiftround\_comp.h, shiftround\_comp.c, shiftround\_comp.hpp, multshiftround\_comp.h, multshiftround\_comp.c, and multshiftround\_comp.hpp take the shift value as a template argument or as part of the function name&#x2014;the shift value must be known at compile time.

## optimal\_pow2\_rational, number\_system

optimal\_pow2\_rational is intended to speed the replacement of floating point mathematical routines with fixed point equivalents. 

`multshiftround<shift>(num, mul)` performs ROUND((num&#xa0;*&#xa0;mul)&#xa0;/&#xa0;2^shift). Conceptually, `multshiftround<shift>(num, mul)` multiplies _num_ by (mul&#xa0;/&#xa0;2^shift) and rounds the result. (mul&#xa0;/&#xa0;2^shift) is a rational number whose denominator is a power of 2. This allows for efficient computation on a microcontroller that possesses a hardware multiplier and barrel shifter but no hardware divider. The workbooks number\_system.ods and number\_system.xlsx are intended to illustrate the properties of rational numbers with power&#x2011;of&#x2011;two denominators; alternatively, see the printouts number\_system\_8&#x2011;bit\_signed.pdf and number\_system\_8&#x2011;bit\_unsigned.pdf.

The optimal\_pow2\_rational program finds the values of _mul_ and _shift_ that approximate a given fraction to within the roundoff of multshiftround over a specified range for _num_. optimal\_pow2\_rational also provides information about overflow in the product (num&#xa0;*&#xa0;mul).
