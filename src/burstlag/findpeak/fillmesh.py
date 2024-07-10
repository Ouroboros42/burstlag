from .. import log_likelihood, DetectorRelation, FactorialCache

import numpy as np

from typing import Callable

MAX_LAG = 0.05

def mesh(n_points: int, max_lag: float = MAX_LAG):
    time_diffs = np.linspace(-max_lag, max_lag, n_points)

def likelihood_mesh(detectors: DetectorRelation, counts_1: np.ndarray, get_offset_counts_2: Callable[[float], np.ndarray], n_points: int, max_lag: float = MAX_LAG) -> tuple[np.ndarray[float], np.ndarray[float]]:
    cache = FactorialCache()

    t_mesh = mesh(n_points, max_lag)

    likelihoods = np.empty_like(t_mesh)

    for i, time_diff in enumerate(t_mesh):
        likelihoods[i] = log_likelihood(cache, detectors, counts_1, get_offset_counts_2(time_diff))

    return t_mesh, likelihoods