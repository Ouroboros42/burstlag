#ifndef QUADRATIC_H
#define QUADRATIC_H

#include "core.hpp"

#include <optional>
#include <utility>

typedef std::optional<std::pair<scalar, scalar>> quad_roots;

quad_roots solve_quadratic(scalar b, scalar c);
quad_roots solve_quadratic(scalar a, scalar b, scalar c);

#endif