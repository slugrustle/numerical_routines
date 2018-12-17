## shiftround

The shiftround functions perform the operation ROUND(num / 2^shift) without
using the division operator. The templated C++ form is found in shiftround.hpp.
Standard C functions are in shiftround.h and shiftround.c.

## multshiftround

The multshiftround functions perform the operation ROUND((num * mul) / 2^shift) without
using the division operator. The templated C++ form is found in multshiftround.hpp.
Standard C functions are in multshiftround.h and multshiftround.c.

## general

All functions are defined for the int8\_t, int16\_t, int32\_t, int64\_t, uint8\_t, uint16\_t,
uint32\_t, and uint64\_t types. Shift values range from 1 to two less than the word length
for signed types and from 1 to one less than the word length for unsigned types.

Test code for all functions, both C-style and C++-style, is in multshiftround\_shiftround\_test.cpp.
multshiftround\_shiftround\_test.exe is a 64-bit windows executable meant to be run from
the command line.

## optimal\_base2\_rational

optimal\_base2\_rational is intended to speed the replacement of floating-point
mathematical routines with fixed-point equivalents. multshiftround<shift>(num, mul)
performs ROUND((num * mul) / 2^shift). Conceptually, multshiftround multiplies _num_ by
the rational number (mul / 2^shift) and rounds the result. The optimal\_base2\_rational 
program finds the values of _mul_ and _shift_ that approximate a given fraction to within
the roundoff of multshiftround over a specified range for _num_. It also provides
information about overflow in the product (num * mul).

optimal\_base2\_rational.exe is a 64-bit windows executable meant to be run from
the command line.