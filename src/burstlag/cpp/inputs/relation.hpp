#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "caching/factorials.hpp"
#include "caching/outputs.hpp"
#include "util/pair_ops.hpp"

#include <unordered_map>
#include <utility>

/* Encodes information about relative rates of both detectors
Relative sensitivity to supernova event is inferred from number of observed events */
class DetectorRelation {
    DetectorRelation(scalar_pair log_sensitivity, scalar_pair rate_const, scalar_pair log_rate_const, scalar log_const_prefactor);

    DetectorRelation(scalar_pair bin_background_rate, scalar_pair sensitivity, scalar log_suppression_prefactor);

    std::unordered_map<likelihood_args, scalar, hash_args> previous_outputs;

    // -log(1 + p/a), -log(1 + a/p)
    scalar_pair log_sensitivity;
    
    // b(1 + p/a) = alpha, q(1 + a/p) = rho
    scalar_pair rate_const;
    scalar_pair log_rate_const;

    scalar log_const_prefactor;

    friend class BinSumTerms;

public:
    DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1, scalar source_suppression);

    DetectorRelation();

    DetectorRelation flip();
    
    /* Returns the log_likelihood of the given observed neutrino counts to specified rel_precision */
    scalar bin_log_likelihood(FactorialCache& fcache, size_t count_1, size_t count_2, scalar rel_precision, bool use_cache);
};

#endif