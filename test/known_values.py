import unittest
import numpy as np

from math import log, factorial

from burstlag import FactorialCache, DetectorRelation

class KnownValuesTest(unittest.TestCase):
    def test(self):
        precision = 1e-3
        fastExp_err = 3e-2
        eps = precision + fastExp_err

        cache = FactorialCache()

        a1 = np.array([0, 1, 2, 1, 0], dtype=np.float64)
        a1 = np.array([0, 1, 2, 1, 0], dtype=np.float64)
        a2 = np.array([1, 2, 1, 0, 0], dtype=np.float64)

        rel = DetectorRelation.from_hist_arrays(0.1, 0.1, a1, a1)

        self.assertAlmostEqual(-0.780886094868,
            rel.bin_log_likelihood(cache, 1, 2, precision),
        delta=eps)

        self.assertEqual(
            rel.bin_log_likelihood(cache, 10, 2, precision),
            rel.bin_log_likelihood(cache, 10, 2, precision),
        )

        self.assertAlmostEqual(
            rel.bin_log_likelihood(cache, 10, 2, precision, False),
            rel.bin_log_likelihood(cache, 10, 2, precision, False),
        delta=eps)

        self.assertAlmostEqual(-1.76942415484,
            rel.log_likelihood(cache, a1, a1, precision),
        delta=eps)

        self.assertAlmostEqual(-2.58342343727,
            rel.log_likelihood(cache, a1, a2, precision),
        delta=eps)

if __name__ == "__main__":
    unittest.main()