#ifndef OUT_CACHE_H
#define OUT_CACHE_H

#include <tuple>
#include "core.hpp"

/* Implement hash function for arguments to the log-likelihood calculation, so outputs can be stored in a hashmap */

typedef std::tuple<size_t, size_t, scalar> likelihood_args;

struct hash_args {
    size_t operator()(likelihood_args const& args) const;
};

#endif