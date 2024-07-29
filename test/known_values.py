import unittest
import numpy as np

from math import log, factorial

from burstlag import FactorialCache, DetectorRelation, log_likelihood, bin_log_likelihood

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

        self.assertAlmostEqual(-0.8012042522809977,
            bin_log_likelihood(cache, rel, 1, 2, precision),
        delta=eps)

        self.assertEqual(
            bin_log_likelihood(cache, rel, 10, 2, precision),
            bin_log_likelihood(cache, rel, 10, 2, precision),
        )

        self.assertAlmostEqual(
            bin_log_likelihood(cache, rel, 10, 2, precision, False),
            bin_log_likelihood(cache, rel, 10, 2, precision, False),
        delta=eps)

        self.assertAlmostEqual(-1.7982855941099491,
            log_likelihood(cache, rel, a1, a1, precision),
        delta=eps)

        self.assertAlmostEqual(-2.6169070459462507,
            log_likelihood(cache, rel, a1, a2, precision),
        delta=eps)

if __name__ == "__main__":
    unittest.main()