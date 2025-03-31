import math
import numpy


def tangentMethod(x1, x2, x0, acc, k = 1):
    x_arr = [x0]
    p_arr = [func(x0)]
    while True:
        x_new = numpy.argmin([p_k(x, x_arr, k) for x in numpy.linspace(x1, x2, 1000)])
        x_next = numpy.linspace(x1, x2, 1000)[x_new]
        if numpy.abs(x_next - x_arr[-1]) < acc:
            break
        k += 1
        x_arr.append(x_next)
        p_arr.append(p_k(x_next, x_arr, k))
    return [x_arr[-1], func(x_arr[-1]) , len(p_arr)]


def func(x):
    res = 2 + x**2 + x**(2/3) - math.log(1 + x**(2/3)) - 2*x*math.atan(x**(1/3))
    return res


def firstD(x):
    return 2*x - 2 * math.atan(x**(1/3))


def p_k(x, x_arr, k):
            return max(func(x_i) + firstD(x_i) * (x - x_i) for x_i in x_arr[:k])


acc = 10**(-6)
x1 = .5
x2 = 1
x0 = (x1 + x2) / 2
answ = tangentMethod(x1, x2, x0, acc)
print("Метод касательных")
print(f"X: {answ[0]} | Y: {answ[1]} | COUNT: {answ[2]}")
