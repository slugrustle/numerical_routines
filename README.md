# numerical\_routines

This repository holds efficient routines for performing rounded shift
operations. These routines are intended for programmers who use
fixed-point arithmetic on microcontrollers or other constrained devices.

I may add a variety of other numerical routines if time permits.

## Getting Started

Everything interesting is in the integer/ directory.

For C++, see shiftround.hpp and multshiftround.hpp.

For C, see shiftround.c, shiftround.h, multshiftround.c, and multshiftround.h.

## Contributing

I would love to receive bug reports and user
feedback. If you have improvements to the code,
please get in touch.

## Author

Ben Tesch - [slugrustle](https://github.com/slugrustle)

## License

All code in this repository that has been produced by the author is distributed under the 
[CC0 1.0 Universal Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/).
See also LICENSE.txt and the individual program files.

A portion of the boost library is distributed in the integer/boost directory. See
integer/boost\_license\_report.hml and integer/LICENSE\_1\_0.txt for more information.

## Acknowledgment

[boost](https://www.boost.org/) provides, among other things, high precision floating point
types that made the test code and helper utility much easier to develop.