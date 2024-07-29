#include "relation.hpp"

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
