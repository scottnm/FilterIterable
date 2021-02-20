# A quick benchmark of a trivial iteration/filter

The benchmark test can be viewed in `profile.cpp`

A basic summary of the test is...
1. iterate over an array of ints 0->10
2. sum the even integers into an accumulator variable
3. repeat many many times

| compiler optimization level | (trial avg) not_using_filter | (trial avg) using_filter |
|-|-|-|
| no optimization | 90 | 23 |
| O2 | 9 | 6 |
