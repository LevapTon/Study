def perevod(num_bin):
  number = list(num_bin)
  result = 0
  n = 0
  while n < len(number):
    result += int(number[n]) * 2**(int(len(number)) - n - 1)
    n += 1
  print('Десятичное число:', result)

def front_end():
  num_bin = input('Введите двоичное число: ')
  perevod(num_bin)

front_end()