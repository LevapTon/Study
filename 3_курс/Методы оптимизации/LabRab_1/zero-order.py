import math


def halvingMethod(x1, x2, delta, acc, count = 1):
    if (abs(x1 - x2) < acc):
        xmin = (x1 + x2) / 2
        y = func(xmin)
        return [xmin, y, count]
    new_x1 = (x1 + x2 - delta) / 2
    new_x2 = (x1 + x2 + delta) / 2
    if (func(new_x1) >= func(new_x2)):
        return halvingMethod(new_x1, x2, delta, acc, count+1)
    else:
        return halvingMethod(x1, new_x2, delta, acc, count+1)


def func(x):
    res = 2 + x**2 + x**(2/3) - math.log(1 + x**(2/3)) - 2*x*math.atan(x**(1/3))
    return res


acc = 10**(-6)
delta = acc / 2
x1 = .5
x2 = 1
answ = halvingMethod(x1, x2, delta, acc)
print("Метод деления пополам")
print(f"X: {answ[0]} | Y: {answ[1]} | COUNT: {answ[2]}")
