# numerical\_routines

This repository holds a variety of programs that either implement or employ numerical methods.

The integer/ directory holds efficient routines for performing value saturation, rounded division, and rounded shift operations on integer types. These are intended for programmers who use fixed point arithmetic on microcontrollers or other constrained devices.

The number\_theory/ directory has code implementing Greatest Common Divisor (GCD) and Least Common Multiple (LCM) on the most common unsigned integer types.

The statistics/ directory contains an implementation of Sn, a robust estimator of scale.

The thermistor/ directory contains two programs related to Negative Temperature Coefficient (NTC) thermistors: (1) a simulator of the worst&#x2011;case measurement errors encountered when monitoring an NTC thermistor with an ADC and (2) a near&#x2011;optimal linear interpolation table generator for NTC thermistors.

## Getting Started

The README files the individual directories descibed above are a good place to start.

## Contributing

I would love to receive bug reports and user feedback. If you have improvements to the code, please get in touch.

## Author

Ben Tesch &#x2013; [slugrustle](https://github.com/slugrustle)

## License

All files in this repository that have been produced by the author are distributed under the [CC0 1.0 Universal Public Domain Dedication](https://creativecommons.org/publicdomain/zero/1.0/). See also LICENSE.txt and the files themselves.

A portion of the boost library is distributed in the integer/boost directory. See integer/boost\_license\_report.hml and integer/LICENSE\_1\_0.txt for more information.

## Acknowledgment

[boost](https://www.boost.org/) provides, among other things, high precision floating point types that made the helper utility and some of the test programs in the integer directory much easier to develop.
