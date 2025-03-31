def get_max(num):
  max_num = 0
  while num > 0:
    tmp = num % 10
    num = num // 10
    if tmp > max_num:
      max_num = tmp
  print('Максимальная цифра:', max_num)

def front_end():
  number = int(input('Введите целое положительное число: '))
  get_max(number)

front_end()