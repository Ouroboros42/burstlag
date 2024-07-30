#include "fast_sum/sum_terms.hpp"
#include "util/quadratic.hpp"

#include <cassert>
#include <numeric>
#include <stdexcept>
#include <cmath>

BinSumTerms::BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2) :
    count_1(count_1), count_2(count_2), fcache(fcache), detectors(detectors)
{
    fcache.build_upto(count_1 + count_2);
}

size_t BinSumTerms::size_1() const { return count_1 + 1; }

size_t BinSumTerms::size_2() const { return count_2 + 1; }

scalar BinSumTerms::get(size_t i, size_t j) const {
    if (i > count_1 || j > count_2) {
        throw std::invalid_argument("Index out of bounds");
    }

    return fcache.log_exp_series_term(detectors.log_rate_const.first, i) +
        fcache.log_exp_series_term(detectors.log_rate_const.second, j) +
        fcache.log_binomial(count_1 - i, count_2 - j);
}

// Convert a real to a the next largest postive integer in range [0, max_index], or closest bound
size_t clamp_index(scalar index_est, size_t max_index) {
    if (index_est > max_index) return max_index;
    if (index_est < 0) return 0;

    return std::ceil(index_est);
}

size_t BinSumTerms::lead_index_2(size_t index_1) const {
    if (detectors.rate_const.second < 1) return 0;

    quad_roots roots = solve_quadratic(
        1 - (scalar) count_1 - count_2 + index_1 - detectors.rate_const.second,
        (detectors.rate_const.second - 1) * count_2 + index_1 - count_1
    );

    if (roots) return clamp_index(std::min(roots->first, roots->second), count_2);

    scalar first_term = get(index_1, 0);
    scalar last_term = get(index_1, count_2);

    return (first_term > last_term) ? 0 : count_2;
}

/* Return ceil of first element from pair in range [0, max_index].*/
std::optional<size_t> valid_index(quad_roots index_ests, size_t max_index) {
    if (!index_ests) return std::nullopt;

    auto [first, second] = *index_ests;
    if (0 <= first && first <= max_index) return std::ceil(first);
    if (0 <= second && second <= max_index) return std::ceil(second);

    return std::nullopt;
}

size_t BinSumTerms::lead_index_1() const {
    if (detectors.rate_const.first < 1) return 0;

    quad_roots roots = solve_quadratic(
        detectors.rate_const.first - detectors.rate_const.second,
        (detectors.rate_const.second - detectors.rate_const.first - count_1 - count_2) * detectors.rate_const.first - 2 * detectors.rate_const.second,
        (detectors.rate_const.first - 1) * (count_1 + detectors.rate_const.second) - count_2 - 1
    );

    if (auto index = valid_index(roots, count_1)) return *index;

    // Analytic location has failed, peak must be at boundary
    scalar first_lead = get(0, lead_index_2(0));
    scalar last_lead = get(count_1, lead_index_2(count_1));

    return (first_lead > last_lead) ? 0 : count_1;
}

scalar BinSumTerms::log_likelihood_prefactor() const {
    return detectors.log_sensitivity.first * count_1 + detectors.log_sensitivity.second * count_2;
}