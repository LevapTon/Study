def AbsoluteNumber(number):
  count = 0
  for i in range(1, number):
    if number % i == 0:
      count += i
      i += 1
    else:
      i += 1
  if count == number:
    print("Число совершенное")
  else:
    print("Число не совершенное")


while True:
  number = int(input("Введите число из диапозона [1; 1000] – "))
  if number < 1 or number > 1000:
    print("Вы ввели неверное число")
  else:
    break

AbsoluteNumber(number)