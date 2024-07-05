# distutils: language = c++

from libcpp.vector cimport vector

import numpy as np

from .interface cimport DetectorRelation as CPPDetectorRelation, FactorialCache as CPPFactorialCache, log_likelihood as cpp_log_likelihood

cdef class DetectorRelation:
    c_rel: CPPDetectorRelation

    def __init__(self, bin_background_rate_1: float, bin_background_rate_2: float, total_count_1: int, total_count_2: int, n_bins_sampled: int) -> None:
        self.c_rel = CPPDetectorRelation(bin_background_rate_1, bin_background_rate_2, total_count_1, total_count_2, n_bins_sampled)

cdef class FactorialCache:
    c_cache: CPPFactorialCache

cdef vector[size_t] any_arr_to_countvec(arr) except *:
    cdef size_t length = arr.shape[0]
    cdef vector[size_t] v = vector[size_t](length)

    cdef size_t i
    for i in range(length):
        elem = arr[i]
        if elem < 0:
            raise ValueError("Negative Neutrino Count in bin",i,"is illegal")
        else:
            v[i] = <size_t> elem
    return v

def log_likelihood(cache: FactorialCache, detectors: DetectorRelation, signal_1: np.ndarray, signal_2: np.ndarray, rel_precision: double) -> double:
    return cpp_log_likelihood(cache.c_cache, detectors.c_rel, any_arr_to_countvec(signal_1), any_arr_to_countvec(signal_2), rel_precision)