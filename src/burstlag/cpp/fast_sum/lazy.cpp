#include "fast_sum/lazy.hpp"

LazyArray::~LazyArray() {}

Lazy2DArray::~Lazy2DArray() {}

LazySubArray::LazySubArray(const Lazy2DArray& source, size_t row, size_t start_index, bool direction) : 
    source(source), row(row),
    direction(direction),
    start_index(start_index)
{}

bool LazySubArray::is_empty() const { return start_index >= source.size_y(); }

size_t LazySubArray::size() const {
    if (is_empty()) return 0;
    return direction ? (source.size_y() - start_index) : (start_index + 1);
}

scalar LazySubArray::get(size_t i) const {
    return source.get(row, direction ? (start_index + i) : (start_index - i));
}