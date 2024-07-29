#ifndef LAZY_H
#define LAZY_H

#include "core.hpp"

/* Lazily evaluated array of values of type V, indexes running over [0, size) */
template <typename A, typename V>
concept LazyArray = requires(A const& array, size_t i) {
    { array.get(i) } -> std::convertible_to<V>;
    { array.size() } -> std::convertible_to<size_t>;
};

/* LazyArray with peak at lead_index */
template <typename A, typename V>
concept PeakedLazyArray = LazyArray<A, V> and requires(A const& array) {
    { array.lead_index() } -> std::convertible_to<size_t>;
};

/* Lazily evaluated 2D-array of values of type V, indexes running over [0, size_1), [0, size_2) */
template <typename A2, typename V>
concept LazyArray2D = requires(A2 const& array, size_t i, size_t j) {
    { array.get(i, j) } -> std::convertible_to<V>;
    { array.size_1() } -> std::convertible_to<size_t>;
    { array.size_2() } -> std::convertible_to<size_t>;
};

/* LazyArray2D with a single peak row at lead_index_1, and a single peak in each row i at lead_index_2(i) */
template <typename A2, typename V>
concept PeakedLazyArray2D = LazyArray2D<A2, V> and requires(A2 const& array, size_t i) {
    { array.lead_index_1() } -> std::convertible_to<size_t>;
    { array.lead_index_2(i) } -> std::convertible_to<size_t>;
};

#endif