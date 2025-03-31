import BackEnd


def taskOne():
  '''Задача №1'''
  palind = str(input('Введите строку для проверки \nее на палиндромность: \n'))
  BackEnd.palindrome(palind)
  print()


def taskTwo():
  '''Задача №2'''
  x = int(input('Введите количество столбцов в таблице: '))
  y = int(input('Введите количество строк в таблице: '))
  BackEnd.multiTable(x,y)
  print()


def taskThree():
  '''Задача №3'''
  height = int(input('Введите высоту пирамиды: '))
  BackEnd.pyramid(height)
  print()


def taskFour():
  '''Задача №4'''
  lin = str(input('Введите предложение:\n'))
  BackEnd.parsingAString(lin)
  print()


def taskFive():
  '''Задача №5'''
  while True:
    rubles = int(input('Введите число в диапазоне [0;100]: '))
    if 0 <= rubles <= 100:
      break
    else:
      print('Введено некорректное число')
  BackEnd.rublesEnd(rubles)


taskOne()
taskTwo()
taskThree()
taskFour()
taskFive()
