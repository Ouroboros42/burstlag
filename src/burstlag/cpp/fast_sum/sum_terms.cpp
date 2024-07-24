#include "fast_sum/sum_terms.hpp"

#include <cassert>
#include <numeric>
#include <stdexcept>

size_t sum(hist_vec values) {
    return std::reduce(values.begin(), values.end());
}

BinSumTerms::BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2) :
    count_1(count_1), count_2(count_2), fcache(fcache), detectors(detectors)
{
    fcache.build_upto(count_1 + count_2);
}

size_t BinSumTerms::size_x() const { return count_1 + 1; }

size_t BinSumTerms::size_y() const { return count_2 + 1; }

scalar BinSumTerms::get(size_t i, size_t j) const {
    if (i > count_1 || j > count_2) {
        throw std::invalid_argument("Index out of bounds");
    }

    return fcache.log_exp_series_term(detectors.log_rate_const_1, i) +
        fcache.log_exp_series_term(detectors.log_rate_const_2, j) +
        fcache.log_binomial(count_1 - i, count_2 - j);
}

scalar bin_log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision, bool use_cache) {
    likelihood_args arg_key = { count_1, count_2, rel_precision };

    if (use_cache) {
        if (auto cache_item = detectors.previous_outputs.find(arg_key); cache_item != detectors.previous_outputs.end()) {
            return cache_item->second;
        }
    }

    BinSumTerms terms(fcache, detectors, count_1, count_2);

    scalar result = detectors.log_likelihood_prefactor(count_1, count_2) + log_sum_exp_peaked_2D(
        terms,
        detectors.lead_index_1(count_1, count_2),
        [detectors, count_1, count_2] (size_t row_i) { return detectors.lead_index_2(count_1, count_2, row_i); },
        rel_precision
    );

    if (use_cache) detectors.previous_outputs[arg_key] = result;
    
    return result;
}