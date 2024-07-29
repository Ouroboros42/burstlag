#ifndef LAZY_SUBS_H
#define LAZY_SUBS_H

#include "lazy_arrays/base.hpp"

/* A LazyArray that is determined by part of another LazyArray */
template <typename V, LazyArray<V> A>
class LazySubArray {
    A const& source;
    size_t start_index;
    bool direction; // true: increasing indices, false: decreasing indices

    /* If there are no elements in the array */
    bool is_empty() const {
        // Accounts for 'negative' start index because size_t wraps below 0
        return start_index >= source.size();
    }

public:
    /* Construct an array accessing elements in source, from start_index (inclusive) to the edge of the array
    Direction:
        true - values continue to the end of the row,
        false - values go back to the start of the row 
    If start_index out of bounds, has size 0
    */
    LazySubArray(A const& source, size_t start_index, bool direction) :
        source(source), start_index(start_index), direction(direction)
    {}

    size_t size() const {
        if (is_empty()) return 0;
        return direction ? (source.size() - start_index) : (start_index + 1);
    }

    V get(size_t i) const  {
        return source.get(direction ? (start_index + i) : (start_index - i));
    }
};

#endif