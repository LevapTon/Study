def taskOne ():
  'Вычисление значения выражения'
  print(18 * (1 / 10) * (30 * (33 / 44) +  50 * (55 / 88)) / (9 / 10))


def taskTwo ():
  'Сумма чисел'
  summa = 0
  for i in range(88888889):
    summa += i
  print (summa)


def taskThree():
  'Среднее арифметическое'
  x = [3, 4, 56, 100, 2, 2, 3]
  print('%.2f' % (sum(x) / len(x)))


taskOne()
taskTwo()
taskThree()