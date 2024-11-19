# distutils: language = c++

# This is a cython implementation file, the code here is a hybrid of python and c++.
# The docstrings refer to python types (for external users), but function declarations may include python or c++ types.

cimport cython

from libcpp.vector cimport vector

import logging
import numpy as np
from sys import float_info
from functools import lru_cache

from .cppdefs cimport DetectorRelation as CPPDetectorRelation, FactorialCache as CPPFactorialCache

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

cdef class DetectorRelation:
    """ Stores the relative parameters describing two neutrino detectors providing data to SNEWS.
    Implements methods to calculate likelihoods of coincident neutrino bursts. """

    c_rel: CPPDetectorRelation
    c_rel_flipped: CPPDetectorRelation

    bin_background_rate_1: float
    bin_background_rate_2: float
    _sensitivity_ratio_2_to_1: float
    _source_suppression: float

    def __init__(self: DetectorRelation, bin_background_rate_1: float = 0., bin_background_rate_2: float = 0., sensitivity_ratio_2_to_1: float = 1., source_suppression: float = 1.) -> None:
        """
        :param bin_background_rate_1 float: Expected background events per histogram bin at detector 1
        :param bin_background_rate_2 float: Expected background events per histogram bin at detector 2
        :param sensitivity_ratio_2_to_1 float: Ratio of expected supernova events at detector 2 relative to detector 1
        :param source_suppression float: Bayesian prior parameter >= 1, indicating how unlikely high event supernova event counts are. Reccomended to leave close to 1.
        """

        self.bin_background_rate_1 = bin_background_rate_1
        self.bin_background_rate_2 = bin_background_rate_2
        self._sensitivity_ratio_2_to_1 = sensitivity_ratio_2_to_1
        self._source_suppression = source_suppression

        self.c_rel = CPPDetectorRelation(bin_background_rate_1, bin_background_rate_2, sensitivity_ratio_2_to_1, source_suppression)
        self.c_rel_flipped = self.c_rel.flip()

    def __repr__(self: DetectorRelation):
        return f"DetectorRelation({self.bin_background_rate_1}, {self.bin_background_rate_2}, {self._sensitivity_ratio_2_to_1}, {self._source_suppression})"

    @property
    def bin_background_rates(self):
        return (self.bin_background_rate_1, self.bin_background_rate_2)
    
    @property
    def sensitivity_ratio_2_to_1(self):
        return self._sensitivity_ratio_2_to_1

    @property
    def source_suppression(self):
        return self._source_suppression

    @staticmethod
    def expected_real_events(background_rate: float, n_events: int, sample_time: float) -> float:
        expected_background: float = sample_time * background_rate
        
        if expected_background > n_events:
            logging.warning(f"Observed fewer events ({n_events} over {sample_time}) than predicted background_rate ({background_rate}).")
            return float_info.epsilon

        return n_events - expected_background

    @classmethod
    def from_counts(cls, background_rate_1: float, background_rate_2: float, n_events_1: int, n_events_2: int, sample_time: float, bin_width: float, source_suppression: float = 1.) -> DetectorRelation:
        """
        :param background_rate_1 float: Expected rate of background events at detector 1 per unit time
        :param background_rate_2 float: Expected rate of background events at detector 2 per unit time
        :param n_events_1 int: Total number of events detected at detector 1
        :param n_events_2 int: Total number of events detected at detector 2
        :param sample_time float: Time period over which events were detected
        :param bin_width float: Width of histogram bins that will be used
        :param source_suppression float: See class constructor

        Background rates are rescaled based on bin_width, and expected background rates are subtracted from event counts when calculating sensitivities.

        :return A new DetectorRelation instance matching given counts and parameters.
        """

        return cls(background_rate_1 * bin_width, background_rate_2 * bin_width,
            cls.expected_real_events(background_rate_2, n_events_2, sample_time) / cls.expected_real_events(background_rate_1, n_events_1, sample_time),
            source_suppression
        )

    @classmethod
    def from_hist_arrays(cls, background_rate_1: float, background_rate_2: float, hist_1: np.ndarray, hist_2: np.ndarray, bin_width: float = 1., source_suppression: float = 1.) -> DetectorRelation:
        n_bins = len(hist_1)
        n_bins_2 = len(hist_2)
        if n_bins != n_bins_2:
            raise IndexError(f"Histograms have different numbers of bins {n_bins}, {n_bins_2}")

        return cls.from_counts(background_rate_1, background_rate_2, int(np.sum(hist_1)), int(np.sum(hist_2)), bin_width * n_bins, bin_width, source_suppression)
    
    cpdef double bin_log_likelihood(DetectorRelation self, FactorialCache cache, numeric_in count_1, numeric_in count_2, double rel_precision, bint use_cache = True):
        cdef size_t u_count_1 = convert_to_count(count_1)
        cdef size_t u_count_2 = convert_to_count(count_2)

        if u_count_1 > u_count_2:
            return self.c_rel.bin_log_likelihood(cache.c_cache, u_count_1, u_count_2, rel_precision, use_cache)
        else:
            return self.c_rel_flipped.bin_log_likelihood(cache.c_cache, u_count_2, u_count_1, rel_precision, use_cache)

    def log_likelihood(DetectorRelation self, FactorialCache cache, numeric_in[:] signal_1, numeric_in[:] signal_2, double rel_precision, bint use_cache = True) -> float:
        """Calculate the log-likelihood of detecting coincident neutrino counts at the two detectors.

        :param cache FactorialCache: Cache of precalculated factorial values (will be filled if needed)

        :param signal_1 np.ndarray: Histogram of event counts at detector 1
        :param signal_2 np.ndarray: Histogram of event counts at detector 2
            Signal arrays may be of any numeric type, but must be all positive - raises ValueError otherwise. Will be converted to integers by casting (roughly equivalent to floor).
        
        :param rel_precision float: Maximum acceptable error in log-likelihood. This corresponds to relative error in the likelihood.
        
        :param use_cache bool: Determines whether to use the likelihood cache of previous outputs (this is per-bin, so is useful even within a single function call). The factorial cache is always used.

        :return float: The total log-likelihood, within specified precision.

        :raises IndexError: If signal arrays are of different size.
        :raises RuntimeError: If the likelihood sum does not converge as expected (this generally indicates a bug).

        Note: the order of detectors does not matter to speed, the fastest order is automatically used internally, on a per-bin basis.
        """

        cdef Py_ssize_t n_bins = signal_1.shape[0]
        cdef Py_ssize_t n_bins_2 = signal_2.shape[0]
        if n_bins != n_bins_2:
            raise IndexError(f"Signals have different numbers of bins {n_bins}, {n_bins_2}")
        
        cdef double likelihood = 0
        cdef Py_ssize_t i
        try:
            for i in range(n_bins):
                likelihood += self.bin_log_likelihood(cache, signal_1[i], signal_2[i], rel_precision, use_cache)
        except RuntimeError:
            logging.warning(f"Divergent sum term for counts: ({signal_1[i]}, {signal_2[i]}) at precision {rel_precision},\ndetector = {self}")

        return likelihood