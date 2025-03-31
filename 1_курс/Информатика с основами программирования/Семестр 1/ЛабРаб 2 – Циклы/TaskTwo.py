def SimpleNumber(number):
  numTwo = int(number - 1)
  if (numTwo**numTwo - 1) % number == 0: # теорема Ферма
    print("Число простое")
  else:
    print("Число не простое")


while True:
  number = int(input("Введите число из диапозона [1; 1000] – "))
  if number < 1 or number > 1000:
    print("Вы ввели неверное число")
  else:
    break

SimpleNumber(number)