#include "caching/factorials.hpp"

#include <cmath>

FactorialCache::FactorialCache()
: max_n(1), log_n { 0 }, log_n_factorial { 0, 0 } {}

FactorialCache::FactorialCache(size_t max_n) : FactorialCache() {
    build_upto(max_n);
}

size_t FactorialCache::max() const {
    return max_n;
}

void FactorialCache::build_upto(size_t new_max_n) {
    if (new_max_n <= max_n) return;

    scalar running_sum = log_factorial(max_n);

    log_n.resize(new_max_n);
    log_n_factorial.resize(new_max_n + 1);

    for (size_t i = max_n + 1; i <= new_max_n; i++) {
        running_sum += log_n[i - 1] = std::log(i);
        log_n_factorial[i] = running_sum;
    }

    max_n = new_max_n;
}

scalar FactorialCache::log_exp_series_term(scalar log_x, size_t index) const {
    return (index * log_x) - log_factorial(index);
}

vec FactorialCache::exp_series(scalar x, size_t n_terms) const {
    scalar log_x = log(x);
    vec expansion(n_terms);

    for (size_t i = 0; i < n_terms; i++) {
        expansion[i] = log_exp_series_term(log_x, i);
    }

    return expansion;
}

scalar FactorialCache::log_binomial(size_t r, size_t s) const {
    return log_factorial(r + s) - log_factorial(r) - log_factorial(s);
}