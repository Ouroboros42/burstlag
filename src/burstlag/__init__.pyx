# distutils: language = c++

from libcpp.vector cimport vector

from .interface cimport DetectorRelation as CPPDetectorRelation, FactorialCache as CPPFactorialCache, log_likelihood as cpp_log_likelihood

cdef class DetectorRelation:
    c_rel: CPPDetectorRelation

    def __init__(self, bin_background_rate_1: float, bin_background_rate_2: float, total_count_1: int, total_count_2: int, n_bins_sampled: int) -> None:
        self.c_rel = CPPDetectorRelation(bin_background_rate_1, bin_background_rate_2, total_count_1, total_count_2, n_bins_sampled)

cdef class FactorialCache:
    c_cache: CPPFactorialCache

cdef vector[double] arr_to_vec(a: double[:]):
    cdef size_t length = a.shape[0]
    cdef vector[double] v = vector[double](length)

    cdef size_t i
    for i in range(length):
        v[i] = a[i]

    return v

def log_likelihood(cache: FactorialCache, detectors: DetectorRelation, signal_1: double[:], signal_2: double[:], rel_precision: double) -> double:
    return cpp_log_likelihood(cache.c_cache, detectors.c_rel, arr_to_vec(signal_1), arr_to_vec(signal_2), rel_precision)