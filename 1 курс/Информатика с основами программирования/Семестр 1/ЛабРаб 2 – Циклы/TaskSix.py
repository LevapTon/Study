def ChtotoProishodid(numberOne, numberTwo):
  if numberOne < numberTwo:
    a = numberOne
    b = numberTwo
  else:
    a = numberTwo
    b = numberOne
  count = 0
  divOne = 0
  divTwo = 0
  for i in range(a, b + 1):
    for n in range(2, i):
      if i % n == 0:
        count += 1
        if count == 1: 
          divOne = n
        elif count == 2:
          divTwo = n
      if count > 2:
        count = 0
        break
    if count == 2:
      print("для ", i, ": ", end = " ")

      print(divOne, " ", divTwo)
    else:
      count = 0


while True:
  numberOne = int(input("Введите первое число из диапозона [100000; 200000] – "))
  if numberOne < 100000 or numberOne > 200000:
    print("Вы ввели неверное число")
  else:
    break

while True:
  numberTwo = int(input("Введите второе число из диапозона [100000; 200000] – "))
  if numberTwo < 100000 or numberTwo > 200000:
    print("Вы ввели неверное число")
  else:
    break

ChtotoProishodid(numberOne, numberTwo)