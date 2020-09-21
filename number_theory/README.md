## gcd

Returns the greatest common divisor of the &ge;2 input integers.

## lcm

Returns the least common multiple of the &ge;2 input integers. Throws an std::overflow\_error exception if the result would overflow the range of the output type.

## general

The gcd and lcm routines are defined for uint8\_t, uint16\_t, uint32\_t, and uint64\_t types. Both gcd and lcm have function variants that take two arguments and function variants that take &ge;2 arguments. The latter come in two varieties: One accepting an initializer list, and the other accepting iterators. See Test\_GCD\_LCM.cpp for usage examples.
