#include "relation.hpp"
#include "fast_sum/sum_terms.hpp"
#include "fast_sum/converging.hpp"

#include <cmath>
#include <optional>

DetectorRelation::DetectorRelation() : DetectorRelation(0, 0, 1) {}

DetectorRelation::DetectorRelation(scalar log_sensitivity_1, scalar log_sensitivity_2, scalar rate_const_1, scalar rate_const_2) :
    log_sensitivity_1(log_sensitivity_1),
    log_sensitivity_2(log_sensitivity_2),
    rate_const_1(rate_const_1),
    rate_const_2(rate_const_2),
    log_rate_const_1(std::log(rate_const_1)),
    log_rate_const_2(std::log(rate_const_2))
{}

DetectorRelation::DetectorRelation(scalar bin_background_rate_1, scalar bin_background_rate_2, scalar sensitivity_ratio_2_to_1)
: DetectorRelation(
    -std::log(1 + sensitivity_ratio_2_to_1),
    -std::log(1 + 1 / sensitivity_ratio_2_to_1),
    bin_background_rate_1 * (1 + sensitivity_ratio_2_to_1),
    bin_background_rate_2 * (1 + 1 / sensitivity_ratio_2_to_1)
) {}

DetectorRelation DetectorRelation::flip() {
    return DetectorRelation(log_sensitivity_2, log_sensitivity_1, rate_const_2, rate_const_1);
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