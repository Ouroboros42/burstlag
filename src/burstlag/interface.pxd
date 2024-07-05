from libcpp.vector cimport vector

cdef extern from "caching/factorials.hpp":
    cdef cppclass FactorialCache:
        FactorialCache() except +

cdef extern from "inputs/relation.hpp":
    cdef cppclass DetectorRelation:
        DetectorRelation() except +

        DetectorRelation(
            double bin_background_rate_1, double bin_background_rate_2,
            size_t total_count_1, size_t total_count_2,
            size_t n_bins_sampled
        ) except +

cdef extern from "fast_sum/sum_terms.hpp":
    cdef double log_likelihood(
        FactorialCache& fcache, DetectorRelation& detectors,
        vector[double] signal_1, vector[double] signal_2,
        double rel_precision
    ) except +