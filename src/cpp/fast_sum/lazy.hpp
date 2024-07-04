#ifndef LAZY_H
#define LAZY_H

#include "core.hpp"

/* A fixed length array of values not stored in memory, whose value is a function of fixed parameters and index */
class LazyArray {
public:
    virtual ~LazyArray();

    /* Number of values in array */
    virtual size_t size() const = 0;

    /* Get the value at index i */
    virtual scalar get(size_t i) const = 0;
};

/* A 2-index, rectangular, fixed size array of values not in memory, whose value is a function of fixed parameters and 2 indices */
class Lazy2DArray {
public:
    virtual ~Lazy2DArray();

    /* Size of array along first index */
    virtual size_t size_x() const = 0;
    /* Size of array along second index*/
    virtual size_t size_y() const = 0;

    /* Get element of array at index (i_x, i_y) */
    virtual scalar get(size_t i_x, size_t i_y) const = 0;
};

/* A LazyArray that is determined by part of a row of another 2-D Lazy Array */
class LazySubArray : public LazyArray {
private:
    const Lazy2DArray& source;

    size_t row;

    size_t start_index;

    // true: increasing indices, false: decreasing indices
    bool direction;

public:
    /* Construct an array accessing elements in row of source, from start_index (inclusive) to the edge of the array
    Direction:
        true - values continue to the end of the row,
        false - values go back to the start of the row 
    If start_index out of bounds, has size 0
    */
    LazySubArray(const Lazy2DArray& source, size_t row, size_t start_index, bool direction);

    /* If there are no elements in the array */
    bool is_empty() const;
    
    size_t size() const;

    scalar get(size_t i) const;
};

#endif