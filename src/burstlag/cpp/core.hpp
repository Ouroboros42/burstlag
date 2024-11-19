#ifndef CORE_H
#define CORE_H

#include <vector>
#include <cstddef>

// For debugging
#include <iostream>
#include <cstdio>

/* Unsigned integer type used for indexing - this is also used for event counts for speed and convenience. */
using std::size_t;

/* Type for all scalars - can be changed to adjust speed vs precision */
typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

#endif
