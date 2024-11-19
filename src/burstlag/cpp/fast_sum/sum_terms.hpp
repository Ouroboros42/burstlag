#ifndef SUM_TERMS_H
#define SUM_TERMS_H

#include "caching/factorials.hpp"
#include "inputs/relation.hpp"

#include <utility>

/* Terms in sum of to find likelihood of two observed neutrino counts (count_1, count_2) in the same time window,
for the specified detectors. */
class BinSumTerms {
    size_t count_1;
    size_t count_2;

    FactorialCache const& fcache;
    DetectorRelation const& detectors;
    
    /* Get value of largest term in a given row*/
    scalar row_lead(size_t row_i) const {
        return get(row_i, lead_index_2(row_i));
    }

    /* Represents a value and its index */
    typedef std::pair<size_t, scalar> i_val;

    /* Get value of largest term in a given row, with the row's index, for sorting rows */
    i_val indexed_row_lead(size_t row_i) const {
        return { row_i, row_lead(row_i) };
    }

    /* Get the index of the i_val with the largest value, from a finite list */
    template <std::convertible_to<i_val>... Rest>
    size_t peak_index(i_val first, i_val second, Rest... others) const {
        return (first.second > second.second) ? peak_index(first, others...) : peak_index(second, others...);
    }

    /* Special case of peak_index defined above, to allow recursive definition*/
    size_t peak_index(i_val only) const {
        return only.first;
    }

    /* Find the index of the row containting the largest element, from among the row indices specified */
    template <std::convertible_to<size_t>... I>
    size_t peak_index(I... indices) const {
        return peak_index(indexed_row_lead(indices)...);
    }

public:
    BinSumTerms(FactorialCache& fcache, DetectorRelation& detectors, size_t count_1, size_t count_2);
    
    size_t size_1() const;
    size_t size_2() const;

    scalar get(size_t i_1, size_t i_2) const;

    size_t lead_index_1() const;

    size_t lead_index_2(size_t index_1) const;
    
    /* Constant normalisation value added to all log-likelihoods calculated */
    scalar log_likelihood_prefactor() const;
};

#endif