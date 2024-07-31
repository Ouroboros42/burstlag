#ifndef PAIR_OPS_H
#define PAIR_OPS_H

#include "core.hpp"

#include <utility>
#include <cmath>

typedef std::pair<scalar, scalar> scalar_pair;

inline scalar_pair flip_pair(scalar_pair p) {
    return { p.second, p.first };
}

inline scalar sum(scalar_pair p) {
    return p.first + p.second;
}

#define PAIR_OP(OP) \
inline scalar_pair operator OP(scalar_pair a, scalar_pair b) { \
	return { a.first OP b.first, a.second OP b.second }; \
} \
inline scalar_pair operator OP(scalar_pair a, scalar b) { \
    return { a.first OP b, a.second OP b }; \
} \
inline scalar_pair operator OP(scalar a, scalar_pair b) { \
    return { a OP b.first, a OP b.second }; \
}

PAIR_OP(+)
PAIR_OP(-)
PAIR_OP(*)
PAIR_OP(/)

#define PAIR_MAP(NAME, APPLY_FUN) \
inline scalar_pair NAME(scalar_pair x) { \
    return { APPLY_FUN(x.first), APPLY_FUN(x.second) }; \
}

PAIR_MAP(log, std::log)
PAIR_MAP(exp, std::exp)

#endif