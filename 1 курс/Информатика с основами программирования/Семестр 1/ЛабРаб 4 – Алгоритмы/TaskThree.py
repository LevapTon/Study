def find_x():
  x = 0
  while True:
    x += 1
    count_six = 0
    func = 343**5 + 7**3 - 1 - x
    while func > 0:
      if func % 7 == 6:
        count_six += 1
      func = func // 7
    if count_six == 12:
      print('X =', x)
      break
  in_seven(343**5 + 7**3 - 1 - x)

def in_seven(num):
  result = ''
  while num > 0:
    result += str(num % 7)
    num = num // 7
  result = result[::-1]
  print(result) # Вывод ответа уравнения с основанием 7

find_x()