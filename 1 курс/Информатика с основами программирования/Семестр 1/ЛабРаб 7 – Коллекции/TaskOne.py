from random import randint


def gen_list_of_numbers():
  result = set()
  while True:
    result.add(randint(0, 9))
    if len(result) == 4:
      break
  print(*result)


gen_list_of_numbers()