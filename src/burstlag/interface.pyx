# distutils: language = c++

cimport cython

from libcpp.vector cimport vector

import numpy as np

from .cppdefs cimport DetectorRelation as CPPDetectorRelation, FactorialCache as CPPFactorialCache, log_likelihood as cpp_log_likelihood, bin_log_likelihood as cpp_bin_log_likelihood

cdef class DetectorRelation:
    c_rel: CPPDetectorRelation

    def __init__(self, bin_background_rate_1: float, bin_background_rate_2: float, total_count_1: int, total_count_2: int, n_bins_sampled: int) -> None:
        self.c_rel = CPPDetectorRelation(bin_background_rate_1, bin_background_rate_2, total_count_1, total_count_2, n_bins_sampled)

cdef class FactorialCache:
    c_cache: CPPFactorialCache

ctypedef fused numeric_in:
    int
    long
    float
    double

cdef vector[size_t] any_arr_to_countvec(numeric_in[:] arr) except *:
    cdef size_t length = arr.shape[0]
    cdef vector[size_t] v = vector[size_t](length)

    cdef size_t i
    cdef numeric_in elem
    for i in range(length):
        elem = arr[i]
        if elem < 0:
            raise ValueError("Negative Neutrino Count in bin",i,"is illegal")
        else:
            v[i] = <size_t> elem
    return v

def bin_log_likelihood(FactorialCache cache, DetectorRelation detectors, size_t count_1, size_t count_2, double rel_precision) -> double:
    return cpp_bin_log_likelihood(cache.c_cache, detectors.c_rel, count_1, count_2, rel_precision)

def log_likelihood(cache: FactorialCache, detectors: DetectorRelation, signal_1: numeric_in[:], signal_2: numeric_in[:], rel_precision: double) -> double:
    return cpp_log_likelihood(cache.c_cache, detectors.c_rel, any_arr_to_countvec(signal_1), any_arr_to_countvec(signal_2), rel_precision)