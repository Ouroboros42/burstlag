#include "util/quadratic.hpp"

#include <cmath>

quad_roots solve_quadratic(scalar b, scalar c) {
    return solve_quadratic(1, b, c);
}

/* First root always lower */
quad_roots solve_quadratic(scalar a, scalar b, scalar c) {
    if (a == 0) {
        if (b == 0) return std::nullopt; // Solution undetermined (equation has no x dependence)
        // Solve linear case
        scalar soln = - c / b;
        return std::pair(soln, soln);
    }

    if (c == 0) return std::pair(0, - b / a);

    scalar half_b = b / 2;
    scalar root_discriminant = std::sqrt(half_b * half_b - a * c);

    // No real solutions
    if (std::isnan(root_discriminant)) return std::nullopt;

    // This combination avoids catastrophic cancellation if b^2 ~= 4ac
    scalar b_sign = (b < 0) ? -1 : 1;
    scalar ax = -(half_b + b_sign * root_discriminant);

    // Using property that product of roots = c/a
    return std::pair(ax / a, c / ax);
}