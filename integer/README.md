## saturate\_value

The saturate\_value functions modify the value argument, if necessary, so that it lies on the range [lower\_bound, upper\_bound].

## divround

The divround functions perform the operation ROUND(dividend / divisor).

## shiftround

The shiftround functions perform the operation ROUND(num / 2^shift) without using the division operator.

## multshiftround

The multshiftround functions perform the operation ROUND((num * mul) / 2^shift) without using the division operator.

## general

All functions are defined for the int8\_t, int16\_t, int32\_t, int64\_t, uint8\_t, uint16\_t, uint32\_t, and uint64\_t types. 

C&#x2011;style functions are found in the .c and .h files. Templated C++&#x2011;style functions are found in the .hpp files.

Test code for both C&#x2011;style and C++&#x2011;style saturate\_value functions is in saturate\_value\_test.cpp.

Test code for both C&#x2011;style and C++&#x2011;style divround functions is in divround\_test.cpp. 

Test code for both C&#x2011;style and C++&#x2011;style shiftround and multshiftround functions is in multshiftround\_shiftround\_test.cpp.

The .exe file corresponding to each ...\_test.cpp file is a 64&#x2011;bit windows executable meant to be run from the command line.

boost code is only used in divround\_test.cpp, multshiftround\_shiftround\_test.cpp, and optimal\_base2\_rational.cpp. The saturate\_value, divround, shiftround, and multshiftround routines only depend on `"inttypes.h"` and/or `<cinttypes>`. If debug code is enabled by the directive `#define DEBUG_INTMATH`, these routines may also require some or all of `"stdio.h"`, `<cstdio>`, `<limits>`, detect_product_overflow.c, `"detect_product_overflow.h"`, and `"stdbool.h"`. 

## \_run and \_comp

Functions in shiftround\_run.h, shiftround\_run.c, shiftround\_run.hpp, multshiftround\_run.h, multshiftround\_run.c, and multshiftround\_run.hpp take the shift value as a function argument and evaluate it at runtime. These functions require the files multshiftround\_shiftround\_masks.h and multshiftround\_shiftround\_masks.c.

Functions in shiftround\_comp.h, shiftround\_comp.c, shiftround\_comp.hpp, multshiftround\_comp.h, multshiftround\_comp.c, and multshiftround\_comp.hpp take the shift value as a template argument or as part of the function name&#x2014;the shift value must be known at compile time.

## optimal\_base2\_rational

optimal\_base2\_rational is intended to speed the replacement of floating point mathematical routines with fixed point equivalents. `multshiftround<shift>(num, mul)` performs ROUND((num * mul) / 2^shift). Conceptually, `multshiftround<shift>(num, mul)` multiplies _num_ by the rational number (mul / 2^shift) and rounds the result. The optimal\_base2\_rational program finds the values of _mul_ and _shift_ that approximate a given fraction to within the roundoff of multshiftround over a specified range for _num_. optimal\_base2\_rational also provides information about overflow in the product (num * mul).

optimal\_base2\_rational.exe is a 64&#x2011;bit windows executable meant to be run from the command line.