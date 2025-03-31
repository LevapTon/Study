from timeit import default_timer as t
from random import randint as r
import FastSorts as f_sorts

count = 1000000
lst = [r(0, 1000000) for _ in range(count)]
print(lst[:10])

start = t()

res = f_sorts.sort_merge(lst)

#res = f_sorts.sort_quick(lst)

#res = f_sorts.sort_quick_fix(lst)

print(round(t() - start, 3))
print(res[:10])