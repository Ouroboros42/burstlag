#include "relation.hpp"
#include "fast_sum/sum_terms.hpp"
#include "fast_sum/converging.hpp"

#include <cmath>
#include <optional>

DetectorRelation::DetectorRelation(scalar_pair log_sensitivity, scalar_pair rate_const, scalar_pair log_rate_const) :
    log_sensitivity(log_sensitivity), rate_const(rate_const), log_rate_const(log_rate_const)
{}

DetectorRelation::DetectorRelation(scalar_pair bin_background_rate, scalar_pair sensitivity) :
    log_sensitivity(log(sensitivity)),
    rate_const(bin_background_rate / sensitivity),
    log_rate_const(log(rate_const))
{}

inline scalar_pair sensitivities_from_ratio(scalar sensitivity_ratio_2_to_1) {
    scalar sensitivity_1 = 1 / (1 + sensitivity_ratio_2_to_1);
    return { sensitivity_1, 1 - sensitivity_1 };
}

DetectorRelation::DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1)
: DetectorRelation(
    { bin_background_rate_1, bin_background_rate_2 }, 
    sensitivities_from_ratio(sensitivity_ratio_2_to_1)
) {}

DetectorRelation::DetectorRelation() : DetectorRelation(0, 0, 1) {}

DetectorRelation DetectorRelation::flip() {
    return DetectorRelation(flip_pair(log_sensitivity), flip_pair(rate_const), flip_pair(log_rate_const));
}

scalar DetectorRelation::bin_log_likelihood(FactorialCache& fcache, size_t count_1, size_t count_2, scalar rel_precision, bool use_cache) {
    likelihood_args arg_key = { count_1, count_2, rel_precision };

    if (use_cache) {
        if (auto cache_item = previous_outputs.find(arg_key); cache_item != previous_outputs.end()) {
            return cache_item->second;
        }
    }

    BinSumTerms terms(fcache, *this, count_1, count_2);

    scalar result = terms.log_likelihood_prefactor() + log_sum_exp(terms, rel_precision);

    if (use_cache) previous_outputs[arg_key] = result;
    
    return result;
}