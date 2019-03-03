# numerical\_routines

This repository holds efficient routines for performing value saturation, rounded division, and rounded shift operations on integer types. These routines are intended for programmers who use fixed point arithmetic on microcontrollers or other constrained devices.

There is also a numerical simulator of the measurement errors encountered when monitoring a thermistor with a typical ADC.

## Getting Started

Take a look in the integer/ and The thermistor\_tolerance/ directories.

## Contributing

I would love to receive bug reports and user feedback. If you have improvements to the code, please get in touch.

## Author

Ben Tesch &#x2013; [slugrustle](https://github.com/slugrustle)

## License

All code in this repository that has been produced by the author is distributed under the [CC0 1.0 Universal Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/). See also LICENSE.txt and the individual program files.

A portion of the boost library is distributed in the integer/boost directory. See integer/boost\_license\_report.hml and integer/LICENSE\_1\_0.txt for more information.

## Acknowledgment

[boost](https://www.boost.org/) provides, among other things, high precision floating point types that made the helper utility and some of the test programs in the integer directory much easier to develop.