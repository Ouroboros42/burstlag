#ifndef COMPARISON_H
#define COMPARISON_H

#include "core.hpp"
#include "caching/factorials.hpp"
#include "caching/outputs.hpp"

#include <unordered_map>

/* Encodes information about relative rates of both detectors
Relative sensitivity to supernova event is inferred from number of observed events */
struct DetectorRelation {
    public:
        // -log(1 + p/a)
        scalar log_sensitivity_1;
        // -log(1 + a/p)
        scalar log_sensitivity_2;
        
        // b(1 + p/a) = alpha
        scalar rate_const_1;  
        scalar log_rate_const_1;

        // q(1 + a/p) = rho
        scalar rate_const_2;
        scalar log_rate_const_2;

        scalar rate_const_ratio_2_to_1;

        std::unordered_map<likelihood_args, scalar, hash_args> previous_outputs;

        DetectorRelation();
        
        DetectorRelation(scalar log_sensitivity_1, scalar log_sensitivity_2, scalar rate_const_1, scalar rate_const_2);

        DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1);

        DetectorRelation flip();

        scalar log_likelihood_prefactor(size_t total_events_1, size_t total_events_2);
};

#endif