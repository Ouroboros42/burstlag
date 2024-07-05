#ifndef FACT_CACHE_H
#define FACT_CACHE_H

#include "core.hpp"

class FactorialCache {
private:
    size_t max_n;
    vec log_n;
    vec log_n_factorial;
public:
    FactorialCache();
    FactorialCache(size_t max_n);

    void build_upto(size_t new_max_n);

    // largest n for which log(n!) is stored
    size_t max() const;

    // log(n) for 0 < n <= size
    inline scalar log(size_t n) const { return log_n[n-1]; }
    
    // log(n!) for 0 <= n <= size
    inline scalar log_factorial(size_t n) const { return log_n_factorial[n]; }

    // log(x^n / n!)
    scalar log_exp_series_term(scalar log_x, size_t index) const;

    // Construct the series log(x^n / n!) for 0 <= n < n_terms
    vec exp_series(scalar x, size_t n_terms) const;

    // log((r+s) C r)
    scalar log_binomial(size_t r, size_t s) const;
};

#endif