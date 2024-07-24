# distutils: language = c++

cimport cython

from libcpp.vector cimport vector

import logging
import numpy as np
from sys import float_info
from functools import lru_cache

from .cppdefs cimport DetectorRelation as CPPDetectorRelation, FactorialCache as CPPFactorialCache, bin_log_likelihood as cpp_bin_log_likelihood

cdef class DetectorRelation:
    c_rel: CPPDetectorRelation
    c_rel_flipped: CPPDetectorRelation

    def __init__(self, bin_background_rate_1: float = 0., bin_background_rate_2: float = 0., sensitivity_ratio_2_to_1: float = 1.) -> None:
        self.c_rel = CPPDetectorRelation(bin_background_rate_1, bin_background_rate_2, sensitivity_ratio_2_to_1)
        self.c_rel_flipped = self.c_rel.flip()

    @staticmethod
    def expected_real_events(background_rate: float, n_events: int, sample_time: float) -> float:
        expected_background: float = sample_time * background_rate
        
        if expected_background > n_events:
            logging.warning(f"Observed fewer events ({n_events} over {sample_time}) than predicted background_rate ({background_rate}).")
            return float_info.epsilon

        return n_events - expected_background

    @classmethod
    def from_counts(cls, background_rate_1: float, background_rate_2: float, n_events_1: int, n_events_2: int, sample_time: float, bin_width: float) -> DetectorRelation:
        return cls(background_rate_1 * bin_width, background_rate_2 * bin_width,
            cls.expected_real_events(background_rate_2, n_events_2, sample_time) / cls.expected_real_events(background_rate_1, n_events_1, sample_time)
        )

    @classmethod
    def from_hist_arrays(cls, background_rate_1: float, background_rate_2: float, hist_1: np.ndarray, hist_2: np.ndarray, bin_width: float = 1.) -> DetectorRelation:
        n_bins = len(hist_1)
        n_bins_2 = len(hist_2)
        if n_bins != n_bins_2:
            raise IndexError(f"Histograms have different numbers of bins {n_bins}, {n_bins_2}")

        return cls.from_counts(background_rate_1, background_rate_2, int(np.sum(hist_1)), int(np.sum(hist_2)), bin_width * n_bins, bin_width)

cdef class FactorialCache:
    c_cache: CPPFactorialCache

ctypedef fused numeric_in:
    int
    long
    float
    double

cdef size_t convert_to_count(numeric_in n):
    if n < 0:
        raise ValueError(f"Negative count: {n}")

    return <size_t> n

cpdef double bin_log_likelihood(FactorialCache cache, DetectorRelation detectors, numeric_in count_1, numeric_in count_2, double rel_precision, bint use_cache = True):
    cdef size_t u_count_1 = convert_to_count(count_1)
    cdef size_t u_count_2 = convert_to_count(count_2)

    if u_count_1 > u_count_2:
        return cpp_bin_log_likelihood(cache.c_cache, detectors.c_rel, u_count_1, u_count_2, rel_precision, use_cache)
    else:
        return cpp_bin_log_likelihood(cache.c_cache, detectors.c_rel_flipped, u_count_2, u_count_1, rel_precision, use_cache)
    
def log_likelihood(FactorialCache cache, DetectorRelation detectors, numeric_in[:] signal_1, numeric_in[:] signal_2, double rel_precision, bint use_cache = True) -> float:
    cdef Py_ssize_t n_bins = signal_1.shape[0]
    cdef Py_ssize_t n_bins_2 = signal_2.shape[0]
    if n_bins != n_bins_2:
        raise IndexError(f"Signals have different numbers of bins {n_bins}, {n_bins_2}")

    cdef double likelihood = 0
    cdef Py_ssize_t i
    for i in range(n_bins):
        likelihood += bin_log_likelihood(cache, detectors, signal_1[i], signal_2[i], rel_precision, use_cache)

    return likelihood