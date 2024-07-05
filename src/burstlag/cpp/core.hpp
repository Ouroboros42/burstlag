#ifndef CORE_H
#define CORE_H

#include <vector>
#include <cstddef>

using std::size_t;

/* Type for all scalars - can be changed to adjust speed vs precision */
typedef double scalar;
typedef std::vector<scalar> vec;
typedef std::vector<std::vector<scalar>> mat;

#endif
