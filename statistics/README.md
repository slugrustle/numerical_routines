## RCSn

RCSn is a C++ implementation of the robust scale estimator Sn directly translated from the Fortran77 implementation found in the paper *Time-Efficient Algorithms for Two Highly Robust Estimators of Scale* by Cristophe Croux and Peter J. Rousseeuw which is published in Computational Statistics, Volume 1: Proceedings of the 10th Symposium on Computational Statistics on pages 411&#x2013;428. Editors: Yadolah Dodge and Joe Whittaker; Publisher: Physica, Heidleberg; Year 1992; DOI: [https://doi.org/10.1007/978-3-662-26811-7_58](https://doi.org/10.1007/978-3-662-26811-7_58).

RCSn.hpp provides the O(n log n) implementation of Sn in the function RCSn(). This version does not modify the input data. RCSn\_InPlace() modifies the input data (sorts it) to preclude making a copy. Finally, RCSn.hpp provides the naive O(n&#xb2;) implementation of Sn in the function RCSn\_naive() which is used to verify the O(n log n) implementations in Test\_RCSn.cpp.

RCSn(), RCSn\_InPlace(), and RCSn\_naive() all accept input floating point data (float or double) via random access iterators.
