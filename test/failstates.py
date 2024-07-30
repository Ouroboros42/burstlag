import unittest
import numpy as np

from burstlag import FactorialCache, DetectorRelation

class FailStatesTest(unittest.TestCase):
    def test(self):
        cache = FactorialCache()

        a1 = np.array([0, 1, 2, 1, 0], dtype=np.float64)
        a3 = np.array([1, 2, 1, 0, 0, 0], dtype=np.float64)

        self.assertRaises(IndexError, lambda: DetectorRelation.from_hist_arrays(1, 1, a1, a3))

if __name__ == "__main__":
    unittest.main()