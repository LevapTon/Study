def Parity (x):
  '''
  Проверка на четность
  '''
  par = x % 2 == 0
  if par == True:
    return ("Четное")
  else:
    return ("Нечетное")


def biggestNumberAndMultiplicity (nOne, nTwo):
  '''
  Определение большего числа
  И кратны ли числа
  '''
  if nOne > nTwo:
    if nOne % nTwo == 0:
      return("Число {0} кратно {1}".format(nOne, nTwo)) # Нашел такое форматирование
    else:
      return("Число {0} не кратно {1}".format(nOne, nTwo))
  else:
    if nTwo % nOne == 0:
      return("Число {0} кратно {1}".format(nTwo, nOne))
    else:
      return("Число {0} не кратно {1}".format(nTwo, nOne))


def smallestNumber (num1, num2, num3):
  '''
  Определение наименьшего числа 
  '''
  if num1 < num2 and num1 < num3:
    return("Наименьшее число: {0}".format(num1))
  elif num2 < num3:
    return("Наименьшее число: {0}".format(num2))
  else:
    return("Наименьшее число: {0}".format(num3))


def existTrian (side1, side2, side3):
  '''
  Проверка существования треугольника
  '''
  if side1 + side2 > side3 and side1 + side3 > side2 and side2 + side3 > side1:
    return("Треугольник существует")
  else:
    return("Треугольник не существует")


def squareLength (rad):
  '''
  Определение площади и
  Длины круга
  '''
  import math
  square = "%.3f" % (float(math.pi * rad ** 2)) # Нашел ограничение количества
  length = "%.3f" % (float(2 * math.pi * rad)) # Знаков после запятой
  return("Площадь круга: {0} \nДлина окружности: {1}".format(square, length))


def quadraticEquation (a, b, c):
  '''
  Нахождение корней
  Квадратного уравнения
  '''
  import math
  D = float(b ** 2 - 4 * a * c)
  if D < 0:
    return("Уравнение не имеет корней")
  elif D == 0:
    root = "%.3f" % (float(- b / (2 * a)))
    return("""В уравнении один корень 
    Корень: {0}""".format(root))
  else:
    rootOne = "%.3f" % (float((-b + math.sqrt(D)) / (2 * a)))
    rootTwo = "%.3f" % (float((-b - math.sqrt(D)) / (2 * a)))
    return("""В уравнении два корня
    Первый корень: {0}
    Второй корень: {1}""".format(rootOne, rootTwo))