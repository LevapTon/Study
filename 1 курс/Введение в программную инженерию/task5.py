import numpy
import random


def sumNumbersOne():
  res = numpy.arange(1, 101, 1)
  print(sum(res))


def sumNumbersTwo():
  nmr = int(input('''Введите любое целое 
положительное число: '''))
  res = numpy.arange(1, nmr + 1, 1)
  print(sum(res))


def srednee():
  res = numpy.array(range(100))
  for i in range(len(res)):
    res[i] = random.randint(0, 100)
  print('%.2f' % res.mean())


sumNumbersOne()
sumNumbersTwo()
srednee()