## RCSn

A C++ implementation of the robust scale estimator Sn directly translated from the Fortran77 implementation found in the paper *Time-Efficient Algorithms for Two Highly Robust Estimators of Scale* by Cristophe Croux and Peter J. Rousseeuw which is published in Computational Statistics, Volume 1: Proceedings of the 10th Symposium on Computational Statistics on pages 411–428. Editors: Yadolah Dodge and Joe Whittaker; Publisher: Physica, Heidleberg; Year 1992; DOI: [https://doi.org/10.1007/978-3-662-26811-7_58](https://doi.org/10.1007/978-3-662-26811-7_58).

RCSn.cpp & RCSn.h provide the O(n log n) implementation of Sn in the function RCSn(). The same files provide the naive O(n²) implementation of Sn in the function RCSn_naive(). The latter is used to verify the O(n log n) implementation in the main() routine of Test\_RCSn.cpp.
