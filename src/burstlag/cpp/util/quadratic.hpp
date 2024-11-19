#ifndef QUADRATIC_H
#define QUADRATIC_H

#include "core.hpp"

#include <optional>
#include <utility>

typedef std::optional<std::pair<scalar, scalar>> quad_roots;

/*
Functions to solve quadratic equations:
 - return nullopt if equation unsolvable
 - otherwise return a pair of solutions 
*/

/* Solve x^2 + bx + c = 0 */
quad_roots solve_quadratic(scalar b, scalar c);
/* Solve ax^2 + bx + c = 0 */
quad_roots solve_quadratic(scalar a, scalar b, scalar c);

#endif