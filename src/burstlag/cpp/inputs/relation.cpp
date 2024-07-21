#include "relation.hpp"
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
    log_rate_const_2(std::log(rate_const_2)),
    rate_const_ratio_2_to_1(rate_const_2 / rate_const_1)
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

scalar DetectorRelation::log_likelihood_prefactor(size_t total_events_1, size_t total_events_2) {
    return log_sensitivity_1 * total_events_1 + log_sensitivity_2 * total_events_2;
}

std::pair<scalar, scalar> quadratic_roots(scalar b, scalar c) {
    scalar half_b = b / 2;
    scalar discriminant = half_b * half_b - c;
    scalar root_discrim = std::sqrt(discriminant);
    std::pair<scalar, scalar> result = { - half_b - root_discrim, - half_b + root_discrim };

    if (c != 0 && (result.first == 0 || result.second == 0)) { // Limit of numerical accuracy for quadratic, can be approximated linear
        scalar linear_result = - c / b;
        return { linear_result, linear_result };
    }

    return result;
}

std::pair<scalar, scalar> quadratic_roots(scalar a, scalar b, scalar c) {
    if (a == 0) {
        scalar linear_result = - c / b;
        return { linear_result, linear_result };
    }

    scalar b_scaled = b / a, c_scaled = c / a;

    return quadratic_roots(b_scaled, c_scaled);
}

// Convert a real to a the next largest postive integer <= max_index, >= 0
size_t normalise_index(scalar index_est, size_t max_index) {
    if (0 <= index_est && index_est <= max_index) {
        return std::ceil(index_est);
    }

    if (index_est > max_index) return max_index;

    return 0;
}

// Convert the a pair of reals to an integer in range <= max_index, >= 0
std::optional<size_t> normalise_index(std::pair<scalar, scalar> index_ests, size_t max_index) {
    if (0 <= index_ests.first && index_ests.first <= max_index) {
        return std::ceil(index_ests.first);
    }

    if (0 <= index_ests.second && index_ests.second <= max_index) {
        return std::ceil(index_ests.second);
    }

    if (index_ests.first > max_index && index_ests.second > max_index) return max_index;

    if (index_ests.first < 0 && index_ests.second < 0) return 0;

    return std::nullopt;
}

size_t DetectorRelation::lead_index_1(size_t count_1, size_t count_2) const {
    if (log_rate_const_1 < 0) { return 0; }

    std::pair<scalar, scalar> estimates = quadratic_roots(
        1 - rate_const_ratio_2_to_1,
        rate_const_2 - rate_const_1 - 2 * rate_const_ratio_2_to_1 - count_1 - count_2,
        rate_const_2 - rate_const_ratio_2_to_1 - count_2 + (rate_const_1 - 1) * count_1 - 1
    );

    if (auto index = normalise_index(estimates, count_1)) {
        return *index;
    } else { // Handles edge case where the solutions of the lead-term quadratic are outside range, on opposite ends
        return lead_index_1(count_1, count_2, 0);
    }
}

size_t DetectorRelation::lead_index_1(size_t count_1, size_t count_2, size_t index_2) const {
    if (log_rate_const_2 < 0) { return 0; }

    std::pair<scalar, scalar> estimates = quadratic_roots(
        1 - (scalar) count_2 - count_1 + index_2 - rate_const_1,
        (rate_const_1 - 1) * count_1 + index_2 - count_2
    );

    return normalise_index(estimates.first, count_1);
}

size_t DetectorRelation::lead_index_2(size_t count_1, size_t count_2, size_t index_1) const {
    if (log_rate_const_2 < 0) { return 0; }

    std::pair<scalar, scalar> estimates = quadratic_roots(
        1 - (scalar) count_1 - count_2 + index_1 - rate_const_2,
        (rate_const_2 - 1) * count_2 + index_1 - count_1
    );

    return normalise_index(estimates.first, count_2);
}