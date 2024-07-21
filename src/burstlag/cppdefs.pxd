from libcpp.vector cimport vector

cdef extern from "caching/factorials.hpp":
    cdef cppclass FactorialCache:
        FactorialCache() except +

cdef extern from "inputs/relation.hpp":
    cdef cppclass DetectorRelation:
        DetectorRelation() except +

        DetectorRelation(
            double bin_background_rate_1,
            double bin_background_rate_2,
            double sensitivity_ratio_2_to_1
        ) except +

        DetectorRelation flip() except +

cdef extern from "fast_sum/sum_terms.hpp":
    cdef double bin_log_likelihood(
        FactorialCache& fcache, DetectorRelation& detectors, 
        size_t count_1, size_t count_2, 
        double rel_precision
    ) except +