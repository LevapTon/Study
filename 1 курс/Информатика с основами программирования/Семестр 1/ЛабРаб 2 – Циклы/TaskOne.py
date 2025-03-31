def WorkWithNumbers(numOne, numTwo):
  i = 0
  for i in range(numOne, numTwo + 1):
    if i % 2 != 0:
      print(i)
      i += 1
    else:
      i += 1


while True:
  numberOne = int(input("Введите первое число из диапозона [-100; 100] – "))
  if numberOne < -100 or numberOne > 100:
    print("Вы ввели неверное число")
  else:
    break

while True:
  numberTwo = int(input("Введите второе число из диапозона [-100; 100] – "))
  if numberTwo < -100 or numberTwo > 100:
    print("Вы ввели неверное число")
  else:
    break

if numberOne < numberTwo:
  WorkWithNumbers(numberOne, numberTwo)
else:
  WorkWithNumbers(numberTwo, numberOne)