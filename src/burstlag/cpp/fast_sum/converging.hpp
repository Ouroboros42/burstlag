#ifndef CONVERGING_H
#define CONVERGING_H

#include "lazy_arrays/rows.hpp"
#include "lazy_arrays/sub.hpp"

#include <fastexp.hpp>
#include <cmath>
#include <utility>

// Chokepoint of entire operation
scalar exp_scaled(scalar log_x, scalar log_rescale) {
    scalar log_scaled = log_x - log_rescale;

    if (log_scaled == 0) return 1;

    scalar result = fast_exp(log_scaled);

    if (std::isinf(result)) {
        std::cerr << "log_x = " << log_x << "\n" << "log_rescale = " << log_rescale << std::endl;
        throw std::runtime_error("Rescaling did not suppress large term");
    }
    
    return result;
}


/*
Return the sum of exp(log_terms[i] - log_rescale)
Assumes termsa are strictly decreasing with i
Rejects terms of value (relative to total) below term_rel_precision
*/
template <LazyArray<scalar> A>
scalar sum_exp(A const& log_terms, scalar total, scalar log_rescale, scalar term_rel_precision) {
    size_t n_terms = log_terms.size();

    for (size_t i = 0; i < n_terms; i++) {
        scalar next_term = exp_scaled(log_terms.get(i), log_rescale);
        
        if (next_term < total * term_rel_precision) break; // Subsequent terms negligible

        total += next_term;
    }

    return total;
}

template <typename V, LazyArray<V> A>
std::pair<LazySubArray<V, A>, LazySubArray<V, A>> split_tails(A const& array, size_t split_i) {
    return { LazySubArray<V, A>(array, split_i-1, false), LazySubArray<V, A>(array, split_i+1, true) };
}

/* Equivalent to sum_exp, but excluding the term at lead_i, and assuming terms decrease around it */
template <LazyArray<scalar> A>
scalar tail_sum_exp(A const& log_terms, size_t lead_i, scalar total, scalar log_rescale, scalar term_rel_precision) {
    auto [left_tail, right_tail] = split_tails<scalar, A>(log_terms, lead_i);
    total = sum_exp(left_tail, total, log_rescale, term_rel_precision);
    total = sum_exp(right_tail, total, log_rescale, term_rel_precision);
    return total;
}

template <PeakedLazyArray<scalar> A>
scalar peaked_sum_exp(A const& log_terms, scalar total, scalar log_rescale, scalar term_rel_precision) {
    size_t lead_i = log_terms.lead_index();

    scalar lead_term = exp_scaled(log_terms.get(lead_i), log_rescale);
    bool has_converged = lead_term < total * term_rel_precision;
    total += lead_term;

    if (has_converged) return total;

    return tail_sum_exp(log_terms, lead_i, total, log_rescale, term_rel_precision);
}

template <PeakedLazyArray2D<scalar> A2, LazyArray<LazyArrayRow<scalar, A2>> RA>
scalar sum_exp_rows(RA const& rows, scalar total, scalar log_rescale, scalar term_rel_precision) {
    size_t n_rows = rows.size();
    for (size_t i = 0; i < n_rows; i++) {
        scalar old_total = total;
        total = peaked_sum_exp(rows.get(i), total, log_rescale, term_rel_precision);
    }
    return total;
}

template <PeakedLazyArray2D<scalar> A2>
scalar log_sum_exp(A2 log_terms, scalar rel_precision) {
    typedef LazyArrayRow<scalar, A2> RowT;
    typedef RowsArray<scalar, A2> RowsT;
    
    RowsT rows(log_terms);
    size_t lead_row_i = log_terms.lead_index_1();
    auto lead_row = rows.get(lead_row_i);

    size_t lead_row_lead_i = lead_row.lead_index();
    scalar log_rescale = lead_row.get(lead_row_lead_i);
    scalar total = 1;

    scalar term_rel_precision = rel_precision / rows.size() / rows.row_size();

    total = tail_sum_exp(lead_row, lead_row_lead_i, total, log_rescale, term_rel_precision);

    auto [left_tail, right_tail] = split_tails<RowT, RowsT>(rows, lead_row_i);
    typedef LazySubArray<RowT, RowsT> RowsTail;
    total = sum_exp_rows<A2, RowsTail>(left_tail, total, log_rescale, term_rel_precision);
    total = sum_exp_rows<A2, RowsTail>(right_tail, total, log_rescale, term_rel_precision);

    return std::log(total) + log_rescale;
}

#endif