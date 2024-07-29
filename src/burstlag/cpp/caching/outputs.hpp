#ifndef OUT_CACHE_H
#define OUT_CACHE_H

#include <tuple>
#include "core.hpp"

typedef std::tuple<size_t, size_t, scalar> likelihood_args;

struct hash_args {
    size_t operator()(likelihood_args const& args) const;
};

#endif