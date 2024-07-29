#ifndef LAZY_ROWS_H
#define LAZY_ROWS_H

#include "lazy_arrays/base.hpp"

/* A LazyArray that is drawn from a row of a LazyArray2D */
template <typename V, LazyArray2D<V> A2>
class LazyArrayRow {
    A2 const& source;
    size_t row;

public:
    LazyArrayRow(A2 const& source, size_t row) : source(source), row(row) {}

    size_t size() const { return source.size_2(); }

    V get(size_t i) const { return source.get(row, i); }

    template <PeakedLazyArray2D<V> PA2 = A2>
    size_t lead_index() const { return source.lead_index_2(row); }
};

template <typename V, LazyArray2D<V> A2>
class RowsArray {
    A2 const& source;

public:
    RowsArray(A2 const& source) : source(source) {}

    size_t size() const { return source.size_1(); }

    size_t row_size() const  { return source.size_2(); }

    LazyArrayRow<V, A2> get(size_t i) const { return LazyArrayRow<V, A2>(source, i); }
};

#endif