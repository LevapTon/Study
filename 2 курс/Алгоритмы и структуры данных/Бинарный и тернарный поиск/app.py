import numpy as np
from graph import func
from TernarySearch import ternary_search
from GoldenRatio import golden_ratio
from BinarySearch import binary_search


x1 = 2.*np.pi
x2 = 3.*np.pi
xm = ternary_search(x1, x2)
ym = func(xm)
print("X: {0:.4f}, Y: {1:.4f}".format(xm, ym))

xm = golden_ratio(x1, x2)
ym = func(xm)
print("X: {0:.4f}, Y: {1:.4f}".format(xm, ym))

xm = binary_search(x1, x2)
ym = func(xm)
print("X: {0:.4f}, Y: {1:.4f}".format(xm, ym))
