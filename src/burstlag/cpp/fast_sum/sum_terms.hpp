#ifndef SUM_TERMS_H
#define SUM_TERMS_H

#include "caching/factorials.hpp"
#include "inputs/relation.hpp"

/* Terms in sum of to find likelihood of two observed neutrino counts (count_1, count_2) in the same time window,
for the specified detectors. */
class BinSumTerms {
    size_t count_1;
    size_t count_2;

    FactorialCache const& fcache;
    DetectorRelation const& detectors;

public:
    BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2);

    size_t size_1() const;
    size_t size_2() const;

    scalar get(size_t i_1, size_t i_2) const;

    size_t lead_index_1() const;

    size_t lead_index_2(size_t index_1) const;
    
    scalar log_likelihood_prefactor() const;
};

#endif