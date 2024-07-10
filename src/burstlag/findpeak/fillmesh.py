from .. import log_likelihood, DetectorRelation, FactorialCache

import numpy as np

from typing import Callable

MAX_LAG = 0.05

def mesh(n_points: int):
    time_diffs = np.linspace(-MAX_LAG, MAX_LAG, n_points)

def likelihood_mesh(detectors: DetectorRelation, counts_1: np.ndarray, get_offset_counts_2: Callable[[float], np.ndarray], n_points: int) -> tuple[np.ndarray[float], np.ndarray[float]]:
    cache = FactorialCache()

    t_mesh = mesh(n_points)

    likelihoods = np.empty_like(t_mesh)

    for i, time_diff in enumerate(t_mesh):
        likelihoods[i] = log_likelihood(cache, detectors, counts_1, get_offset_counts_2(time_diff))

    return t_mesh, likelihoods