import BackEnd


# Задача 1

print(BackEnd.Parity(int(input("Введите целое число: "))))
print("–"*29) # Способ визуально отделить задачи в консоли


# Задача 2

numberOne = int(input("Введите 1–е целое число: "))
numberTwo = int(input("Введите 2–е целое число: "))
print(BackEnd.biggestNumberAndMultiplicity(numberOne, numberTwo))
print("–"*29)


# Задача 3

numOne = int(input("Введите 1–е число: "))
numTwo = int(input("Введите 2–е число: "))
numThree = int(input("Введите 3–е число: "))
print(BackEnd.smallestNumber(numOne, numTwo, numThree))
print("–"*29)


# Задача 4

sideOne = int(input("Введите длину 1–й стороны: "))
sideTwo = int(input("Введите длину 2–й стороны: "))
sideThree = int(input("Введите длину 3–й стороны: "))
print(BackEnd.existTrian(sideOne, sideTwo, sideThree))
print("–"*29)


# Задача 5

rad = int(input("Введите радиус окружности: "))
print(BackEnd.squareLength(rad))
print("–"*29)


# Задача 6

a = int(input("Введите коэффициент a: "))
b = int(input("Введите коэффициент b: "))
c = int(input("Введите коэффициент c: "))
print(BackEnd.quadraticEquation(a, b, c))
print("–"*29)
