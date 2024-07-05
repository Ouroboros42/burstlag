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

scalar bin_log_likelihood_unscaled(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision) {
    BinSumTerms terms(fcache, detectors, count_1, count_2);

    return log_sum_exp_peaked_2D(
        terms,
        detectors.lead_index_1(count_1, count_2),
        [detectors, count_1, count_2] (size_t row_i) { return detectors.lead_index_2(count_1, count_2, row_i); },
        rel_precision
    );
}

scalar log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, hist_vec signal_1, hist_vec signal_2, scalar rel_precision) {
    assert(signal_1.size() == signal_2.size());
    size_t n_bins = signal_1.size();

    DetectorRelation flipped_detectors = detectors.flip();

    scalar likelihood = detectors.log_likelihood_prefactor(sum(signal_1), sum(signal_2));

    for (size_t i_bin = 0; i_bin < n_bins; i_bin++) {
        size_t count_1 = signal_1[i_bin], count_2 = signal_2[i_bin];

        if (count_1 > count_2) {
            likelihood += bin_log_likelihood_unscaled(fcache, flipped_detectors, count_2, count_1, rel_precision);
        } else {
            likelihood += bin_log_likelihood_unscaled(fcache, detectors, count_1, count_2, rel_precision);
        }
    }

    return likelihood;
}