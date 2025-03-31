digits = {
    0: ('XXX', 'X X', 'X X', 'X X', 'XXX'),
    1: ('  X', ' XX', 'X X', '  X', '  X'),
    2: ('XXX', '  X', ' X ', 'X  ', 'XXX'),
    3: ('XXX', '  X', ' XX', '  X', 'XXX'),
    4: ('X X', 'X X', 'XXX', '  X', '  X'),
    5: ('XXX', 'X  ', ' X ', '  X', 'XXX'),
    6: ('XXX', 'X  ', 'XXX', 'X X', 'XXX'),
    7: ('XXX', '  X', ' X ', ' X ', ' X '),
    8: ('XXX', 'X X', 'XXX', 'X X', 'XXX'),
    9: ('XXX', 'X X', 'XXX', '  X', 'XXX')
}


def print_index(index):
  for i in range(len(digits[0])):
    for obj in index:
      print(digits[int(obj)][i].ljust(5, ' '), end='')
    print()


def input_index():
  index = input('Введите индекс: ')
  print_index(index)


input_index()