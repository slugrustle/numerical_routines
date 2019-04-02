# numerical\_routines

This repository holds efficient routines for performing value saturation, rounded division, and rounded shift operations on integer types. These routines are intended for programmers who use fixed point arithmetic on microcontrollers or other constrained devices.

There are two numerical solvers related to Negative Temperature Coefficient (NTC) thermistors: (1) a simulator of the measurement errors encountered when monitoring an NTC thermistor with a typical ADC and (2) a near-optimal linear interpolation table generator for NTC thermistors.

## Getting Started

Take a look in the integer/ and The thermistor/ directories.

## Contributing

I would love to receive bug reports and user feedback. If you have improvements to the code, please get in touch.

## Author

Ben Tesch &#x2013; [slugrustle](https://github.com/slugrustle)

## License

All code in this repository that has been produced by the author is distributed under the [CC0 1.0 Universal Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/). See also LICENSE.txt and the individual program files.

A portion of the boost library is distributed in the integer/boost directory. See integer/boost\_license\_report.hml and integer/LICENSE\_1\_0.txt for more information.

A portion of the Eigen library is distributed in the thermistor/Eigen directory; Eigne is distributed under the Mozilla Public License Version 2.0, which can be found at http://mozilla.org/MPL/2.0/.

## Acknowledgment

[boost](https://www.boost.org/) provides, among other things, high precision floating point types that made the helper utility and some of the test programs in the integer directory much easier to develop.

[Eigen](http://eigen.tuxfamily.org/) is a lovely header-only linear algebra library. It is used in the thermistor\_interpolator program for the lowly task of performing least-squares fitting of line segments.