import math


def newtonsMethod(x, acc, count = 1):
    nx = x - (firstD(x) / secondD(x))
    if (abs(x - nx) < acc):
        return [x, func(x), count]
    return newtonsMethod(nx, acc, count + 1)


def func(x):
    res = 2 + x**2 + x**(2/3) - math.log(1 + x**(2/3)) - 2*x*math.atan(x**(1/3))
    return res


def firstD(x):
    return 2*x - 2 * math.atan(x**(1/3))


def secondD(x):
    return 2 - (2 / (3 * (math.pow(x, 4/3) + math.pow(x, 2/3))))


acc = 10**(-6)
x0 = .5
answ = newtonsMethod(x0, acc)
print("Метод Ньютона")
print(f"X: {answ[0]} | Y: {answ[1]} | COUNT: {answ[2]}")