#ifndef CONVERGING_H
#define CONVERGING_H

#include "fast_sum/lazy.hpp"

#include <functional>

typedef std::function<size_t(size_t index_1)> index_getter;

/*
Return the sum of exp(log_terms[i] - log_rescale)
Assumes strictly decreasing terms
Rejects terms of value (relative to total) below term_rel_precision

*/   
scalar converging_sum_exp(LazyArray const& log_terms, scalar total, scalar log_rescale, scalar term_rel_precision);

/*
Return the log of the sum of exp(log_terms)
Assumes largest term is in row[lead_i]
Assumes largest term in row i is in column get_lead_j(i)
Relative error should be less than rel_precision
*/
scalar log_sum_exp_peaked_2D(Lazy2DArray& log_terms, size_t lead_i, index_getter get_lead_j, scalar rel_precision);

#endif