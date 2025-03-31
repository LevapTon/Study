def Pyramid(num):
  for i in range(1, num + 1):
    for n in range(1, i + 1):
      print(i, end = "")
    print("")


while True:
  number = int(input("Введите число из диапозона [1; 9] – "))
  if number < 1 or number > 9:
    print("Вы ввели неверное число")
  else:
    break

Pyramid(number)