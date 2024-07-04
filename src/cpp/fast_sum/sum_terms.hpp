#ifndef SUM_TERMS_H
#define SUM_TERMS_H

#include "fast_sum/lazy.hpp"
#include "caching/factorials.hpp"
#include "inputs/relation.hpp"

/* Terms in sum of to find likelihood of two observed neutrino counts (count_1, count_2) in the same time window,
for the specified detectors. */
class BinSumTerms : public Lazy2DArray {
public:
    size_t count_1;
    size_t count_2;

    const FactorialCache& fcache;
    const DetectorRelation& detectors;

    BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2);

    size_t size_x() const;
    size_t size_y() const;

    scalar get(size_t i_x, size_t i_y) const;
};

/* Returns the sum to find the log_likelihood of the given observed neutrino counts to specified rel_precision
Neglects overall scaling factors from numbers of events */
scalar bin_log_likelihood_unscaled(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision);

/* Returns the log_likelihood (up to a constant factor which depends only on detectors and number of bins) of observing the two signals from the given detectors */
scalar log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, vec signal_1, vec signal_2, scalar rel_precision);

#endif