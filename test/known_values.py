import unittest
import numpy as np

from math import log, factorial

from burstlag import FactorialCache, DetectorRelation

class KnownValuesTest(unittest.TestCase):
    def testbig(self):
        cache = FactorialCache()

        rel = DetectorRelation(3000.0, 0.002, 0.010950365266681326)
        rel.bin_log_likelihood(cache, 5812, 22, 1e-2)
        rel.bin_log_likelihood(cache, 2990, 0, 1e-2)

    def testsmall(self):
        precision = 1e-3
        fastExp_err = 3e-2
        eps = precision + fastExp_err

        cache = FactorialCache()

        a1 = np.array([0, 1, 2, 1, 0], dtype=np.float64)
        a1 = np.array([0, 1, 2, 1, 0], dtype=np.float64)
        a2 = np.array([1, 2, 1, 0, 0], dtype=np.float64)

        rel = DetectorRelation.from_hist_arrays(0.1, 0.1, a1, a1)

        b1, b2 = rel.bin_background_rates
        self.assertAlmostEqual(0.1, b1)
        self.assertAlmostEqual(0.1, b2)
        self.assertAlmostEqual(1, rel.sensitivity_ratio_2_to_1)

        self.assertAlmostEqual(-0.980886094868,
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

        self.assertAlmostEqual(-2.76942415484,
            rel.log_likelihood(cache, a1, a1, precision),
        delta=eps)

        self.assertAlmostEqual(-3.58342343727,
            rel.log_likelihood(cache, a1, a2, precision),
        delta=eps)

if __name__ == "__main__":
    unittest.main()