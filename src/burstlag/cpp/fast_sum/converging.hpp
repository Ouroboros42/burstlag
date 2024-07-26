#ifndef CONVERGING_H
#define CONVERGING_H

#include "fast_sum/lazy.hpp"

#include <functional>

class PeakedLazy2DArray : public Lazy2DArray {
public:
    /* Return index of row with globally largest element */
    virtual size_t lead_index_1() const = 0;
    
    /* Return index of largest element in specified row */
    virtual size_t lead_index_2(size_t index_1) const = 0;

    /*
    Return the log of the sum of exp(terms)
    Assumes terms are strictly decreasing around lead indices
    Relative error should be less than rel_precision
    */
    scalar log_sum_exp(scalar rel_precision);
};

#endif