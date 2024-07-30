import numpy as np
from timeit import timeit

from burstlag import FactorialCache, DetectorRelation

def simple_timing():
    a1 = np.array([10000, 1000, 100, 10, 1, 0], dtype=np.float64)
    a2 = np.array([20000, 2000, 200, 10, 2, 0], dtype=np.float64)

    cache = FactorialCache()
    rel = DetectorRelation.from_hist_arrays(1., 1., a1, a2)

    precision = 0.1

    compute = lambda: rel.log_likelihood(cache, a1, a1, precision, False)
    
    n = 1000
    avg = timeit(compute, number=n) / n

    print(f"T={avg*1e6:.4g}ns")

if __name__ == "__main__":
    simple_timing()