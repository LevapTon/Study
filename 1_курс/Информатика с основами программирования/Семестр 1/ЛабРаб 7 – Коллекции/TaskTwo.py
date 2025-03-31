def print_digit(dig):
    for line in dig:
        print(line)


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


for i in range(len(digits)):
    dig = digits[i]  # найдём образ цифры
    print_digit(dig)  # распечатаем образ цифры
    print()