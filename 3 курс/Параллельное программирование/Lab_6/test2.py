from random import randint
from math import sqrt, pi
from cmath import exp


def qfp(arr):
    for i in range(len(arr)):
        arr[i] = (1/sqrt(2**m)) * qfpSum(arr, i)
    return arr


def qfpSum(arr, l):
    s = 0
    for i in range(len(arr)):
        s += arr[i]*exp(1j*(2*pi/2**m)*l*i)
    return s


def getResult(arr):
    return (1/sqrt(2**m)) * resultSum(arr)


def resultSum(arr):
    s = 0
    for i in range(len(arr)):
        s += arr[i]
    return s


m = 2 # Количество кубитов
arr = [] # Система кубитов

for i in range(2**m):
    arr.append(randint(0, 1))
print("Система кубитов:", arr)
arr = qfp(arr)
print("КПФ:", arr)
res = getResult(arr)
print("Результат вычисления:", res)
