import logging
logger = logging.getLogger(__name__)

import numpy as np
from numpy.typing import ArrayLike
from numpy.polynomial import Polynomial

float_array = np.ndarray[float]

def poly_domain_check(x: ArrayLike, poly: Polynomial):
    lo, hi = poly.domain
    return (lo <= x) & (x <= hi)

def find_peak(time_diff: float_array, likelihood: float_array, degree: int = 10) -> tuple[float, float]:
    pfit = Polynomial.fit(time_diff, likelihood, deg=degree)
    pderiv = pfit.deriv()
    
    extrema = pderiv.roots()
    real_extrema = np.real(extrema[np.is_real(extrema)])
    valid_extrema = real_extrema[poly_domain_check(real_extrema, pfit)]
    
    extrema_likelihoods = pfit(valid_extrema)

    peak_index = np.argmax(extrema_likelihoods)
    peak_time = valid_extrema[peak_index]
    second_deriv = pderiv.deriv()
    fisher_info = -second_deriv(peak_time)
    if fisher_info < 0:
        logger.warning("Likelihood maximum not found - positive second derivative")
    stddev = np.sqrt(1 / np.abs(fisher_info))

    return peak_time, stddev, pfit
