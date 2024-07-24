#ifndef SUM_TERMS_H
#define SUM_TERMS_H

#include "fast_sum/lazy.hpp"
#include "caching/factorials.hpp"
#include "inputs/relation.hpp"

typedef std::vector<size_t> hist_vec;

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

/* Returns the log_likelihood of the given observed neutrino counts to specified rel_precision */
scalar bin_log_likelihood(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2, scalar rel_precision, bool use_cache);

#endif